#pragma once
#pragma comment(lib, "setupapi.lib")
#include "SensorStatus.h"
#include "MissionAct.h"

#include <QtWidgets/QMainWindow>
#include "ui_Ariadne.h"
#include <QtWidgets/QPushButton>
#include <QTimer>
#include <QCloseEvent>

#include <iostream>
#include <fstream>
#include <string>
#include <initguid.h>
#include <objbase.h>
#include <devguid.h>
#include <setupapi.h>
#include <cfgmgr32.h>   
#include "atlstr.h"
#include <QString>
#include <QTimer>
#include <QSystemTrayIcon>

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
	void latitudeChanged(int);
	void longitudeChanged(int);
	void headingChanged(double);

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
	//LidarComThread *lidarComThread;
	LidarCom* lidarCom;
	GPSCom* gpsCom;
	Scnn* scnn;
	Yolo* yolo;
	View* view;

	QThread* platformThread;
	QThread* lidarThread;
	QThread* gpsThread;
	QThread* scnnThread;
	QThread* yoloThread;

	QTimer* TimerUIUpdate;

	Driving* driving;
	QThread* drivingThread;

	DataContainer* dataContainer;

	QTimer* TimerSensorStatus;

	static Ui::AriadneClass* getUI();

	bool parkingOn = 0;
	
private:
	static Ui::AriadneClass* ui;
	void updateSensorStatus();
	void keyPressEvent(QKeyEvent *);
	void closeEvent(QCloseEvent *event);
	void reset();

public slots:
	void clicked_btn_mission1();
	void clicked_btn_mission2();
	void clicked_btn_mission3();
	void clicked_btn_mission4();
	void clicked_btn_mission5();
	void clicked_btn_mission6();
	void clicked_btn_mission7();
	void clicked_btn_mission8();
	void clicked_btn_mission9();
	void clicked_btn_missionExit();
	void clicked_btn_bust(bool);
	void clicked_btn_kidsafe(bool);
	void clicked_btn_shift();

	void clicked_btn_sensor();
	void clicked_btn_driving_mid();
	void clicked_btn_driving_fin();

	void clicked_speed_up();
	void clicked_speed_down();
	void clicked_steer_left();
	void clicked_steer_right();
	void gear_input();
	void clicked_E_stop();
	void clicked_auto();
	void clicked_manual();

	void onAorMChanged(int);
	void onEStopChanged(int);
	void onGearChanged(int);
	void onSpeedChanged(int);
	void onBreakChanged(int);
	void onSteerChanged(int);
	void onEncChanged(int);

	void onLatitudeChanged(int);
	void onLongitudeChanged(int);
	void onHeadingChanged(double);
	void onCurrentMission(int);
	void onExitMission(int);
	void onDrivingEnabled();
	void onBustExist(bool);
	void onKidsafeExist(bool);

	void AutoPortFinder();

	void updateUI();

	void scnnStart();

};

CString ConvertQstringtoCString(QString);
// this function is used in GPSCom Class and PlatformCom Class

double rad2deg(double);
double deg2rad(double);