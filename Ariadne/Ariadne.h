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

#include "atlstr.h"
#include <QString>
#include <QTimer>


class RTKCom : public QObject
{
    Q_OBJECT
protected:

public:
    DataContainer *dataContainer;
    Ui::AriadneClass *ui;

    RTKCom();

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

    /*
int f_mode = 1;
int f_Estop = 0;
int f_gear = 0;
int f_speed = 20;
int f_steer = 0;  //steer �ݴ��γ���
int f_brake = 0;
*/

    // void setWritePram(BYTE* writeBuffer);
    /// void run() Q_DECL_OVERRIDE; //thread ���� �� �ڵ� �����Ǵ� �Լ�

signals: /// thread�� ���� broadcast �Լ���
    void RTKExit();

public slots:
    void comRTK();

};

class PlatformCom : public QObject
{
    Q_OBJECT
protected:
    Ui::AriadneClass *ui;

public:
    DataContainer *dataContainer;
    bool loopStatusPlatform = true;
    PlatformCom();
    ///void run();

    //PlatformComThread() {
    //	//dataContainer = DataContainer::getInstance();
    //}
    //~PlatformComThread() {
    //	cout << "�÷��� �����尡 �����Ǿ����ϴ�.\n";
    //	this->wait();
    //
    //

private:

    ComPlatform _platform;
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
    void comPlatform();

};

class Ariadne : public QMainWindow
{
    Q_OBJECT

public:
    Ariadne(QWidget *parent = Q_NULLPTR);

    PlatformCom *platformCom;
    //LidarCom *lidarCom;
    //RTKCom *rtkCom;
    //Scnn *scnn;

    QThread* platformThread;
    /*QThread* lidarThread;
    QThread* rtkThread;
    QThread* scnnThread;*/

    DataContainer *dataContainer;

    QTimer* TimerSensorStatus;

    static Ui::AriadneClass* getUI();

private:
    static Ui::AriadneClass* ui;
    void updateSensorStatus();
    void keyPressEvent(QKeyEvent *);

public slots:
    void clicked_btn_mission0();
    
    void clicked_btn_confirm();
    void clicked_speed_up();
    void clicked_speed_down();
    void clicked_steer_left();
    void clicked_steer_right();
    void gear_input();
    void clicked_E_stop();

    void onAorMChanged(int);
    void onEStopChanged(int);
    void onGearChanged(int);
    void onSpeedChanged(int);
    void onBreakChanged(int);
    void onSteerChanged(int);
    void onEncChanged(int);

};

CString ConvertQstringtoCString(QString); 
/// this function is used in RTKCom Class and PlatformCom Class