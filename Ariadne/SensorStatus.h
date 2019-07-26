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

using namespace std;

//class PlatformCom : public QObject
//{
//    Q_OBJECT
//protected:
//
//public:
//    DataContainer *dataContainer;
//    bool loopStatusPlatform = true;
//    PlatformCom();
//    ///void run();
//
//    //PlatformComThread() {
//    //	//dataContainer = DataContainer::getInstance();
//    //}
//    //~PlatformComThread() {
//    //	cout << "�÷��� �����尡 �����Ǿ����ϴ�.\n";
//    //	this->wait();
//    //
//    //
//
//private:
//
//    ComPlatform _platform;
//    /// void run() Q_DECL_OVERRIDE; //thread ���� �� �ڵ� �����Ǵ� �Լ�
//
//signals: /// thread�� ���� broadcast �Լ���
//    void AorMChanged(int);
//    void EStopChanged(int);
//    void GearChanged(int);
//    void SpeedChanged(int);
//    void SteerChanged(int);
//    void BreakChanged(int);
//    void EncChanged(int);
//    void AliveChanged(int);
//    void PlatformExit();
//
//public slots:
//    void comPlatform();
//
//};

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

    Scnn();

private:

signals: /// thread�� ���� broadcast �Լ���


public slots:
    void comScnn();
};