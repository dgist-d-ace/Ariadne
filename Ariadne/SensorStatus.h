#include "CSerialPort.h"
#include "ComPlatform.h"
//#include "Ariadne.h"

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

class SensorStatus : public QObject
{
	Q_OBJECT
public:
    Ariadne *MainWindow;
	SensorStatus();

private:
    
	
    MissionThread mission_thread;
	PlatformComThread platformcom_thread;
    LidarComThread lidarcom_thread;
    Camera1ComThread camera1com_thread;
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

private slots:
    void updateSensor(UINT_PTR nIDEvent);

};