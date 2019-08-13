#pragma once

#include "ComPlatform.h"
#include "atlstr.h"
#include "Planner.hpp"

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

	SOCKET server;
	SOCKET client;
	Yolo();

	bool Yolo_Com = true;

private:
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

private:

public slots:
    void comView();
    void SuspendView();
    void ResumeView();

};

int ConnectClient(HANDLE hNamePipe);