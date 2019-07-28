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

    Scnn();

private:

signals: /// thread�� ���� broadcast �Լ���


public slots:
    void comScnn();
};

class Yolo : public QObject
{
	Q_OBJECT
protected:

public:
	DataContainer *dataContainer;

private:
	Yolo();
	void comYolo();
};