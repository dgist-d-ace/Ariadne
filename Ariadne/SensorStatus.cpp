#include "SensorStatus.h"

#include <iostream>


#define UPDATE_PLATFORM_STATUS 100
#define UPDATE_SENSOR_CONNECTION 101
#define UPDATE_SENSOR_STATUS 102
#define UPDATE_SENSOR_AUTOSTARTUP 103

void PlatformComThread::comPlatform() // 추후 인자로 CString이 들어갈 것
{
    dataContainer = DataContainer::getInstance();
    //플랫폼 통신 코드입니다.
    ComPlatform _serial;

    std::cout << "플랫폼 커뮤니케이션 시작\n";

    if (_serial.OpenPort(L"COM6"))   // 실제 사용될 COM Port 를 넣어야합니다.  
    {
        _serial.ConfigurePort(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT);
        _serial.SetCommunicationTimeouts(0, 0, 0, 0, 0);

        while (loopStatusPlatform)
        {
            _serial.MyCommRead();
            _serial.MyCommWrite();
            dataContainer->updateValue_platform_status();

            emit(AorMChanged(dataContainer->getValue_PtoU_AorM()));
            emit(EStopChanged(dataContainer->getValue_PtoU_E_STOP()));
            emit(SpeedChanged(dataContainer->getValue_PtoU_SPEED()));
            emit(SteerChanged(dataContainer->getValue_PtoU_STEER()));
            emit(GearChanged(dataContainer->getValue_PtoU_GEAR()));
            emit(BreakChanged(dataContainer->getValue_PtoU_BRAKE()));
            emit(EncChanged(dataContainer->getValue_PtoU_ENC()));

            this->msleep(100);
        }
    }
    else
    {
        _serial.ClosePort();
    }
}

void PlatformComThread::run() {
    cout << "플랫폼 스레드가 생성되었습니다.\n";
    CString temp;
    comPlatform();
}

int LidarComThread::comLidar() {
    
    LastOfLiDAR lol;
    ObjectVector ov;

    if (!lol.Initialize()) {
        cout << "Connect ERROR!!!" << endl;
        return -1;
    }

    lol.StartCapture();

    while (1) {
        if (lol.m_bDataAvailable) {
            lol.imgLiDAR = cv::Mat::zeros(768, 1366, CV_8UC3);

            lol.GetValidDataRTheta(lol.validScans);
            lol.Conversion(lol.validScans, lol.finQVecXY, 5);
            lol.Average(lol.finQVecXY, lol.finVecXY, 5);
            lol.Clustering(lol.finVecXY, lol.finLiDARData);
            lol.Vector(lol.finLiDARData, lol.finVecData, lol.finBoolData);
            lol.DrawData(lol.finVecXY, lol.finLiDARData, lol.finVecData, lol.finBoolData, lol.imgLiDAR);

            //ov.PlatformVector(lol.finLiDARData, ov.finVecData, ov.finBoolData);
            //ov.DrawVector(lol.finLiDARData, ov.finVecData, lol.imgLiDAR);

            cout << "Reset" << endl;

            cv::imshow("DrawLiDARData", lol.imgLiDAR);
        }

        int key = cv::waitKey(1);

        if (key == 27) {
            break;
        }
    }

    lol.StopCapture();
    lol.UnInitialize();
    return 0;
}

void LidarComThread::run() {
	cout << "라이다 스레드가 생성되었습니다.\n";
	comLidar();

}

void ScnnThread::run() { 
    cout << "scnn thread" << endl;
    //mainfun();
}


int ScnnThread::mainfun()
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



