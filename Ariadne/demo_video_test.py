# pythom demo_video_test -i demo/testsizemore.mp4 -w experiments/vgg_SCNN_DULR_w9/vgg_SCNN_DULR_w9.pth -v

import argparse
import numpy as np
import cv2
import random

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
        self.message_passing.add_module('left_right',
                                        nn.Conv2d(128, 128, (ms_ks, 1), padding=(ms_ks // 2, 0), bias=False))
        self.message_passing.add_module('right_left',
                                        nn.Conv2d(128, 128, (ms_ks, 1), padding=(ms_ks // 2, 0), bias=False))
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


class RandomResize(Resize):
    """
    Resize to (w, h), where w randomly samples from (minW, maxW) and h randomly samples from (minH, maxH)
    """
    def __init__(self, minW, maxW, minH=None, maxH=None, batch=False):
        if minH is None or maxH is None:
            minH, maxH = minW, maxW
        super(RandomResize, self).__init__((minW, minH))
        self.minW = minW
        self.maxW = maxW
        self.minH = minH
        self.maxH = maxH
        self.batch = batch

    def random_set_size(self):
        w = np.random.randint(self.minW, self.maxW+1)
        h = np.random.randint(self.minH, self.maxH+1)
        self.reset_size((w, h))


class Rotation(CustomTransform):
    def __init__(self, theta):
        self.theta = theta

    def __call__(self, img, segLabel=None, exist=None):
        u = np.random.uniform()
        degree = (u-0.5) * self.theta
        R = cv2.getRotationMatrix2D((img.shape[1]//2, img.shape[0]//2), degree, 1)
        img = cv2.warpAffine(img, R, (img.shape[1], img.shape[0]), flags=cv2.INTER_LINEAR)
        if segLabel is not None:
            segLabel = cv2.warpAffine(segLabel, R, (segLabel.shape[1], segLabel.shape[0]), flags=cv2.INTER_NEAREST)

        return img, segLabel, exist

    def reset_theta(self, theta):
        self.theta = theta


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


class SquarePad(CustomTransform):
    def __init__(self, mode='constant', value=0):
        mode_dict = {
            'constant': cv2.BORDER_CONSTANT,
            'replicate': cv2.BORDER_REPLICATE,
            'reflect': cv2.BORDER_REFLECT
        }
        self.mode = mode_dict[mode]
        self.value = value

    def __call__(self, img, boxes):
        H, W = img.shape[:2]
        diff = abs(H-W)
        if W >= H:
            img = cv2.copyMakeBorder(img, 0, diff, 0, 0, self.mode, value=self.value)
        else:
            img = cv2.copyMakeBorder(img, 0, 0, 0, diff, self.mode, value=self.value)
        boxes = boxes.copy()
        return img, boxes


class RandomFlip(CustomTransform):
    def __init__(self, px=0, py=0):
        """
        Arguments:
        ----------
        px: range [0, 1], probability to use horizontal flip
        py: range [0, 1], probability to use vertical flip
        """
        self.px = px
        self.py = py

    def __call__(self, img, boxes):
        """
        Arguments:
        ----------
        img: numpy array, shape (H, W, 3)
        boxes: numpy array: shape (N,4)
        """
        img = img.copy()
        boxes = boxes.copy()
        H, W = img.shape[:2]

        flip_x = np.random.choice([False, True], p=(1-self.px, self.px))
        flip_y = np.random.choice([False, True], p=(1-self.py, self.py))
        if flip_x:
            img = np.ascontiguousarray(np.flip(img, axis=1))
            boxes[:, ::2] = W - boxes[:, ::2]
            boxes[:, ::2] = boxes[:, -2::-2]

        if flip_y:
            img = np.ascontiguousarray(np.flip(img, axis=0))
            boxes[:, 1::2] = H - boxes[:, 1::2]
            boxes[:, 1::2] = boxes[:, -1::-2]
        return img, boxes


class RandomTranslate(CustomTransform):
    """
    Randomly translate img
    """
    def __init__(self, exceed_x=0.2, exceed_y=0.2, prob=0.5):
        """
        Argmuments:
        ----------
        exceed_x: percentage allowed for all boxes to exceed image board in x axis
        exceed_y: percentage allowed for all boxes to exceed image board in y axis
        prob: range [0, 1], larger value means larger probability to perform the operation
        """
        self.exceed_x = exceed_x
        self.exceed_y = exceed_y
        self.prob = prob

    def __call__(self, img, boxes):
        boxes = boxes.copy()

        rand_prob = random.random()
        if rand_prob <= self.prob:
            H, W = img.shape[:2]
            _img = np.zeros_like(img)

            offset_x_low = -np.min(boxes[:, 0]*(1-self.exceed_x) + boxes[:, 2]*self.exceed_x)
            offset_y_low = -np.min(boxes[:, 1] * (1 - self.exceed_y) + boxes[:, 3] * self.exceed_y)
            offset_x_high = W - np.max(boxes[:, 2] * (1 - self.exceed_x) + boxes[:, 0] * self.exceed_x)
            offset_y_high = H - np.max(boxes[:, 3] * (1 - self.exceed_y) + boxes[:, 1] * self.exceed_y)

            offset_x = random.randint(int(offset_x_low), int(offset_x_high))
            offset_y = random.randint(int(offset_y_low), int(offset_y_high))

            if offset_y<=0 and offset_x<=0:
                _img[:H+offset_y, :W+offset_x] = img[-offset_y:, -offset_x:]
            elif offset_y<=0 and offset_x>0:
                _img[:H+offset_y, offset_x:] = img[-offset_y:, :W-offset_x]
            elif offset_y>0 and  offset_x<=0:
                _img[offset_y:, :W+offset_x] = img[:H-offset_y, -offset_x:]
            else:
                _img[offset_y:, offset_x:] = img[:H-offset_y, :W-offset_x]

            boxes[:, ::2] += offset_x
            boxes[:, 1::2] += offset_y
            boxes[:, [0, 1]] = np.maximum(boxes[:, [0, 1]], 0.)
            boxes[:, 2] = np.minimum(boxes[:, 2], W)
            boxes[:, 3] = np.minimum(boxes[:, 3], H)
        else:
            _img = img.copy()
        return _img, boxes


class Random_Color_Distort(CustomTransform):
    def __init__(self, brightness=32, contrast=0.4, prob=0.5):
        self.brightness = brightness
        self.contrast = max(contrast, 1.)
        self.prob = prob

    def __call__(self, img, boxes=None):
        img = img.copy()

        rand_prob = random.random()
        if rand_prob <= self.prob:
            img = self._brightness(img)
            img = self._contrast(img)

        if boxes is not None:
            return img, boxes
        return img

    def _brightness(self, img):
        low = np.min(img)
        high = 255 - np.max(img)
        value = random.randint(-self.brightness, self.brightness)
        value = np.uint8(min(min(low, value), high))
        img += value
        return img

    def _contrast(self, img):
        value = random.uniform(1-self.contrast, 1+self.contrast)
        _img = img.copy().astype('float')
        _img *= max(value, np.max(_img / 255.))
        return _img.astype('uint8')


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



net = SCNN(pretrained=False)
mean=(0.3598, 0.3653, 0.3662) # CULane mean, std
std=(0.2573, 0.2663, 0.2756)
transform = Compose(Resize((800, 288)), ToTensor(),
                    Normalize(mean=mean, std=std))

device = torch.device("cuda:0")
print(device)
# ts = torch.rand(3,3,device=device)
# print(ts)

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--img_path", '-i', type=str, default="demo/demo.jpg", help="Path to demo img")
    parser.add_argument("--weight_path", '-w', type=str, help="Path to model weights")
    parser.add_argument("--visualize", '-v', action="store_true", default=False, help="Visualize the result")
    args = parser.parse_args()
    return args


def main():
    capture = cv2.VideoCapture("test.mp4")

    # Network
    #save_dict = torch.load(weight_path, map_location='cpu')
    save_dict = torch.load("exp1_kcity_best_50.pth", map_location="cuda:0")
    net.load_state_dict(save_dict['net'])
    net.to(device)
    net.eval()

    frametime = 0

    while True:
        if (capture.get(cv2.CAP_PROP_POS_FRAMES) == capture.get(cv2.CAP_PROP_FRAME_COUNT)):
            capture.open(img_path)

        frame_capture_time = time.time()
        """
        ret, frame = capture.read()
        ret, frame = capture.read()
        ret, frame = capture.read()
        ret, frame = capture.read()
        """
        ret, frame = capture.read()

        image_to_tensor_time = time.time()
        img = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        x = transform(img)[0]
        x = x.to(device)
        x.unsqueeze_(0)

        gpu_calculation_time = time.time()
        seg_pred, exist_pred = net(x)[:2]

        cpu_calculation_time = time.time()
        seg_pred = seg_pred.detach().cpu().numpy()
        exist_pred = exist_pred.detach().cpu().numpy()
        # seg_pred = (seg_pred.cuda()).detach().cpu().numpy()
        # exist_pred = (exist_pred.cuda()).detach().cpu().numpy()
        seg_pred = seg_pred[0]
        exist = [1 if exist_pred[0, i] > 0.0 else 0 for i in range(4)]

        image_show_time = time.time()
        img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)
        img = cv2.resize(img, (800, 288))
        lane_img = np.zeros_like(img)
        color = np.array([[255, 125, 0], [0, 255, 0], [0, 0, 255], [0, 255, 255]], dtype='uint8')
        coord_mask = np.argmax(seg_pred, axis=0)

        for i in range(0, 4):
            if exist_pred[0, i] > 0.0:
                lane_img[coord_mask == (i + 1)] = color[i]
        img = cv2.addWeighted(src1=lane_img, alpha=0.8, src2=img, beta=1., gamma=0.)

        output_time = time.time()
        for x in prob2lines(seg_pred, exist):
            print(x)
    
        if True:
            print([1 if exist_pred[0, i] > 0.5 else 0 for i in range(4)])
            cv2.imshow("OutputFrame", img)

        frametime += 1
        print(frametime)

        print("frame_capture_time :", image_to_tensor_time - frame_capture_time)
        print("image_to_tensor_time :", gpu_calculation_time - image_to_tensor_time)
        print("gpu_calculation_time :", cpu_calculation_time - gpu_calculation_time)
        print("cpu_calculation_time :", image_show_time - cpu_calculation_time)
        print("image_show_time :", output_time - image_show_time)
        print("output_time :", time.time() - output_time)
        print("total_time :", time.time() - frame_capture_time)

        if cv2.waitKey(2) == 'q':
            break

    capture.release()
    cv2.destroyAllWindows()

    # 원본 코드
    """
    img = cv2.imread(img_path)
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    x = transform(img)[0]
    x.unsqueeze_(0)

    save_dict = torch.load(weight_path, map_location='cpu')
    net.load_state_dict(save_dict['net'])
    net.eval()

    seg_pred, exist_pred = net(x)[:2]
    seg_pred = seg_pred.detach().cpu().numpy()
    exist_pred = exist_pred.detach().cpu().numpy()
    seg_pred = seg_pred[0]
    exist = [1 if exist_pred[0, i] > 0.5 else 0 for i in range(4)]

    img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)
    img = cv2.resize(img, (800, 288))
    lane_img = np.zeros_like(img)
    color = np.array([[255, 125, 0], [0, 255, 0], [0, 0, 255], [0, 255, 255]], dtype='uint8')
    coord_mask = np.argmax(seg_pred, axis=0)
    for i in range(0, 4):
        if exist_pred[0, i] > 0.5:
            lane_img[coord_mask == (i + 1)] = color[i]
    img = cv2.addWeighted(src1=lane_img, alpha=0.8, src2=img, beta=1., gamma=0.)
    cv2.imwrite("demo/demo_result.jpg", img)

    for x in getLane.prob2lines(seg_pred, exist):
        print(x)

    if args.visualize:
        print([1 if exist_pred[0, i] > 0.5 else 0 for i in range(4)])
        cv2.imshow("", img)
        cv2.waitKey(0)
        cv2.destroyAllWindows()
    """

if __name__ == "__main__":
    main()
