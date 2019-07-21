#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_Ariadne.h"
#include <QtWidgets/QPushButton>
#include <QTimer>
#include "SensorStatus.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
// CString 사용하려고 함
#include "atlstr.h"
#include <QString>
#include <QTimer>


class RTKComThread : public QThread
{
	Q_OBJECT
protected:

public:
	DataContainer *dataContainer;
	Ui::AriadneClass * ui;

	RTKComThread();

	bool loopStatusPlatform = true;
	void comRTK();
	void run();

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
	/*
int f_mode = 1;
int f_Estop = 0;
int f_gear = 0;
int f_speed = 20;
int f_steer = 0;  //steer 반대로나옴
int f_brake = 0;
*/
	void setWritePram(BYTE* writeBuffer);
	/// void run() Q_DECL_OVERRIDE; //thread 생성 후 자동 실행되는 함수

signals: /// thread가 보낼 broadcast 함수들
	//void AorMChanged(int);
	//void EStopChanged(int);
	//void GearChanged(int);
	//void SpeedChanged(int);
	//void SteerChanged(int);
	//void BreakChanged(int);
	//void EncChanged(int);
	//void AliveChanged(int);

public slots:

};


class Ariadne : public QMainWindow
{
    Q_OBJECT

public:
    Ariadne(QWidget *parent = Q_NULLPTR);

    PlatformComThread *platformComThread;
    LidarComThread *lidarComThread;
	RTKComThread *rtkComThread;

    CString QStringtoCString(QString);
    DataContainer *dataContainer;

	QTimer* TimerSensorStatus;

	static Ui::AriadneClass* getUI();
	

private:
	static Ui::AriadneClass* ui;
    void updateSensorStatus();
    void keyPressEvent(QKeyEvent *);

public slots:
    void clicked_btn_mission0();
    CString ConvertQstringtoCString(QString);
    void clicked_btn_confirm();
    void clicked_btn_up();
    void gear_input();

    void onAorMChanged(int);
    void onEStopChanged(int);
    void onGearChanged(int);
    void onSpeedChanged(int);
    void onBreakChanged(int);
    void onSteerChanged(int);
    void onEncChanged(int);


};

