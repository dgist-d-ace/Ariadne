/********************************************************************************
** Form generated from reading UI file 'Ariadne.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARIADNE_H
#define UI_ARIADNE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_AriadneClass
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QComboBox *comboBox;
    QComboBox *comboBox_4;
    QPushButton *pushButton_2;
    QLabel *label_9;
    QLabel *label_12;
    QLabel *label_13;
    QPushButton *pushButton_3;
    QLabel *label_10;
    QLabel *label_11;
    QPushButton *Btn_lidar_restart;
    QPushButton *Btn_lidar_stop;
    QPushButton *Btn_yolo_stop;
    QPushButton *Btn_yolo_restart;
    QPushButton *Btn_scnn_stop;
    QPushButton *pushButton_9;
    QGroupBox *groupBox_2;
    QGroupBox *groupBox_3;
    QPushButton *Btn_Mission0;
    QPushButton *Btn_mission1;
    QPushButton *Btn_mission2;
    QPushButton *Btn_mission3;
    QPushButton *Btn_mission6;
    QPushButton *Btn_mission7;
    QPushButton *Btn_mission8;
    QPushButton *Btn_mission4;
    QPushButton *Btn_mission9;
    QPushButton *Btn_mission5;
    QPlainTextEdit *plainTextEdit;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *Btn_up;
    QPushButton *Btn_down;
    QPushButton *Btn_right;
    QPushButton *Btn_left;
    QComboBox *comboBox_6;
    QPushButton *Btn_gearInput;
    QWidget *tab_2;
    QTextBrowser *textBrowser;
    QPushButton *pushButton;
    QGroupBox *groupBox_4;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label;
    QLabel *label_8;
    QLCDNumber *lcdNumber;
    QLCDNumber *lcdNumber_2;
    QLCDNumber *lcdNumber_3;
    QLCDNumber *lcdNumber_4;
    QLCDNumber *lcdNumber_5;
    QLCDNumber *lcdNumber_6;
    QLCDNumber *lcdNumber_7;
    QLabel *label_5;
    QLabel *label_14;
    QLCDNumber *lcdNumber_8;
    QLCDNumber *lcdNumber_9;
    QCustomPlot *rt_plot;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLabel *label_15;
    QLabel *label_16;
    QPushButton *Btn_Go;
    QLabel *label_17;
    QLineEdit *lineEdit_3;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *AriadneClass)
    {
        if (AriadneClass->objectName().isEmpty())
            AriadneClass->setObjectName(QString::fromUtf8("AriadneClass"));
        AriadneClass->resize(1198, 859);
        centralWidget = new QWidget(AriadneClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 180, 241, 211));
        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(120, 20, 101, 21));
        comboBox_4 = new QComboBox(groupBox);
        comboBox_4->setObjectName(QString::fromUtf8("comboBox_4"));
        comboBox_4->setGeometry(QRect(120, 50, 101, 22));
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(10, 170, 101, 28));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(10, 20, 64, 15));
        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(10, 50, 64, 15));
        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 80, 64, 15));
        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(120, 170, 101, 28));
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 110, 64, 15));
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 140, 64, 15));
        Btn_lidar_restart = new QPushButton(groupBox);
        Btn_lidar_restart->setObjectName(QString::fromUtf8("Btn_lidar_restart"));
        Btn_lidar_restart->setGeometry(QRect(150, 80, 71, 23));
        Btn_lidar_stop = new QPushButton(groupBox);
        Btn_lidar_stop->setObjectName(QString::fromUtf8("Btn_lidar_stop"));
        Btn_lidar_stop->setGeometry(QRect(70, 80, 71, 23));
        Btn_yolo_stop = new QPushButton(groupBox);
        Btn_yolo_stop->setObjectName(QString::fromUtf8("Btn_yolo_stop"));
        Btn_yolo_stop->setGeometry(QRect(70, 110, 71, 23));
        Btn_yolo_restart = new QPushButton(groupBox);
        Btn_yolo_restart->setObjectName(QString::fromUtf8("Btn_yolo_restart"));
        Btn_yolo_restart->setGeometry(QRect(150, 110, 71, 23));
        Btn_scnn_stop = new QPushButton(groupBox);
        Btn_scnn_stop->setObjectName(QString::fromUtf8("Btn_scnn_stop"));
        Btn_scnn_stop->setGeometry(QRect(70, 140, 71, 23));
        pushButton_9 = new QPushButton(groupBox);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        pushButton_9->setGeometry(QRect(150, 140, 71, 23));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 241, 171));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 390, 241, 241));
        Btn_Mission0 = new QPushButton(groupBox_3);
        Btn_Mission0->setObjectName(QString::fromUtf8("Btn_Mission0"));
        Btn_Mission0->setGeometry(QRect(10, 40, 101, 31));
        Btn_mission1 = new QPushButton(groupBox_3);
        Btn_mission1->setObjectName(QString::fromUtf8("Btn_mission1"));
        Btn_mission1->setGeometry(QRect(10, 80, 101, 31));
        Btn_mission2 = new QPushButton(groupBox_3);
        Btn_mission2->setObjectName(QString::fromUtf8("Btn_mission2"));
        Btn_mission2->setGeometry(QRect(10, 120, 101, 31));
        Btn_mission3 = new QPushButton(groupBox_3);
        Btn_mission3->setObjectName(QString::fromUtf8("Btn_mission3"));
        Btn_mission3->setGeometry(QRect(10, 160, 101, 31));
        Btn_mission6 = new QPushButton(groupBox_3);
        Btn_mission6->setObjectName(QString::fromUtf8("Btn_mission6"));
        Btn_mission6->setGeometry(QRect(130, 80, 101, 31));
        Btn_mission7 = new QPushButton(groupBox_3);
        Btn_mission7->setObjectName(QString::fromUtf8("Btn_mission7"));
        Btn_mission7->setGeometry(QRect(130, 120, 101, 31));
        Btn_mission8 = new QPushButton(groupBox_3);
        Btn_mission8->setObjectName(QString::fromUtf8("Btn_mission8"));
        Btn_mission8->setGeometry(QRect(130, 160, 101, 31));
        Btn_mission4 = new QPushButton(groupBox_3);
        Btn_mission4->setObjectName(QString::fromUtf8("Btn_mission4"));
        Btn_mission4->setGeometry(QRect(10, 200, 101, 31));
        Btn_mission9 = new QPushButton(groupBox_3);
        Btn_mission9->setObjectName(QString::fromUtf8("Btn_mission9"));
        Btn_mission9->setGeometry(QRect(130, 200, 101, 31));
        Btn_mission5 = new QPushButton(groupBox_3);
        Btn_mission5->setObjectName(QString::fromUtf8("Btn_mission5"));
        Btn_mission5->setGeometry(QRect(130, 40, 101, 31));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 680, 1171, 101));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(970, 80, 211, 191));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        Btn_up = new QPushButton(tab);
        Btn_up->setObjectName(QString::fromUtf8("Btn_up"));
        Btn_up->setGeometry(QRect(80, 10, 51, 51));
        Btn_down = new QPushButton(tab);
        Btn_down->setObjectName(QString::fromUtf8("Btn_down"));
        Btn_down->setGeometry(QRect(80, 60, 51, 51));
        Btn_right = new QPushButton(tab);
        Btn_right->setObjectName(QString::fromUtf8("Btn_right"));
        Btn_right->setGeometry(QRect(140, 60, 51, 51));
        Btn_left = new QPushButton(tab);
        Btn_left->setObjectName(QString::fromUtf8("Btn_left"));
        Btn_left->setGeometry(QRect(20, 60, 51, 51));
        comboBox_6 = new QComboBox(tab);
        comboBox_6->setObjectName(QString::fromUtf8("comboBox_6"));
        comboBox_6->setGeometry(QRect(20, 120, 94, 22));
        Btn_gearInput = new QPushButton(tab);
        Btn_gearInput->setObjectName(QString::fromUtf8("Btn_gearInput"));
        Btn_gearInput->setGeometry(QRect(120, 120, 71, 28));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        textBrowser = new QTextBrowser(tab_2);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(10, 20, 191, 101));
        tabWidget->addTab(tab_2, QString());
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(970, 10, 211, 61));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(970, 280, 211, 351));
        label_2 = new QLabel(groupBox_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 150, 64, 15));
        label_3 = new QLabel(groupBox_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 90, 64, 15));
        label_4 = new QLabel(groupBox_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 210, 101, 16));
        label_6 = new QLabel(groupBox_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 30, 64, 21));
        label_7 = new QLabel(groupBox_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 60, 64, 16));
        label = new QLabel(groupBox_4);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 180, 64, 15));
        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(20, 120, 64, 15));
        lcdNumber = new QLCDNumber(groupBox_4);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(130, 30, 64, 23));
        lcdNumber_2 = new QLCDNumber(groupBox_4);
        lcdNumber_2->setObjectName(QString::fromUtf8("lcdNumber_2"));
        lcdNumber_2->setGeometry(QRect(130, 60, 64, 23));
        lcdNumber_3 = new QLCDNumber(groupBox_4);
        lcdNumber_3->setObjectName(QString::fromUtf8("lcdNumber_3"));
        lcdNumber_3->setGeometry(QRect(130, 90, 64, 23));
        lcdNumber_4 = new QLCDNumber(groupBox_4);
        lcdNumber_4->setObjectName(QString::fromUtf8("lcdNumber_4"));
        lcdNumber_4->setGeometry(QRect(130, 120, 64, 23));
        lcdNumber_5 = new QLCDNumber(groupBox_4);
        lcdNumber_5->setObjectName(QString::fromUtf8("lcdNumber_5"));
        lcdNumber_5->setGeometry(QRect(130, 150, 64, 23));
        lcdNumber_6 = new QLCDNumber(groupBox_4);
        lcdNumber_6->setObjectName(QString::fromUtf8("lcdNumber_6"));
        lcdNumber_6->setGeometry(QRect(130, 180, 64, 23));
        lcdNumber_7 = new QLCDNumber(groupBox_4);
        lcdNumber_7->setObjectName(QString::fromUtf8("lcdNumber_7"));
        lcdNumber_7->setGeometry(QRect(130, 210, 64, 23));
        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 240, 101, 16));
        label_14 = new QLabel(groupBox_4);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(20, 300, 101, 16));
        lcdNumber_8 = new QLCDNumber(groupBox_4);
        lcdNumber_8->setObjectName(QString::fromUtf8("lcdNumber_8"));
        lcdNumber_8->setGeometry(QRect(23, 260, 171, 23));
        lcdNumber_9 = new QLCDNumber(groupBox_4);
        lcdNumber_9->setObjectName(QString::fromUtf8("lcdNumber_9"));
        lcdNumber_9->setGeometry(QRect(20, 320, 171, 23));
        rt_plot = new QCustomPlot(centralWidget);
        rt_plot->setObjectName(QString::fromUtf8("rt_plot"));
        rt_plot->setGeometry(QRect(269, 19, 691, 611));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(180, 640, 113, 21));
        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(400, 640, 113, 21));
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(20, 640, 151, 16));
        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(310, 640, 81, 16));
        Btn_Go = new QPushButton(centralWidget);
        Btn_Go->setObjectName(QString::fromUtf8("Btn_Go"));
        Btn_Go->setGeometry(QRect(790, 640, 93, 28));
        label_17 = new QLabel(centralWidget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(530, 640, 121, 16));
        lineEdit_3 = new QLineEdit(centralWidget);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(650, 640, 113, 21));
        AriadneClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(AriadneClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1198, 21));
        AriadneClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(AriadneClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        AriadneClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(AriadneClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        AriadneClass->setStatusBar(statusBar);

        retranslateUi(AriadneClass);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(AriadneClass);
    } // setupUi

    void retranslateUi(QMainWindow *AriadneClass)
    {
        AriadneClass->setWindowTitle(QApplication::translate("AriadneClass", "Ariadne", nullptr));
        groupBox->setTitle(QApplication::translate("AriadneClass", "GroupBox", nullptr));
        pushButton_2->setText(QApplication::translate("AriadneClass", "\354\204\274\354\204\234 \355\206\265\354\213\240", nullptr));
        label_9->setText(QApplication::translate("AriadneClass", "Platform", nullptr));
        label_12->setText(QApplication::translate("AriadneClass", "GPS", nullptr));
        label_13->setText(QApplication::translate("AriadneClass", "LiDAR", nullptr));
        pushButton_3->setText(QApplication::translate("AriadneClass", "\354\243\274\355\226\211 \354\213\234\354\236\221", nullptr));
        label_10->setText(QApplication::translate("AriadneClass", "YOLO", nullptr));
        label_11->setText(QApplication::translate("AriadneClass", "SCNN", nullptr));
        Btn_lidar_restart->setText(QApplication::translate("AriadneClass", "RESTART", nullptr));
        Btn_lidar_stop->setText(QApplication::translate("AriadneClass", "STOP", nullptr));
        Btn_yolo_stop->setText(QApplication::translate("AriadneClass", "STOP", nullptr));
        Btn_yolo_restart->setText(QApplication::translate("AriadneClass", "RESTART", nullptr));
        Btn_scnn_stop->setText(QApplication::translate("AriadneClass", "STOP", nullptr));
        pushButton_9->setText(QApplication::translate("AriadneClass", "RESTART", nullptr));
        groupBox_2->setTitle(QApplication::translate("AriadneClass", "GroupBox", nullptr));
        groupBox_3->setTitle(QApplication::translate("AriadneClass", "Mission", nullptr));
        Btn_Mission0->setText(QApplication::translate("AriadneClass", "Mission0", nullptr));
        Btn_mission1->setText(QApplication::translate("AriadneClass", "Mission1", nullptr));
        Btn_mission2->setText(QApplication::translate("AriadneClass", "Mission2", nullptr));
        Btn_mission3->setText(QApplication::translate("AriadneClass", "Mission3", nullptr));
        Btn_mission6->setText(QApplication::translate("AriadneClass", "Mission6", nullptr));
        Btn_mission7->setText(QApplication::translate("AriadneClass", "Mission7", nullptr));
        Btn_mission8->setText(QApplication::translate("AriadneClass", "Mission8", nullptr));
        Btn_mission4->setText(QApplication::translate("AriadneClass", "Mission4", nullptr));
        Btn_mission9->setText(QApplication::translate("AriadneClass", "Mission9", nullptr));
        Btn_mission5->setText(QApplication::translate("AriadneClass", "Mission5", nullptr));
        Btn_up->setText(QApplication::translate("AriadneClass", "\342\226\262", nullptr));
        Btn_down->setText(QApplication::translate("AriadneClass", "\342\226\274", nullptr));
        Btn_right->setText(QApplication::translate("AriadneClass", "\342\226\266", nullptr));
        Btn_left->setText(QApplication::translate("AriadneClass", "\342\227\200", nullptr));
        Btn_gearInput->setText(QApplication::translate("AriadneClass", "gear input", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("AriadneClass", "PC Con", nullptr));
        textBrowser->setHtml(QApplication::translate("AriadneClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Gulim'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\354\241\260\354\242\205\352\270\260\353\241\234 \354\241\260\354\242\205\355\225\230\353\212\224 \353\252\250\353\223\234\353\213\271!</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\354\227\254\352\270\260 \353\255\220 \353\204\243\354\235\204\354\247\200 \353\252\250\353\245\264\352\262\240\353\213\271!!</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\352\267\270\353\246\254\352\263\240 \354\227\254\352"
                        "\270\260 \354\225\204\354\247\201 \355\225\250\354\210\230 \354\225\210\353\247\214\353\223\244\354\227\210\353\213\271!!!</p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("AriadneClass", "remoteCon", nullptr));
        pushButton->setText(QApplication::translate("AriadneClass", "E-Stop", nullptr));
        groupBox_4->setTitle(QApplication::translate("AriadneClass", "SensorStatus", nullptr));
        label_2->setText(QApplication::translate("AriadneClass", "STEER", nullptr));
        label_3->setText(QApplication::translate("AriadneClass", "GEAR", nullptr));
        label_4->setText(QApplication::translate("AriadneClass", "Encoder", nullptr));
        label_6->setText(QApplication::translate("AriadneClass", "AorM", nullptr));
        label_7->setText(QApplication::translate("AriadneClass", "E-Stop", nullptr));
        label->setText(QApplication::translate("AriadneClass", "BREAK", nullptr));
        label_8->setText(QApplication::translate("AriadneClass", "SPEED", nullptr));
        label_5->setText(QApplication::translate("AriadneClass", "Latitude", nullptr));
        label_14->setText(QApplication::translate("AriadneClass", "Longitude", nullptr));
        label_15->setText(QApplication::translate("AriadneClass", "Destination lattitude(m)", nullptr));
        label_16->setText(QApplication::translate("AriadneClass", "longitude", nullptr));
        Btn_Go->setText(QApplication::translate("AriadneClass", "GO", nullptr));
        label_17->setText(QApplication::translate("AriadneClass", "heading(degree)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AriadneClass: public Ui_AriadneClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARIADNE_H
