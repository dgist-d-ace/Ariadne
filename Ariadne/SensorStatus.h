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
#include <QThread>
#include <sstream>

using namespace std;

class LidarCom : public QObject
{
	Q_OBJECT
protected:

public:
	DataContainer *dataContainer;

	LidarCom();
private:

signals: /// thread�� ���� broadcast �Լ���
	void LidarExit();

public slots:
	int comLidar();
};


class Scnn : public QObject
{
	Q_OBJECT
protected:

public:
	DataContainer *dataContainer;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE tid;

	SOCKET server;
	SOCKET client;

	Scnn();
private:

signals: /// thread�� ���� broadcast �Լ���
	void drivingEnabled();

public slots:
	void comScnn();
	void SuspendScnn();
	void ResumeScnn();
	int boostScnn(); /// for boost test
};

class Yolo : public QObject
{
	Q_OBJECT
protected:

public:
	DataContainer *dataContainer;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE tid;

	HANDLE hNamePipe;
	clock_t bustCon = 0;
	SOCKET server;
	SOCKET client;
	Yolo();
	virtual ~Yolo();
	bool Yolo_first = true;

private:

signals:
	void BustExist(bool);
	void KidsafeExist(bool);
	void yoloStarted();

public slots:
	void comYolo();
	void SuspendYolo();
	void ResumeYolo();
};

class View : public QObject
{
	Q_OBJECT
protected:

public:
	DataContainer *dataContainer;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE tid;
	SOCKET server;
	SOCKET client;
	View();
	~View();
private:

public slots:
	void comView(int id);
	void SuspendView();
	void ResumeView();

};

int ConnectClient(HANDLE hNamePipe);