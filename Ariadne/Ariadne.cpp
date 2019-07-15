#include "Ariadne.h"
#include <iostream>
#include "atlstr.h"

// HyeAhn View�� ���� ������ ��

Ariadne::Ariadne(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    
    dataContainer = DataContainer::getInstance();
    platformComThread = new PlatformComThread;
    ///  -------------------  UI ������ �ֱ� �� �ٹ̱� ------------------------- ///

    for (int i = 0; i < 5; i++) // combobox�� ������ �ֱ�. �ش� ���������� ���� ��ſ� ����Ʈ�� ����.
    {
        ui.comboBox->addItem("COM" + QString::number(i));
        ui.comboBox_2->addItem("COM" + QString::number(i));
        ui.comboBox_3->addItem("COM" + QString::number(i));
        ui.comboBox_4->addItem("COM" + QString::number(i));
        ui.comboBox_5->addItem("COM" + QString::number(i));
    } // combobox ����: https://www.bogotobogo.com/Qt/Qt5_QComboBox.php

    /// --------------------- Qobject�� ��ư connect ---------------------------------///

    QObject::connect(ui.Btn_Mission0, SIGNAL(clicked()), this, SLOT(clicked_btn_mission0()));
    QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(clicked_btn_confirm()));

    /// ------------------- thread and signals for UI update ----------------------///
    connect(platformComThread, SIGNAL(AorMChanged(int)), this, SLOT(onAorMChanged(int)));
    connect(platformComThread, SIGNAL(EStopChanged(int)), this, SLOT(onEStopChanged(int)));
    connect(platformComThread, SIGNAL(GearChanged(int)), this, SLOT(onGearChanged(int)));
    connect(platformComThread, SIGNAL(SpeedChanged(int)), this, SLOT(onSpeedChanged(int)));
    connect(platformComThread, SIGNAL(SteerChanged(int)), this, SLOT(onSteerChanged(int)));
    connect(platformComThread, SIGNAL(BreakChanged(int)), this, SLOT(onBreakChanged(int)));
    connect(platformComThread, SIGNAL(EncChanged(int)), this, SLOT(onEncChanged(int)));
}

// comport�� ���� ������ CString�� �̿�ǹǷ� QString�� CString���� �ٲ��ִ� �Լ��� �������.
CString Ariadne::ConvertQstringtoCString(QString qs)
{

    /// h ttps://stackoverflow.com/questions/4214369/how-to-convert-qstring-to-stdstring
    std::string utf8_text = qs.toUtf8().constData();
    std::cout << utf8_text << std::endl;
    CString cs(utf8_text.c_str());
    return cs;
}

void Ariadne::clicked_btn_confirm() {

    // for test
    QString Temp;
    Temp = ui.comboBox->currentText();
    std::wcout << (const wchar_t*)ConvertQstringtoCString(Temp) << std::endl;


    QString Temp1, Temp2, Temp3, Temp4, Temp5;
    Temp1 = ui.comboBox->currentText();
    Temp2 = ui.comboBox_2->currentText();
    Temp3 = ui.comboBox_3->currentText();
    Temp4 = ui.comboBox_4->currentText();
    Temp5 = ui.comboBox_5->currentText();

    ui.plainTextEdit->appendPlainText("I love you");
    ui.plainTextEdit->appendPlainText("I love you very much");
    ui.plainTextEdit->appendPlainText("will you going to me together?");

    /// platformComThread->start();
    /// platformComThread->comPlatform(QStringtoCString(Temp1));*/
    
    // ������ �����ϱ�

    
    platformComThread->start();
}

void Ariadne::clicked_btn_mission0() {
    //mission.doMission0();
}


CString Ariadne::QStringtoCString(QString qs)
{
    /* https:  //stackoverflow.com/questions/4214369/how-to-convert-qstring-to-stdstring */
    std::string utf8_text = qs.toUtf8().constData();
    CString cs(utf8_text.c_str());

    std::cout << utf8_text << std::endl;

    return cs;
}


