#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_Ariadne.h"
#include <QtWidgets/QPushButton>
#include <QTimer>
#include "SensorStatus.h"


// CString 사용하려고 함
#include "atlstr.h"
#include <QString>


class Ariadne : public QMainWindow
{
    Q_OBJECT

public:
    Ariadne(QWidget *parent = Q_NULLPTR);
    PlatformComThread *platformComThread;
    //LidarComThread *lidarComThread;
    CString QStringtoCString(QString);
    DataContainer *dataContainer;

private:
    Ui::AriadneClass ui;
    void updateSensorStatus();

public slots:
    void clicked_btn_mission0();
    CString ConvertQstringtoCString(QString);
    void clicked_btn_confirm();


    void onAorMChanged(int);
    void onEStopChanged(int);
    void onGearChanged(int);
    void onSpeedChanged(int);
    void onBreakChanged(int);
    void onSteerChanged(int);
    void onEncChanged(int);
};
