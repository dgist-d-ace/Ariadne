#include "SensorStatus.h"

#define UPDATE_PLATFORM_STATUS 100
#define UPDATE_SENSOR_CONNECTION 101
#define UPDATE_SENSOR_STATUS 102
#define UPDATE_SENSOR_AUTOSTARTUP 103

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
		cout << "�÷��� ����\n";
		//thread ���� �� �ٽ� �����ϱ� ���ؼ� slot�� thread�� �ٽ� �����ϴ� �Լ��� ������ �ʿ䰡 ����
		//connect(&platformcom_thread, SIGNAL(finished()), this, SLOT(quit()));
		platformcom_thread.start();
        break;
    case 2:
		cout << "���̴� ����\n";
		//connect(&lidarcom_thread, SIGNAL(finished()), this, SLOT(quit()));
		lidarcom_thread.start();
        break;
    case 3:
		cout << "ī�޶� ����\n";
		//connect(&camera1com_thread, SIGNAL(finished()), this, SLOT(quit()));
		camera1com_thread.start();
        break;
    case 4:
		cout << "gps ����\n";
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

    //���� ����
    // �÷��� ���� ����
    if (dataContainer->getValue_platform_status() > 5)
    {
        cout << "�÷����� ����Ǿ����ϴ�." << endl;
    }
    else if (dataContainer->getValue_platform_status() > 0)
    {
        cout << "�÷����� ����� �����ǰ� �ֽ��ϴ�." << endl;
    }
    else if (dataContainer->getValue_platform_status() == 0)
    {
        cout << "�÷����� ����� �����Ͽ����ϴ�." << endl;
    }
    dataContainer->setValue_platform_status(0);

    // LiDAR ���� ����
    if (dataContainer->getValue_lidar_status() > 5)
    {
        cout << "���̴ٿ� ����Ǿ����ϴ�." << endl;
    }

    else if (dataContainer->getValue_lidar_status() == 0)
    {
        cout << "���̴ٿ� ����� �����ǰ� �ֽ��ϴ�." << endl;
    }
    else if (dataContainer->getValue_lidar_status() < 0)
    {
        cout << "���̴ٿ� ����� �����Ͽ����ϴ�." << endl;
    }

    dataContainer->setValue_lidar_status(dataContainer->getValue_lidar_status() - 1);

    // CAMERA1 ���� ����
    if (dataContainer->getValue_camera1_status() > 5)
    {
        cout << "ī�޶�1�� ����Ǿ����ϴ�." << endl;
    }
    else if (dataContainer->getValue_camera1_status() > 0)
    {
        cout << "ī�޶�1�� ����� �����ǰ� �ֽ��ϴ�." << endl;
    }
    else if (dataContainer->getValue_camera1_status() == 0)
    {
        cout << "ī�޶�1�� ����� �����Ͽ����ϴ�." << endl;
    }
    dataContainer->setValue_camera1_status(0);


    // GPS �������
    if (dataContainer->getValue_gps_status() > 5)
    {
        cout << "GPS�� ����Ǿ����ϴ�." << endl;
    }
    else if (dataContainer->getValue_gps_status() > 0)
    {
        cout << "GPS�� ����� �����ǰ� �ֽ��ϴ�." << endl;
    }
    else if (dataContainer->getValue_gps_status() == 0)
    {
        cout << "GPS�� ����� �����Ͽ����ϴ�." << endl;
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
        // ���� ��� ����
            // ���� ��� ������ ����
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

void MissionThread::run() {
	//���� �ʿ�
	cout << "�̼� �����尡 �����Ǿ����ϴ�.\n";
	exec();
}

void PlatformComThread::run() {
	//���� �ʿ�
	cout << "�÷��� �����尡 �����Ǿ����ϴ�.\n";
	exec();
}

void LidarComThread::run() {
	//���� �ʿ�
	cout << "���̴� �����尡 �����Ǿ����ϴ�.\n";
	exec();
}

void Camera1ComThread::run() {
	//���� �ʿ�
	cout << "ī�޶�1 �����尡 �����Ǿ����ϴ�.\n";
	exec();
}

void Camera2ComThread::run() {
	//���� �ʿ�
	cout << "ī�޶�2 �����尡 �����Ǿ����ϴ�.\n";
	exec();
}

void RTKComThread::run() {
	//���� �ʿ�
	cout << "GPS �����尡 �����Ǿ����ϴ�.\n";
	exec();
	
}



