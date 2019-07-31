#pragma once

#include "ComPlatform.h"
#include "atlstr.h"
#include "LidarFunc.h"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QString>
#include <QTimer>
#include <iostream>
#include <iterator>
#include <QtCore>

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
    void comLidar();
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

private:
public slots:

	void comYolo();
};

int ConnectClient(HANDLE hNamePipe);