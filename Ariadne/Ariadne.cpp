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
    } // combobox 관련: https://www.bogotobogo.com/Qt/Qt5_QComboBox.php

    QObject::connect(ui.Btn_Mission0, SIGNAL(clicked()), this, SLOT(clicked_btn_mission0()));

}

// comport를 열고 닫을때 CString이 이용되므로 QString을 CString으로 바꿔주는 함수를 만들었다.
CString Ariadne::ConvertQstringtoCString(QString qs)
{
    // https://stackoverflow.com/questions/4214369/how-to-convert-qstring-to-stdstring
    std::string utf8_text = qs.toUtf8().constData();
    std::cout << utf8_text << std::endl;
    CString cs(utf8_text.c_str());
    return cs;
}

void Ariadne::clicked_btn_mission0() {
    mission.doMission0();
    std::cout << "button이 클릭되었습니다." << std::endl;

    // for test
    QString Temp;
    Temp = ui.comboBox->currentText();
    std::wcout << (const wchar_t*)ConvertQstringtoCString(Temp) << std::endl;
}
