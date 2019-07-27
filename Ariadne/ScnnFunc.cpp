#include <torch/script.h>
#include <torch/torch.h> 
#include <torch/data/transforms/tensor.h> 
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ScnnFunc.h"

int mainfun()
{
        std::shared_ptr<torch::jit::script::Module> module = torch::jit::load("model.pt");
        assert(module != nullptr);
        std::cout << "load model ok\n";

        // Create a vector of inputs.
        std::vector<torch::jit::IValue> inputs;
        inputs.push_back(torch::rand({ 64, 3, 224, 224 }));

        // evalute time
        double t = (double)cv::getTickCount();
        module->forward(inputs).toTensor();
        t = (double)cv::getTickCount() - t;
        printf("execution time = %gs\n", t / cv::getTickFrequency());
        inputs.pop_back();


        std::cout << "test11" << std::endl;

        // load image with opencv and transform
        cv::Mat image;
        image = cv::imread("dog.png", 1);
        cv::cvtColor(image, image, CV_BGR2RGB);
        cv::Mat img_float;
        image.convertTo(img_float, CV_32F, 1.0 / 255);
        cv::resize(img_float, img_float, cv::Size(224, 224));

        std::cout << "test12" << std::endl;

        //std::cout << img_float.at<cv::Vec3f>(56,34)[1] << std::endl;
        //auto img_tensor = torch::CPU(torch::kFloat32).tensorFromBlob(img_float.data, { 1, 224, 224, 3 });
        //auto img_tensor = torch::from_blob(img_float.data, { 1, 224, 224, 3 }).to(torch::kCUDA);
        auto img_tensor = torch::from_blob(img_float.data, { 1, 224, 224, 3 });
        img_tensor = img_tensor.permute({ 0,3,1,2 });
        img_tensor[0][0] = img_tensor[0][0].sub_(0.485).div_(0.229);
        img_tensor[0][1] = img_tensor[0][1].sub_(0.456).div_(0.224);
        img_tensor[0][2] = img_tensor[0][2].sub_(0.406).div_(0.225);

        std::cout << "test13" << std::endl;

        //auto img_var = torch::autograd::make_variable(img_tensor, false);
    //	auto img_var = torch::autograd::make_variable(img_tensor, false);
        //auto img_var = torch::autograd::make_variable(img_tensor, false).data();

    //	inputs.push_back(img_var);
        inputs.push_back(img_tensor);

        std::cout << "test14" << std::endl;

        // Execute the model and turn its output into a tensor.
        torch::Tensor out_tensor = module->forward(inputs).toTensor();

        std::cout << "test15" << std::endl;

        std::cout << out_tensor.slice(/*dim=*/1, /*start=*/0, /*end=*/10) << '\n';


        std::cout << "test" << std::endl;

        // Load labels
        std::string label_file = "synset_words.txt";
        std::ifstream rf(label_file.c_str());
        CHECK(rf) << "Unable to open labels file " << label_file;
        std::string line;
        std::vector<std::string> labels;
        while (std::getline(rf, line))
            labels.push_back(line);

        std::cout << "test" << std::endl;

        // print predicted top-5 labels
        std::tuple<torch::Tensor, torch::Tensor> result = out_tensor.sort(-1, true);
        torch::Tensor top_scores = std::get<0>(result)[0];
        torch::Tensor top_idxs = std::get<1>(result)[0].toType(torch::kInt32);

        std::cout << "test" << std::endl;
        auto top_scores_a = top_scores.accessor<float, 1>();
        std::cout << "test" << std::endl;
        auto top_idxs_a = top_idxs.accessor<int, 1>();

        std::cout << top_idxs_a[0] << std::endl;

        for (int i = 0; i < 5; ++i) {
            int idx = top_idxs_a[i];
            std::cout << "top-" << i + 1 << " label: ";
            std::cout << labels[idx] << ", score: " << top_scores_a[i] << std::endl;
        }
    return 0;
}