#include "Ariadne.h"
#include <iostream>
#include "atlstr.h"

// HyeAhn View와 같은 역할을 함

Ariadne::Ariadne(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    
    dataContainer = DataContainer::getInstance();
    platformComThread = new PlatformComThread;
    ///  -------------------  UI 선택지 넣기 및 꾸미기 ------------------------- ///

    for (int i = 0; i < 5; i++) // combobox에 선택지 넣기. 해당 선택지들이 이후 통신용 컴포트에 사용됨.
    {
        ui.comboBox->addItem("COM" + QString::number(i));
        ui.comboBox_2->addItem("COM" + QString::number(i));
        ui.comboBox_3->addItem("COM" + QString::number(i));
        ui.comboBox_4->addItem("COM" + QString::number(i));
        ui.comboBox_5->addItem("COM" + QString::number(i));
    } // combobox 관련: https://www.bogotobogo.com/Qt/Qt5_QComboBox.php

    /// --------------------- Qobject와 버튼 connect ---------------------------------///

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

// comport를 열고 닫을때 CString이 이용되므로 QString을 CString으로 바꿔주는 함수를 만들었다.
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
    
    // 쓰레드 생성하기

    
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

/// TODO: GPS location display 추가

void Ariadne::updateSensorStatus()
{
    using namespace std;
    // TODO: 콘솔창 말고 ui에 출력하기
    //센서 정보
    // 플랫폼 연결 상태

    DataContainer *dataContainer;
    dataContainer = DataContainer::getInstance();

    if (dataContainer->getValue_platform_status() > 5)
    {
        cout << "플랫폼과 연결되었습니다." << endl;
    }
    else if (dataContainer->getValue_platform_status() > 0)
    {
        cout << "플랫폼과 통신이 지연되고 있습니다." << endl;
    }
    else if (dataContainer->getValue_platform_status() == 0)
    {
        cout << "플랫폼과 통신이 실패하였습니다." << endl;
    }
    dataContainer->setValue_platform_status(0);

    // LiDAR 연결 상태
    if (dataContainer->getValue_lidar_status() > 5)
    {
        cout << "라이다와 연결되었습니다." << endl;
    }

    else if (dataContainer->getValue_lidar_status() == 0)
    {
        cout << "라이다와 통신이 지연되고 있습니다." << endl;
    }
    else if (dataContainer->getValue_lidar_status() < 0)
    {
        cout << "라이다와 통신이 실패하였습니다." << endl;
    }

    dataContainer->setValue_lidar_status(dataContainer->getValue_lidar_status() - 1);

    // CAMERA1 연결 상태
    if (dataContainer->getValue_camera1_status() > 5)
    {
        cout << "카메라1과 연결되었습니다." << endl;
    }
    else if (dataContainer->getValue_camera1_status() > 0)
    {
        cout << "카메라1과 통신이 지연되고 있습니다." << endl;
    }
    else if (dataContainer->getValue_camera1_status() == 0)
    {
        cout << "카메라1과 통신이 실패하였습니다." << endl;
    }
    dataContainer->setValue_camera1_status(0);


    // GPS 연결상태
    if (dataContainer->getValue_gps_status() > 5)
    {
        cout << "GPS와 연결되었습니다." << endl;
    }
    else if (dataContainer->getValue_gps_status() > 0)
    {
        cout << "GPS와 통신이 지연되고 있습니다." << endl;
    }
    else if (dataContainer->getValue_gps_status() == 0)
    {
        cout << "GPS와 통신이 실패하였습니다." << endl;
    }
    dataContainer->setValue_gps_status(0);
}