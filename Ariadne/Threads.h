#pragma once

// 현재 수정중 0704

# include <QThread>
# include <QMutex>
# include <iostream>
# include "ComPlatform.h"
# include "DataContainer.h"

QMutex mutex;
DataContainer* dataContainer;

class PlatformComThread : public QThread
{
protected:
    ComPlatform _serial;

public:
    bool loopStatusPlatform = true;
    void comPlatform(CString);
};

class LidarComThread : public QThread
{
protected:
    
};

class Camera1ComThread : public QThread
{
public:
    void comCamera1();
};

class Camera2ComThread : public QThread
{
public:
    void comCamera2();
};

class RTKComThread : public QThread
{

};

class MissionThread : public QThread
{

};