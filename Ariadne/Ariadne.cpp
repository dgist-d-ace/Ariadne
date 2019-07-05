#include "Ariadne.h"
#include "MissionAct.h"
#include <iostream>
#include "atlstr.h"

// HyeAhn View와 같은 역할을 함

Mission mission;

Ariadne::Ariadne(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    for (int i = 0; i < 5; i++) // combobox에 선택지 넣기. 해당 선택지들이 이후 통신용 컴포트에 사용됨.
    {
        ui.comboBox->addItem("COM" + QString::number(i));
        ui.comboBox_2->addItem("COM" + QString::number(i));
        ui.comboBox_3->addItem("COM" + QString::number(i));
        ui.comboBox_4->addItem("COM" + QString::number(i));
        ui.comboBox_5->addItem("COM" + QString::number(i));
    } // combobox 관련: https://www.bogotobogo.com/Qt/Qt5_QComboBox.php

    QObject::connect(ui.Btn_Mission0, SIGNAL(clicked()), this, SLOT(clicked_btn_mission0()));
    QObject::connect(ui.Btn_confirm, SIGNAL(clicked()), this, SLOT(clicked_btn_confirm()));
}


void Ariadne::clicked_btn_confirm()
{
    QString Temp1, Temp2, Temp3, Temp4, Temp5;
    Temp1 = ui.comboBox->currentText();
    Temp2 = ui.comboBox_2->currentText();
    Temp3 = ui.comboBox_3->currentText();
    Temp4 = ui.comboBox_4->currentText();
    Temp5 = ui.comboBox_5->currentText();

    //std::wcout << (const wchar_t*)ConvertQstringtoCString(Temp1) << " " << (const wchar_t*)ConvertQstringtoCString(Temp2)
    //    << " " << (const wchar_t*)ConvertQstringtoCString(Temp3) << " " << (const wchar_t*)ConvertQstringtoCString(Temp4) 
    //    << " " << (const wchar_t*)ConvertQstringtoCString(Temp5) << std::endl;

    ui.plainTextEdit->setPlainText("I love you");
    ui.plainTextEdit->appendPlainText("\n I love you very much");
    // 왜 버튼을 클릭할때마다 실행되는게 아닌거지?

    
}

void Ariadne::clicked_btn_mission0() {
    mission.doMission0();
}
