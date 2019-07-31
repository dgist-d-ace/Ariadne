#pragma once
#include "SensorStatus.h"
#include "MissionAct.h"

#include <QtWidgets/QMainWindow>
#include "ui_Ariadne.h"
#include <QtWidgets/QPushButton>
#include <QTimer>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "atlstr.h"
#include <QString>
#include <QTimer>

class GPSCom : public QObject
{
    Q_OBJECT
protected:

public:
    DataContainer *dataContainer;
    Ui::AriadneClass *ui;

    GPSCom();
    bool loopStatusPlatform = true;

private:
    bool Geofence;
    QString GF;
    QVector<double> x, y;
    double heading;
    double _lat, _lng;
    double lat, lng;
    vector<vector<double>> map_link;

    void Paint_base();
    void Paint_school(); 

signals:
    void GPSExit();
    void latitudeChanged(double);
    void longitudeChanged(double);

public slots:
    void comGPS();

};

class PlatformCom : public QObject
{
    Q_OBJECT
protected:
    Ui::AriadneClass *ui;
    /// Because of including header file, 
    /// GPS Thread and Platform Com Thread should be in the Ariadne.h
    /// These two classes use UI component.

public:
    DataContainer *dataContainer;
    bool loopStatusPlatform = true;
    PlatformCom();
    //void run();

private:
    ComPlatform _platform;

signals:
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
    void comPlatform();

};



class Ariadne : public QMainWindow
{
    Q_OBJECT

public:
    Ariadne(QWidget *parent = Q_NULLPTR);

    PlatformCom* platformCom;
 //   LidarCom*lidarCom;
    GPSCom* gpsCom;
 //   Scnn* scnn;
	Yolo* yolo;

    QThread* platformThread;
 //   QThread* lidarThread;
    QThread* gpsThread;
 //   QThread* scnnThread;
	QThread* yoloThread;

	Driving* driving;
	QThread* drivingThread;

    DataContainer* dataContainer;

    QTimer* TimerSensorStatus;
    static Ui::AriadneClass* getUI();

    

private:
    static Ui::AriadneClass* ui;
    void updateSensorStatus();
    void keyPressEvent(QKeyEvent *);

public slots:
    void clicked_btn_mission0();
    void clicked_btn_sensor();
	void clicked_btn_driving();

    void clicked_speed_up();
    void clicked_speed_down();
    void clicked_steer_left();
    void clicked_steer_right();
    void gear_input();
    void clicked_E_stop();

    void gotoGPSdestination();

    void onAorMChanged(int);
    void onEStopChanged(int);
    void onGearChanged(int);
    void onSpeedChanged(int);
    void onBreakChanged(int);
    void onSteerChanged(int);
    void onEncChanged(int);

    void onLatitudeChanged(double);
    void onLongitudeChanged(double);

};

CString ConvertQstringtoCString(QString); 
// this function is used in GPSCom Class and PlatformCom Class

double rad2deg(double);
double deg2rad(double);