#include "CSerialPort.h"
#include "ComPlatform.h"
#include "DataContainer.h"
#include "Ariadne.h"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QString>
#include <QTimer>
#include <iostream>
#include <vector>
#include <iterator>
#include <math.h>

using namespace std;

<<<<<<< Updated upstream

=======
>>>>>>> Stashed changes
class PlatformComThread : public QThread
{
	Q_OBJECT

protected:
	ComPlatform _serial;

public:
	bool loopStatusPlatform = true;
<<<<<<< Updated upstream
	void comPlatform(CString);
=======
    void comPlatform(CString);
    void run(CString);

>>>>>>> Stashed changes
	PlatformComThread() {
		dataContainer = DataContainer::getInstance();
	}
	~PlatformComThread() {
		cout << "플랫폼 스레드가 종료되었습니다.\n";
		this->wait();
	}

signals: /// thread가 보낼 broadcast 함수들
    void AorMChanged(int);
    void EStopChanged(int);
    void GearChanged(int);
    void SpeedChanged(int);
    void SteerChanged(int);
    void BreakChanged(int);
    void EncChanged(int);
    void AliveChanged(int);

public slots:

private:
    DataContainer *dataContainer;
	void run() Q_DECL_OVERRIDE;//thread 생성 후 자동 실행되는 함수
};

class LidarComThread : public QThread
{
	Q_OBJECT
protected:
public:
	LidarComThread() {
		dataContainer = DataContainer::getInstance();
	}
	~LidarComThread() {
		cout << "라이다 스레드가 종료되었습니다.\n";
		this->wait();
	}
private:
	DataContainer *dataContainer;
	void run() Q_DECL_OVERRIDE;//thread 생성 후 자동 실행되는 함수
};

class Camera1ComThread : public QThread
{
	Q_OBJECT
public:
	void comCamera1();
	Camera1ComThread() {
		dataContainer = DataContainer::getInstance();
	}
	~Camera1ComThread() {
		cout << "카메라1 스레드가 종료되었습니다.\n";
		this->wait();
	}

private:
	DataContainer *dataContainer;
	void run() Q_DECL_OVERRIDE;//thread 생성 후 자동 실행되는 함수
};

class Camera2ComThread : public QThread
{
	Q_OBJECT
public:

	void comCamera2();
	Camera2ComThread() {
		dataContainer = DataContainer::getInstance();
	}
	~Camera2ComThread() {
		cout << "카메라2 스레드가 종료되었습니다.\n";
		this->wait();
	}
private:
	DataContainer *dataContainer;
	void run() Q_DECL_OVERRIDE; //thread 생성 후 자동 실행되는 함수
};

class RTKComThread : public QThread
{
	Q_OBJECT
public:
	RTKComThread() {
		dataContainer = DataContainer::getInstance();
	}
	~RTKComThread() {
		cout << "GPS 스레드가 종료되었습니다.\n";
		this->wait();
	}
private:
	DataContainer *dataContainer;
	void run() Q_DECL_OVERRIDE; //thread 생성 후 자동 실행되는 함수
};

class MissionThread : public QThread
{
	Q_OBJECT
public:
	MissionThread() {
		dataContainer = DataContainer::getInstance();
	}
	~MissionThread() {
		cout << "미션 스레드가 종료되었습니다.\n";
		this->wait();
	}
private:
	DataContainer *dataContainer;
	void run() Q_DECL_OVERRIDE; //thread 생성 후 자동 실행되는 함수
};

class SensorStatus : public QObject
{
	Q_OBJECT
public:
	SensorStatus();

private:
	DataContainer *dataContainer;

    MissionThread mission_thread;
    PlatformComThread platformcom_thread;
    LidarComThread lidarcom_thread;
    Camera1ComThread camera1com_thread;
    Camera2ComThread camera2com_thread;
    RTKComThread gpscom_thread;

    int sensorCount = 0;
    int sensorAutoCount = 0;
    ComPlatform _serial;

    bool loopStatusAutomode = true;
    bool loopStatusPlatform = true;
    bool loopStatusLidar = true;
    bool loopStatusCamera1 = true;
    bool loopStatusGps = true;

    void updateSensorConnection();
    void updateSensorStatus();
    void updateSensorAutostartup();
    void showPlatformControlValue();

    void comPlatform();
    void comLidar();
    void comCamera1();
    void comGps();

    QTimer* TimerAutostartup;
    QTimer* TimerSensorConnection;
    QTimer* TimerSensorStatus;

public slots:
	void SlotUpdatePlatformStatus();

	void SlotUpdateSensorConnection();

	void SlotUpdateSensorStatus();

	void SlotUpdateSensorAutostartup();

};