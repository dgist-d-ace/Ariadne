#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QTimer>
#include <atlstr.h>
#include "ui_Ariadne.h"
#include "DataContainer.h"
#include "SensorStatus.h"

class Ariadne : public QMainWindow
{
    Q_OBJECT

public:
    Ariadne(QWidget *parent = Q_NULLPTR);
    PlatformComThread *platformComThread;
    DataContainer *dataContainer;
    //LidarComThread *lidarComThread;
    CString QStringtoCString(QString);

private:
    Ui::AriadneClass ui;

public slots:
    void onAorMChanged(int);
    void onEStopChanged(int);
    void onGearChanged(int);
    void onSpeedChanged(int);
    void onBreakChanged(int);
    void onSteerChanged(int);
    void onEncChanged(int);

private slots: /// 버튼 클릭 이벤트들
    void clicked_btn_mission0();
    CString ConvertQstringtoCString(QString);

};
