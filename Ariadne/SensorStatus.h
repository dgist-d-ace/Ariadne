#pragma once

#include "ComPlatform.h"
#include "atlstr.h"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QString>
#include <QTimer>
#include <iostream>
#include <iterator>
#include <QtCore>

using namespace std;

class PlatformComThread : public QThread
{
    Q_OBJECT
protected:

public:
    DataContainer *dataContainer;
	bool loopStatusPlatform = true;
	void comPlatform();
    void run();

	//PlatformComThread() {
	//	//dataContainer = DataContainer::getInstance();
	//}
	//~PlatformComThread() {
	//	cout << "�÷��� �����尡 ����Ǿ����ϴ�.\n";
	//	this->wait();
	//}


private:
	/// void run() Q_DECL_OVERRIDE; //thread ���� �� �ڵ� ����Ǵ� �Լ�

signals: /// thread�� ���� broadcast �Լ���
    void AorMChanged(int);
    void EStopChanged(int);
    void GearChanged(int);
    void SpeedChanged(int);
    void SteerChanged(int);
    void BreakChanged(int);
    void EncChanged(int);
    void AliveChanged(int);

public slots:

};

/*
class LidarComThread : public QThread
{
	Q_OBJECT
protected:
public:
	LidarComThread() {
		dataContainer = DataContainer::getInstance();
	}
	~LidarComThread() {
		cout << "���̴� �����尡 ����Ǿ����ϴ�.\n";
		this->wait();
	}
private:

	DataContainer *dataContainer;
	void run() Q_DECL_OVERRIDE;//thread ���� �� �ڵ� ����Ǵ� �Լ�
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
		cout << "ī�޶�1 �����尡 ����Ǿ����ϴ�.\n";
		this->wait();
	}
private:

	DataContainer *dataContainer;
	void run() Q_DECL_OVERRIDE;//thread ���� �� �ڵ� ����Ǵ� �Լ�
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
		cout << "ī�޶�2 �����尡 ����Ǿ����ϴ�.\n";
		this->wait();
	}
private:


	DataContainer *dataContainer;
	void run() Q_DECL_OVERRIDE; //thread ���� �� �ڵ� ����Ǵ� �Լ�
};

class RTKComThread : public QThread
{
	Q_OBJECT
public:
	RTKComThread() {
		dataContainer = DataContainer::getInstance();
	}
	~RTKComThread() {
		cout << "GPS �����尡 ����Ǿ����ϴ�.\n";
		this->wait();
	}
private:

	DataContainer *dataContainer;
	void run() Q_DECL_OVERRIDE; //thread ���� �� �ڵ� ����Ǵ� �Լ�
};

class MissionThread : public QThread
{
	Q_OBJECT
public:
	MissionThread() {
		dataContainer = DataContainer::getInstance();
	}
	~MissionThread() {
		cout << "�̼� �����尡 ����Ǿ����ϴ�.\n";
		this->wait();
	}
private:
	DataContainer *dataContainer;
	void run() Q_DECL_OVERRIDE; //thread ���� �� �ڵ� ����Ǵ� �Լ�
};
*/

/*
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
*/