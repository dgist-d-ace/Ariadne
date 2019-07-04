#include "SensorStatus.h"

#define UPDATE_PLATFORM_STATUS 100
#define UPDATE_SENSOR_CONNECTION 101
#define UPDATE_SENSOR_STATUS 102
#define UPDATE_SENSOR_AUTOSTARTUP 103

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
    // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
        platformcom_thread = thread([&]() { comPlatform(); });
        break;
    case 2:
        lidarcom_thread = thread([&]() { comLidar(); });
        break;
    case 3:
        camera1com_thread = thread([&]() { comCamera1(); });
        break;
    case 4:
        gpscom_thread = thread([&]() { comGps(); });
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

    //���� ����
    // �÷��� ���� ����
    if (dataContainer->getValue_platform_status() > 5)
    {
        cout << "�÷��� ����" << endl;
    }
    else if (dataContainer->getValue_platform_status() > 0)
    {
        cout << "�÷������� ���� ����" << endl;
    }
    else if (dataContainer->getValue_platform_status() == 0)
    {
        cout << "�÷������� ��� �ߴ�" << endl;
    }
    dataContainer->setValue_platform_status(0);

    // LiDAR ���� ����
    if (dataContainer->getValue_lidar_status() > 0)
    {
        cout << "Lidar is on" << endl;
    }

    else if (dataContainer->getValue_lidar_status() == 0)
    {
        cout << "���̴ٿ� ��� ����" << endl;
    }
    else if (dataContainer->getValue_lidar_status() < 0)
    {
        cout << "���̴ٿ� ��� �ߴ�" << endl;
    }

    dataContainer->setValue_lidar_status(dataContainer->getValue_lidar_status() - 1);

    // CAMERA1 ���� ����
    if (dataContainer->getValue_camera1_status() > 5)
    {
        cout << "camera ����" << endl;
    }
    else if (dataContainer->getValue_camera1_status() > 0)
    {
        cout << "camera ���� ����" << endl;
    }
    else if (dataContainer->getValue_camera1_status() == 0)
    {
        cout << "camera ��� �ߴ�" << endl;
    }
    dataContainer->setValue_camera1_status(0);


    // GPS �������
    if (dataContainer->getValue_gps_status() > 5)
    {
        cout << "GPS ����" << endl;
    }
    else if (dataContainer->getValue_gps_status() > 0)
    {
        cout << "GPS ���� ����" << endl;
    }
    else if (dataContainer->getValue_gps_status() == 0)
    {
        cout << "GPS ��� �ߴ�" << endl;
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
        // ���� ���� Ÿ�̸� ����
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
        // ���� ��� ����
            // ���� ��� ������ ����
        if (automode_thread.joinable())
        {
            loopStatusAutomode = true;
        }
        else
        {
            automode_thread = thread([&]() { /*automode();*/ });
        }

        sensorAutoCount = -1;
        //KillTimer(hwnd, UPDATE_SENSOR_AUTOSTARTUP);
        delete TimerAutostartup;
        break;
    }
    sensorAutoCount++;
}
void SensorStatus::showPlatformControlValue() {
    //�����ؾ���
}
void SensorStatus::comPlatform()
{
    //�÷��� ��� �ڵ��Դϴ�.

    CString comPort;

    if (_serial.OpenPort(comPort))   // ���� ���� COM Port �� �־���մϴ�.  
    {
        // BaudRate, ByteSize, fParity, Parity, StopBit ������ �������ݴϴ�.  
        _serial.ConfigurePort(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT);
        // Timeout �����Դϴ�. ���ٸ��� ������ ���� zero �����ص� �Ǳ���.  
        _serial.SetCommunicationTimeouts(0, 0, 0, 0, 0);

        while (loopStatusPlatform)
        {
            //			auto start = chrono::high_resolution_clock::now();

            _serial.MyCommRead();
            _serial.MyCommWrite();

            //���� ���� ������
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