/// SensorStatus class를 사용하지 않아 필요 없게 됨
/*
SensorStatus::SensorStatus() {
    dataContainer = DataContainer::getInstance();

    dataContainer->setValue_UtoP_AorM(1);

    //SetTimer(hwnd, UPDATE_SENSOR_AUTOSTARTUP, 1000, TimerProc);

    TimerAutostartup = new QTimer(this);
    QTimer::connect(TimerAutostartup, &QTimer::timeout, this, &SensorStatus::SlotUpdateSensorAutostartup);
    TimerAutostartup->start(1000);
}

void SensorStatus::SlotUpdatePlatformStatus() {
	showPlatformControlValue();
}

void SensorStatus::SlotUpdateSensorConnection() {
	updateSensorConnection();
}

void SensorStatus::SlotUpdateSensorStatus() {
	updateSensorStatus();
}

void SensorStatus::SlotUpdateSensorAutostartup() {
	updateSensorAutostartup();
}

void SensorStatus::updateSensorConnection() {

    switch (sensorCount) {
    case 0:
        break;
    case 1:
		cout << "플랫폼 실행\n";
		//thread 종료 시 다시 시작하기 위해서 slot에 thread를 다시 시작하는 함수를 구현할 필요가 있음
		//connect(&platformcom_thread, SIGNAL(finished()), this, SLOT(quit()));
		platformcom_thread.start();
        break;
    case 2:
		cout << "라이다 실행\n";
		//connect(&lidarcom_thread, SIGNAL(finished()), this, SLOT(quit()));
		lidarcom_thread.start();
        break;
    case 3:
		cout << "카메라 실행\n";
		//connect(&camera1com_thread, SIGNAL(finished()), this, SLOT(quit()));
		camera1com_thread.start();
        break;
    case 4:
		cout << "gps 실행\n";
		//connect(&gpscom_thread, SIGNAL(finished()), this, SLOT(quit()));
		gpscom_thread.start();
        break;
    case 5:
        sensorCount = -1;
        delete TimerSensorConnection;
        TimerSensorStatus = new QTimer(this);
        QTimer::connect(TimerSensorStatus, &QTimer::timeout, this, &SensorStatus::SlotUpdateSensorStatus);
        TimerSensorStatus->start(1000);
    }
    sensorCount++;
}


void SensorStatus::updateSensorStatus()
{
    using namespace std;

    //센서 정보
    // 플랫폼 연결 상태
    if (dataContainer->getValue_platform_status() > 5)
    {
        cout << "플랫폼과 연결되었습니다." << endl;
    }
    else if (dataContainer->getValue_platform_status() > 0)
    {
        cout << "플랫폼과 통신이 지연되고 있습니다." << endl;
    }
    else if (dataContainer->getValue_platform_status() == 0)
    {
        cout << "플랫폼과 통신이 실패하였습니다." << endl;
    }
    dataContainer->setValue_platform_status(0);

    // LiDAR 연결 상태
    if (dataContainer->getValue_lidar_status() > 5)
    {
        cout << "라이다와 연결되었습니다." << endl;
    }

    else if (dataContainer->getValue_lidar_status() == 0)
    {
        cout << "라이다와 통신이 지연되고 있습니다." << endl;
    }
    else if (dataContainer->getValue_lidar_status() < 0)
    {
        cout << "라이다와 통신이 실패하였습니다." << endl;
    }

    dataContainer->setValue_lidar_status(dataContainer->getValue_lidar_status() - 1);

    // CAMERA1 연결 상태
    if (dataContainer->getValue_camera1_status() > 5)
    {
        cout << "카메라1과 연결되었습니다." << endl;
    }
    else if (dataContainer->getValue_camera1_status() > 0)
    {
        cout << "카메라1과 통신이 지연되고 있습니다." << endl;
    }
    else if (dataContainer->getValue_camera1_status() == 0)
    {
        cout << "카메라1과 통신이 실패하였습니다." << endl;
    }
    dataContainer->setValue_camera1_status(0);


    // GPS 연결상태
    if (dataContainer->getValue_gps_status() > 5)
    {
        cout << "GPS와 연결되었습니다." << endl;
    }
    else if (dataContainer->getValue_gps_status() > 0)
    {
        cout << "GPS와 통신이 지연되고 있습니다." << endl;
    }
    else if (dataContainer->getValue_gps_status() == 0)
    {
        cout << "GPS와 통신이 실패하였습니다." << endl;
    }
    dataContainer->setValue_gps_status(0);
}
*/


/*
void SensorStatus::updateSensorAutostartup()
{
    switch (sensorAutoCount)
    {
    case 0:
        sensorAutoCount += 2;
        break;

    case 1:
        break;

    case 2:
        break;

    case 3:
        break;

    case 4:
        break;

    case 5:
        break;

    case 6:
        break;

    case 7:
        break;

    case 8:
        break;

    case 9:
        break;

    case 10:
        // 센서 연결 타이머 실행
        //SetTimer(hwnd, UPDATE_SENSOR_CONNECTION, 1000, NULL);
        TimerSensorConnection = new QTimer(this);
        QTimer::connect(TimerSensorConnection, &QTimer::timeout, this, &SensorStatus::SlotUpdateSensorConnection);
        TimerSensorConnection->start(1000);
        break;

    case 11:
        break;
    case 12:
        break;
    case 13:
        break;
    case 14:
        break;
    case 15:
        break;
    case 16:
        break;
    case 17:
        break;
    case 18:
        break;

    case 19:
        // 오토 모드 실행
            // 오토 모드 스레드 실행

        if (!mission_thread.isRunning())
        {
			//connect(&mission_thread, SIGNAL(finished()), this, SLOT(deleteLater()));
			mission_thread.start();
        }

        sensorAutoCount = -1;
        //KillTimer(hwnd, UPDATE_SENSOR_AUTOSTARTUP);
        delete TimerAutostartup;
        break;
    }
    sensorAutoCount++;
}


void SensorStatus::showPlatformControlValue() {
    //구현해야함
}
*/



/*
void SensorStatus::comLidar() {}
void SensorStatus::comCamera1() {}
void SensorStatus::comGps() {}


void MissionThread::run() {
	//구현 필요
	cout << "미션 스레드가 생성되었습니다.\n";
	exec();
}


void PlatformComThread::run() {
	//구현 필요
	cout << "플랫폼 스레드가 생성되었습니다.\n";
	exec();

*/


/*
void LidarComThread::run() {
	//구현 필요
	cout << "라이다 스레드가 생성되었습니다.\n";
	exec();
}

void Camera1ComThread::run() {
	//구현 필요
	cout << "카메라1 스레드가 생성되었습니다.\n";
	exec();
}

void Camera2ComThread::run() {
	//구현 필요
	cout << "카메라2 스레드가 생성되었습니다.\n";
	exec();
}

void RTKComThread::run() {
	//구현 필요
	cout << "GPS 스레드가 생성되었습니다.\n";
	exec();
	

}
*/
