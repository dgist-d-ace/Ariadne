#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Ariadne.h"
#include <QtWidgets/QPushButton>
#include <QTimer>
#include "DataContainer.h"

// CString 사용하려고 함
#include "atlstr.h"
#include <QString>


class Ariadne : public QMainWindow
{
    Q_OBJECT

public:
    Ariadne(QWidget *parent = Q_NULLPTR);

private:
    Ui::AriadneClass ui;
    // BK: DataContainer *dataContainer;

public slots:
    void clicked_btn_mission0();
    CString ConvertQstringtoCString(QString);

};
