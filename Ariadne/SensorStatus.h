#include "DataContainer.h"
#include "ComPlatform.h"
#include "CSerialPort.h"
#include "Ariadne.h"


#include <thread>
#include <QTimer>
#include <iostream>
#include <thread>
#include <vector>
#include <iterator>
#include <math.h>

using namespace std;

class SensorStatus
{
public:
    Ariadne *MainWindow;


private:
    SensorStatus();

    DataContainer *dataContainer;

    thread automode_thread;
    thread platformcom_thread;
    thread lidarcom_thread;
    thread camera1com_thread;
    thread gpscom_thread;

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