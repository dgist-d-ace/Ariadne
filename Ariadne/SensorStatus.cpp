#include "SensorStatus.h"
#include "DataContainer.h"

#define UPDATE_PLATFORM_STATUS 100
#define UPDATE_SENSOR_CONNECTION 101
#define UPDATE_SENSOR_STATUS 102
#define UPDATE_SENSOR_AUTOSTARTUP 103

DataContainer *dataContainer;

SensorStatus::SensorStatus() {
    dataContainer = DataContainer::getInstance();
    //	MainWindow = selfDriving::getInstance();

    dataContainer->setValue_UtoP_AorM(1);

    //SetTimer(hwnd, UPDATE_SENSOR_AUTOSTARTUP, 1000, TimerProc);

    TimerAutostartup = new QTimer(MainWindow);
    //	QTimer::connect(TimerAutostartup, SIGNAL(timeout()), MainWindow, SLOT(updateSensor(UPDATE_SENSOR_AUTOSTARTUP)));
    TimerAutostartup->start(1000);
}

void SensorStatus::updateSensor(UINT_PTR nIDEvent)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

    switch (nIDEvent)
    {
    case UPDATE_PLATFORM_STATUS:
        showPlatformControlValue();
        break;

    case UPDATE_SENSOR_CONNECTION:
        updateSensorConnection();
        break;

    case UPDATE_SENSOR_STATUS:
        updateSensorStatus();
        break;

    case UPDATE_SENSOR_AUTOSTARTUP:
        updateSensorAutostartup();
        break;

    }
}
void SensorStatus::updateSensorConnection() {

    switch (sensorCount) {
    case 0:
        break;
    case 1:
		cout << "플랫폼 실행\n";
        //platformcom_thread = ([&]() { comPlatform(); });
        break;
    case 2:
		cout << "라이다 실행\n";
        //lidarcom_thread = thread([&]() { comLidar(); });
        break;
    case 3:
		cout << "카메라 실행\n";
        //camera1com_thread = thread([&]() { comCamera1(); });
        break;
    case 4:
		cout << "gps 실행\n";
        //gpscom_thread = thread([&]() { comGps(); });
        break;
    case 5:
        sensorCount = -1;
        //KillTimer(hwnd, UPDATE_SENSOR_CONNECTION);
        delete TimerSensorConnection;
        //SetTimer(hwnd, UPDATE_SENSOR_STATUS, 1000, NULL);
        TimerSensorStatus = new QTimer(MainWindow);
        QTimer::connect(TimerSensorStatus, SIGNAL(timeout()), MainWindow, SLOT(updateSensor(UPDATE_SENSOR_STATUS)));
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
        cout << "플랫폼 연결" << endl;
    }
    else if (dataContainer->getValue_platform_status() > 0)
    {
        cout << "플랫폼과의 연결 지연" << endl;
    }
    else if (dataContainer->getValue_platform_status() == 0)
    {
        cout << "플랫폼과의 통신 중단" << endl;
    }
    dataContainer->setValue_platform_status(0);

    // LiDAR 연결 상태
    if (dataContainer->getValue_lidar_status() > 0)
    {
        cout << "Lidar is on" << endl;
    }

    else if (dataContainer->getValue_lidar_status() == 0)
    {
        cout << "라이다와 통신 지연" << endl;
    }
    else if (dataContainer->getValue_lidar_status() < 0)
    {
        cout << "라이다와 통신 중단" << endl;
    }

    dataContainer->setValue_lidar_status(dataContainer->getValue_lidar_status() - 1);

    // CAMERA1 연결 상태
    if (dataContainer->getValue_camera1_status() > 5)
    {
        cout << "camera 연결" << endl;
    }
    else if (dataContainer->getValue_camera1_status() > 0)
    {
        cout << "camera 연결 지연" << endl;
    }
    else if (dataContainer->getValue_camera1_status() == 0)
    {
        cout << "camera 통신 중단" << endl;
    }
    dataContainer->setValue_camera1_status(0);


    // GPS 연결상태
    if (dataContainer->getValue_gps_status() > 5)
    {
        cout << "GPS 연결" << endl;
    }
    else if (dataContainer->getValue_gps_status() > 0)
    {
        cout << "GPS 연결 지연" << endl;
    }
    else if (dataContainer->getValue_gps_status() == 0)
    {
        cout << "GPS 통신 중단" << endl;
    }
    dataContainer->setValue_gps_status(0);
}

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
        TimerSensorConnection = new QTimer(MainWindow);
        QTimer::connect(TimerSensorConnection, SIGNAL(timeout()), MainWindow, SLOT(updateSensor(UPDATE_SENSOR_CONNECTION)));
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
        //if (mission_thread.joinable())
        //{
        //    loopStatusAutomode = true;
        //}
        //else
        //{
        //    mission_thread = thread([&]() { /*automode();*/ });
        //}

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
void SensorStatus::comPlatform()
{
    //플랫폼 통신 코드입니다.

    CString comPort;

    if (_serial.OpenPort(comPort))   // 실제 사용될 COM Port 를 넣어야합니다.  
    {
        // BaudRate, ByteSize, fParity, Parity, StopBit 정보를 설정해줍니다.  
        _serial.ConfigurePort(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT);
        // Timeout 설정입니다. 별다른거 없으면 전부 zero 설정해도 되구요.  
        _serial.SetCommunicationTimeouts(0, 0, 0, 0, 0);

        while (loopStatusPlatform)
        {
            //			auto start = chrono::high_resolution_clock::now();

            _serial.MyCommRead();
            _serial.MyCommWrite();

            //연결 상태 관리용
            dataContainer->updateValue_platform_status();
            //this_thread::sleep_for(100ms);

            //			auto end = chrono::high_resolution_clock::now();
            //			chrono::duration<double, std::milli> elapsed = end - start;
            //			cout << "Waited " << elapsed.count() << " ms\n";
        }
    }
    else
    {
        _serial.ClosePort();
    }
}
void SensorStatus::comLidar() {}
void SensorStatus::comCamera1() {}
void SensorStatus::comGps() {}

