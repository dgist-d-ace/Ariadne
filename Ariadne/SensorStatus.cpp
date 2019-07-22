#include "SensorStatus.h"

#include <QMutex>
#include <iostream>


#define UPDATE_PLATFORM_STATUS 100
#define UPDATE_SENSOR_CONNECTION 101
#define UPDATE_SENSOR_STATUS 102
#define UPDATE_SENSOR_AUTOSTARTUP 103

void PlatformComThread::comPlatform() // 추후 인자로 CString이 들어갈 것
{
    dataContainer = DataContainer::getInstance();
    //플랫폼 통신 코드입니다.
    CString comPort;
    ComPlatform _serial;

    std::cout << "플랫폼 커뮤니케이션 시작\n";

    if (_serial.OpenPort(L"COM1"))   // 실제 사용될 COM Port 를 넣어야합니다.  
    {
        _serial.ConfigurePort(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT);
        _serial.SetCommunicationTimeouts(0, 0, 0, 0, 0);

        while (loopStatusPlatform)
        {
			if (_serial.MyCommRead()) {}
			else { break; }
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
    comPlatform();
	emit(PlatformExit());
}

void LidarComThread::comLidar() {


}

void LidarComThread::run() {
	cout << "라이다 스레드가 생성되었습니다.\n";
	comLidar();
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