///  _    _ _____    _    _ _____  _____       _______ ______ 
/// | |  | |_   _|  | |  | |  __ \|  __ \   /\|__   __|  ____|
/// | |  | | | |    | |  | | |__) | |  | | /  \  | |  | |__   
/// | |  | | | |    | |  | |  ___/| |  | |/ /\ \ | |  |  __|  
/// | |__| |_| |_   | |__| | |    | |__| / ____ \| |  | |____ 
///  \____/|_____|   \____/|_|    |_____/_/    \_\_|  |______|
// this part is for UI updating functions; display, slots... etc

void Ariadne::onAorMChanged(int Number)
{
    ui.lcdNumber->display(Number);
}

void Ariadne::onEStopChanged(int Number)
{
    ui.lcdNumber_2->display(Number);
}

void Ariadne::onGearChanged(int Number)
{
    ui.lcdNumber_3->display(Number);
}

void Ariadne::onSpeedChanged(int Number)
{
    ui.lcdNumber_4->display(Number);
}

void Ariadne::onSteerChanged(int Number)
{
    ui.lcdNumber_5->display(Number);
}

void Ariadne::onBreakChanged(int Number)
{
    ui.lcdNumber_6->display(Number);
}

void Ariadne::onEncChanged(int Number)
{
    ui.lcdNumber_7->display(Number);
} 

/// TODO: GPS location display �߰�

void Ariadne::updateSensorStatus()
{
    using namespace std;
    // TODO: �ܼ�â ���� ui�� ����ϱ�
    //���� ����
    // �÷��� ���� ����

    DataContainer *dataContainer;
    dataContainer = DataContainer::getInstance();

    if (dataContainer->getValue_platform_status() > 5)
    {
        cout << "�÷����� ����Ǿ����ϴ�." << endl;
    }
    else if (dataContainer->getValue_platform_status() > 0)
    {
        cout << "�÷����� ����� �����ǰ� �ֽ��ϴ�." << endl;
    }
    else if (dataContainer->getValue_platform_status() == 0)
    {
        cout << "�÷����� ����� �����Ͽ����ϴ�." << endl;
    }
    dataContainer->setValue_platform_status(0);

    // LiDAR ���� ����
    if (dataContainer->getValue_lidar_status() > 5)
    {
        cout << "���̴ٿ� ����Ǿ����ϴ�." << endl;
    }

    else if (dataContainer->getValue_lidar_status() == 0)
    {
        cout << "���̴ٿ� ����� �����ǰ� �ֽ��ϴ�." << endl;
    }
    else if (dataContainer->getValue_lidar_status() < 0)
    {
        cout << "���̴ٿ� ����� �����Ͽ����ϴ�." << endl;
    }

    dataContainer->setValue_lidar_status(dataContainer->getValue_lidar_status() - 1);

    // CAMERA1 ���� ����
    if (dataContainer->getValue_camera1_status() > 5)
    {
        cout << "ī�޶�1�� ����Ǿ����ϴ�." << endl;
    }
    else if (dataContainer->getValue_camera1_status() > 0)
    {
        cout << "ī�޶�1�� ����� �����ǰ� �ֽ��ϴ�." << endl;
    }
    else if (dataContainer->getValue_camera1_status() == 0)
    {
        cout << "ī�޶�1�� ����� �����Ͽ����ϴ�." << endl;
    }
    dataContainer->setValue_camera1_status(0);


    // GPS �������
    if (dataContainer->getValue_gps_status() > 5)
    {
        cout << "GPS�� ����Ǿ����ϴ�." << endl;
    }
    else if (dataContainer->getValue_gps_status() > 0)
    {
        cout << "GPS�� ����� �����ǰ� �ֽ��ϴ�." << endl;
    }
    else if (dataContainer->getValue_gps_status() == 0)
    {
        cout << "GPS�� ����� �����Ͽ����ϴ�." << endl;
    }
    dataContainer->setValue_gps_status(0);
}