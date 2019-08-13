import numpy as np
import cv2

import torch
import torch.nn as nn
import torch.nn.functional as F
import torchvision.models as models
from torchvision.transforms import Normalize as Normalize_th

import time

class SCNN(nn.Module):
    def __init__(self, ms_ks=9, pretrained=True):
        """
        Argument
            ms_ks: kernel size in message passing conv
        """
        super(SCNN, self).__init__()
        self.pretrained = pretrained
        self.net_init(ms_ks)
        if not pretrained:
            self.weight_init()

        self.scale_background = 0.4
        self.scale_seg = 1.0
        self.scale_exist = 0.1

        self.ce_loss = nn.CrossEntropyLoss(weight=torch.tensor([self.scale_background, 1, 1, 1, 1]))
        self.bce_loss = nn.BCELoss(reduction='sum')

    def forward(self, img, seg_gt=None, exist_gt=None):
        x = self.backbone(img)
        x = self.layer1(x)
        x = self.message_passing_forward(x)
        x = self.layer2(x)

        seg_pred = F.interpolate(x, scale_factor=8, mode='bilinear', align_corners=True)
        x = self.layer3(x)
        x = x.view(-1, 4500)
        exist_pred = self.fc(x)

        if seg_gt is not None and exist_gt is not None:
            loss_seg = self.ce_loss(seg_pred, seg_gt) * self.scale_seg
            loss_exist = self.bce_loss(exist_pred, exist_gt) / img.shape[0] * self.scale_exist
            loss = loss_seg + loss_exist
        else:
            loss_seg = torch.tensor(0, dtype=img.dtype, device=img.device)
            loss_exist = torch.tensor(0, dtype=img.dtype, device=img.device)
            loss = torch.tensor(0, dtype=img.dtype, device=img.device)

        return seg_pred, exist_pred, loss_seg, loss_exist, loss

    def message_passing_forward(self, x):
        Vertical = [True, True, False, False]
        Reverse = [False, True, False, True]
        for ms_conv, v, r in zip(self.message_passing, Vertical, Reverse):
            x = self.message_passing_once(x, ms_conv, v, r)
        return x

    def message_passing_once(self, x, conv, vertical=True, reverse=False):
        """
        Argument:
        ----------
        x: input tensor
        vertical: vertical message passing or horizontal
        reverse: False for up-down or left-right, True for down-up or right-left
        """
        nB, C, H, W = x.shape
        if vertical:
            slices = [x[:, :, i:(i + 1), :] for i in range(H)]
            dim = 2
        else:
            slices = [x[:, :, :, i:(i + 1)] for i in range(W)]
            dim = 3
        if reverse:
            slices = slices[::-1]

        out = [slices[0]]
        for i in range(1, len(slices)):
            out.append(slices[i] + F.relu(conv(out[i - 1])))
        if reverse:
            out = out[::-1]
        return torch.cat(out, dim=dim)

    def net_init(self, ms_ks):
        self.backbone = models.vgg16_bn(pretrained=self.pretrained).features

        # ----------------- process backbone -----------------
        for i in [34, 37, 40]:
            conv = self.backbone._modules[str(i)]
            dilated_conv = nn.Conv2d(
                conv.in_channels, conv.out_channels, conv.kernel_size, stride=conv.stride,
                padding=tuple(p * 2 for p in conv.padding), dilation=2, bias=(conv.bias is not None)
            )
            dilated_conv.load_state_dict(conv.state_dict())
            self.backbone._modules[str(i)] = dilated_conv
        self.backbone._modules.pop('33')
        self.backbone._modules.pop('43')

        # ----------------- SCNN part -----------------
        self.layer1 = nn.Sequential(
            nn.Conv2d(512, 1024, 3, padding=4, dilation=4, bias=False),
            nn.BatchNorm2d(1024),
            nn.ReLU(),
            nn.Conv2d(1024, 128, 1, bias=False),
            nn.BatchNorm2d(128),
            nn.ReLU()  # (nB, 128, 36, 100)
        )

        # ----------------- add message passing -----------------
        self.message_passing = nn.ModuleList()
        self.message_passing.add_module('up_down', nn.Conv2d(128, 128, (1, ms_ks), padding=(0, ms_ks // 2), bias=False))
        self.message_passing.add_module('down_up', nn.Conv2d(128, 128, (1, ms_ks), padding=(0, ms_ks // 2), bias=False))
        self.message_passing.add_module('left_right', nn.Conv2d(128, 128, (ms_ks, 1), padding=(ms_ks // 2, 0), bias=False))
        self.message_passing.add_module('right_left', nn.Conv2d(128, 128, (ms_ks, 1), padding=(ms_ks // 2, 0), bias=False))
        # (nB, 128, 36, 100)

        # ----------------- SCNN part -----------------
        self.layer2 = nn.Sequential(
            nn.Dropout2d(0.1),
            nn.Conv2d(128, 5, 1)  # get (nB, 5, 36, 100)
        )

        self.layer3 = nn.Sequential(
            nn.Softmax(dim=1),  # (nB, 5, 36, 100)
            nn.AvgPool2d(2, 2),  # (nB, 5, 18, 50)
        )
        self.fc = nn.Sequential(
            nn.Linear(4500, 128),
            nn.ReLU(),
            nn.Linear(128, 4),
            nn.Sigmoid()
        )

    def weight_init(self):
        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                m.reset_parameters()
            elif isinstance(m, nn.BatchNorm2d):
                m.weight.data[:] = 1.
                m.bias.data.zero_()


class CustomTransform:
    def __call__(self, *args, **kwargs):
        raise NotImplementedError

    def __str__(self):
        return self.__class__.__name__

    def __eq__(self, name):
        return str(self) == name

    def __iter__(self):
        def iter_fn():
            for t in [self]:
                yield t
        return iter_fn()

    def __contains__(self, name):
        for t in self.__iter__():
            if isinstance(t, Compose):
                if name in t:
                    return True
            elif name == t:
                return True
        return False


class Compose(CustomTransform):
    """
    All transform in Compose should be able to accept two non None variable, img and boxes
    """
    def __init__(self, *transforms):
        self.transforms = [*transforms]

    def __call__(self, img, segLabel=None, exist=None):
        for t in self.transforms:
            img, segLabel, exist = t(img=img, segLabel=segLabel, exist=exist)
        return img, segLabel, exist

    def __iter__(self):
        return iter(self.transforms)

    def modules(self):
        yield self
        for t in self.transforms:
            if isinstance(t, Compose):
                for _t in t.modules():
                    yield _t
            else:
                yield t


class Resize(CustomTransform):
    def __init__(self, size):
        if isinstance(size, int):
            size = (size, size)
        self.size = size  #(W, H)

    def __call__(self, img, segLabel=None, exist=None):
        img = cv2.resize(img, self.size, interpolation=cv2.INTER_CUBIC)
        if segLabel is not None:
            segLabel = cv2.resize(segLabel, self.size, interpolation=cv2.INTER_NEAREST)
        return img, segLabel, exist

    def reset_size(self, size):
        if isinstance(size, int):
            size = (size, size)
        self.size = size


class Normalize(CustomTransform):
    def __init__(self, mean, std):
        self.transform = Normalize_th(mean, std)

    def __call__(self, img, segLabel=None, exist=None):
        img = self.transform(img)
        return img, segLabel, exist


class ToTensor(CustomTransform):
    def __init__(self, dtype=torch.float):
        self.dtype=dtype

    def __call__(self, img, segLabel=None, exist=None):
        img = img.transpose(2, 0, 1)
        img = torch.from_numpy(img).type(self.dtype) / 255.
        if segLabel is not None:
            segLabel = torch.from_numpy(segLabel).type(torch.long)
        if exist is not None:
            exist = torch.FloatTensor(exist)
        return img, segLabel, exist


def getLane(prob_map, y_px_gap, pts, thresh, resize_shape=None):
    """
    Arguments:
    ----------
    prob_map: prob map for single lane, np array size (h, w)
    resize_shape:  reshape size target, (H, W)

    Return:
    ----------
    coords: x coords bottom up every y_px_gap px, 0 for non-exist, in resized shape
    """
    if resize_shape is None:
        resize_shape = prob_map.shape
    h, w = prob_map.shape
    H, W = resize_shape

    coords = np.zeros(pts)
    for i in range(pts):
        y = int(h - i*y_px_gap / H * h - 1)
        if y < 0:
            break
        line = prob_map[y, :]
        id = np.argmax(line)
        if line[id] > thresh:
            coords[i] = int(id / w * W)
    if (coords>0).sum() < 2:
        coords = np.zeros(18)
    return coords


def prob2lines(seg_pred, exist, resize_shape=None, smooth=True, y_px_gap=20, pts=None, thresh=0.3):
    """
    Arguments:
    ----------
    seg_pred: np.array size (5, h, w)
    resize_shape:  reshape size target, (H, W)
    exist:   list of existence, e.g. [0, 1, 1, 0]
    smooth:  whether to smooth the probability or not
    y_px_gap: y pixel gap for sampling
    pts:     how many points for one lane
    thresh:  probability threshold

    Return:
    ----------
    coordinates: [x, y] list of lanes, e.g.: [ [[9, 569], [50, 549]] ,[[630, 569], [647, 549]] ]
    """
    if resize_shape is None:
        resize_shape = seg_pred.shape[1:] # seg_pred (5, h, w)
    _, h, w = seg_pred.shape
    H, W = resize_shape
    coordinates = []

    if pts is None:
        pts = round(H/2 / y_px_gap)

    seg_pred = np.ascontiguousarray(np.transpose(seg_pred, (1, 2, 0)))
    for i in range(4):
        prob_map = seg_pred[..., i+1]
        if smooth:
            prob_map = cv2.blur(prob_map, (9, 9), borderType=cv2.BORDER_REPLICATE)
        if exist[i]>0:
            coords = getLane(prob_map, y_px_gap, pts, thresh, resize_shape)
            coordinates.append([[coords[i], H-1-i*y_px_gap] for i in range(pts) if coords[i]>0])

    return coordinates


# SCNN INIT
net = SCNN(pretrained=False)
mean=(0.3598, 0.3653, 0.3662) # CULane mean, std
std=(0.2573, 0.2663, 0.2756)
transform = Compose(Resize((800, 288)), ToTensor(),
                    Normalize(mean=mean, std=std))

print("fuck2")
device = torch.device("cuda:0")
print(device)

print("fuck2")
print("fuck2")
print("fuck2")

# PARAM INIT
capture = 0
save_dict = 0
# frametime = 0
visual = False


def scnn_init(weight_path, img_path = 0, visualize = False):
    global capture
    print("fucking")
    #capture = cv2.VideoCapture(img_path)

    # Network
    global save_dict
    save_dict = torch.load(weight_path, map_location="cuda:0")
    net.load_state_dict(save_dict['net'])
    net.to(device)
    net.eval()

    global visual
    visual = visualize

    # global frametime
    # frametime = 0


def scnn_run():
    # frame_capture_time = time.time()
    ret, frame = capture.read()

    # image_to_tensor_time = time.time()
    img = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    # gpu_calculation_time = time.time()
    seg_pred, exist_pred = net(transform(img)[0].unsqueeze_(0).cuda())[:2]

    # cpu_calculation_time = time.time()
    seg_pred = seg_pred.data.cpu().numpy()
    exist_pred = exist_pred.data.cpu().numpy()

    seg_pred = seg_pred[0]
    exist = [1 if exist_pred[0, i] > 0.0 else 0 for i in range(4)]

    # image_show_time = time.time()
    img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)
    img = cv2.resize(img, (800, 288))
    lane_img = np.zeros_like(img)
    color = np.array([[255, 125, 0], [0, 255, 0], [0, 0, 255], [0, 255, 255]], dtype='uint8')
    coord_mask = np.argmax(seg_pred, axis=0)

    for i in range(0, 4):
        if exist_pred[0, i] > 0.0:
            lane_img[coord_mask == (i + 1)] = color[i]
    img = cv2.addWeighted(src1=lane_img, alpha=0.8, src2=img, beta=1., gamma=0.)

    # output_time = time.time()
    #print([1 if exist_pred[0, i] > 0.5 else 0 for i in range(4)])
    params = []
    params.append([1 if exist_pred[0, i] > 0.5 else 0 for i in range(4)])
    for n, x in enumerate(prob2lines(seg_pred, exist, resize_shape=None, smooth=True, y_px_gap=20, pts=10, thresh=0.95)):
        #print(x)
        params.append(x)
        for i in range(len(x) - 2):
            if exist_pred[0, n] > 0.5:
                cv2.line(img, (int(x[i][0]), int(x[i][1])), (int(x[i + 1][0]), int(x[i + 1][1])), (255, 0, 0), 3)

    if visual:
        cv2.imshow("OutputFrame", img)
        print(params)

    # global frametime
    # frametime += 1
    # print(frametime)

    # print("frame_capture_time :", image_to_tensor_time - frame_capture_time)
    # print("image_to_tensor_time :", gpu_calculation_time - image_to_tensor_time)
    # print("gpu_calculation_time :", cpu_calculation_time - gpu_calculation_time)
    # print("cpu_calculation_time :", image_show_time - cpu_calculation_time)
    # print("image_show_time :", output_time - image_show_time)
    # print("output_time :", time.time() - output_time)
    # print("total_time :", time.time() - frame_capture_time)

    if cv2.waitKey(33) > 0:
        return

    return params


def scnn_destroy():
    capture.release()
    cv2.destroyAllWindows()


def main():
    #scnn_init("experiments/exp1/exp1_kcity_best_50.pth", "demo/WIN_20190628_14_14_39_Pro.mp4", True)
    scnn_init("exp1_kcity_best_50.pth", "test.mp4", True)
    while True:
        scnn_run()
    scnn_destroy()


if __name__ == "__main__":
    main()
