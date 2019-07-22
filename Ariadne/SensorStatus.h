#pragma once

#include "ComPlatform.h"
#include "atlstr.h"
#include "LidarFun.h"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QString>
#include <QTimer>
#include <iostream>
#include <iterator>
#include <QtCore>
#include <Thread>
#include <torch/script.h>
#include <torch/torch.h>
#include <torch/data/transforms/tensor.h>
#include <opencv2/imgproc/imgproc.hpp>


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
	//	cout << "�÷��� �����尡 �����Ǿ����ϴ�.\n";
	//	this->wait();
	//}


private:
	/// void run() Q_DECL_OVERRIDE; //thread ���� �� �ڵ� �����Ǵ� �Լ�

signals: /// thread�� ���� broadcast �Լ���
    void AorMChanged(int);
    void EStopChanged(int);
    void GearChanged(int);
    void SpeedChanged(int);
    void SteerChanged(int);
    void BreakChanged(int);
    void EncChanged(int);
    void AliveChanged(int);
	void PlatformExit();

public slots:

};

class LidarComThread : public QThread
{
	Q_OBJECT
protected:

public:
	DataContainer *dataContainer;

	int comLidar();
	void run();

private:

signals: /// thread�� ���� broadcast �Լ���
	void LidarExit();

public slots:
};


class ScnnThread : public QThread
{
    Q_OBJECT
protected:

public:
    DataContainer *dataContainer;
    void run();
    //int mainfun();
};


class ScnnThread : public QThread
{
    Q_OBJECT
protected:

public:
    DataContainer *dataContainer;
    void run();
    int mainfun();

private:

signals: /// thread�� ���� broadcast �Լ���


public slots:
};
