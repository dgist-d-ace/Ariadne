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
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
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
    QPushButton *Btn_scnn_restart;
    QGroupBox *groupBox_3;
    QPushButton *Btn_Mission1;
    QPushButton *Btn_mission2;
    QPushButton *Btn_mission3;
    QPushButton *Btn_mission4;
    QPushButton *Btn_mission8;
    QPushButton *Btn_mission6;
    QPushButton *Btn_mission5;
    QPushButton *Btn_mission7;
    QPushButton *Btn_kidsafe;
    QPushButton *Btn_Bust;
    QLabel *label_20;
    QLabel *label_21;
    QPushButton *Btn_mission9;
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
    QFrame *line;
    QLabel *label_15;
    QLCDNumber *lcdNumber_10;
    QFrame *line_2;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QLCDNumber *lcdNumber_11;
    QLCDNumber *lcdNumber_12;
    QLCDNumber *lcdNumber_13;
    QLCDNumber *lcdNumber_14;
    QCustomPlot *rt_plot;
    QGroupBox *groupBox_5;
    QLabel *AriadneLogo;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *AriadneClass)
    {
        if (AriadneClass->objectName().isEmpty())
            AriadneClass->setObjectName(QString::fromUtf8("AriadneClass"));
        AriadneClass->resize(1179, 859);
        centralWidget = new QWidget(AriadneClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 100, 241, 211));
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
        label_13->setGeometry(QRect(10, 80, 51, 16));
        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(120, 170, 101, 28));
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 110, 51, 16));
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 140, 51, 16));
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
        Btn_scnn_restart = new QPushButton(groupBox);
        Btn_scnn_restart->setObjectName(QString::fromUtf8("Btn_scnn_restart"));
        Btn_scnn_restart->setGeometry(QRect(150, 140, 71, 23));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 310, 241, 301));
        Btn_Mission1 = new QPushButton(groupBox_3);
        Btn_Mission1->setObjectName(QString::fromUtf8("Btn_Mission1"));
        Btn_Mission1->setGeometry(QRect(10, 20, 101, 31));
        Btn_mission2 = new QPushButton(groupBox_3);
        Btn_mission2->setObjectName(QString::fromUtf8("Btn_mission2"));
        Btn_mission2->setGeometry(QRect(10, 60, 101, 31));
        Btn_mission3 = new QPushButton(groupBox_3);
        Btn_mission3->setObjectName(QString::fromUtf8("Btn_mission3"));
        Btn_mission3->setGeometry(QRect(10, 100, 101, 31));
        Btn_mission4 = new QPushButton(groupBox_3);
        Btn_mission4->setObjectName(QString::fromUtf8("Btn_mission4"));
        Btn_mission4->setGeometry(QRect(10, 140, 101, 31));
        Btn_mission8 = new QPushButton(groupBox_3);
        Btn_mission8->setObjectName(QString::fromUtf8("Btn_mission8"));
        Btn_mission8->setGeometry(QRect(130, 140, 101, 31));
        Btn_mission6 = new QPushButton(groupBox_3);
        Btn_mission6->setObjectName(QString::fromUtf8("Btn_mission6"));
        Btn_mission6->setGeometry(QRect(130, 60, 101, 31));
        Btn_mission5 = new QPushButton(groupBox_3);
        Btn_mission5->setObjectName(QString::fromUtf8("Btn_mission5"));
        Btn_mission5->setGeometry(QRect(130, 20, 101, 31));
        Btn_mission7 = new QPushButton(groupBox_3);
        Btn_mission7->setObjectName(QString::fromUtf8("Btn_mission7"));
        Btn_mission7->setGeometry(QRect(130, 100, 101, 31));
        Btn_kidsafe = new QPushButton(groupBox_3);
        Btn_kidsafe->setObjectName(QString::fromUtf8("Btn_kidsafe"));
        Btn_kidsafe->setGeometry(QRect(130, 220, 101, 31));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../../DYibre/Checkbox_Off.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8("../../DYibre/Checkbox_On.png"), QSize(), QIcon::Normal, QIcon::On);
        Btn_kidsafe->setIcon(icon);
        Btn_kidsafe->setIconSize(QSize(100, 40));
        Btn_kidsafe->setCheckable(true);
        Btn_Bust = new QPushButton(groupBox_3);
        Btn_Bust->setObjectName(QString::fromUtf8("Btn_Bust"));
        Btn_Bust->setGeometry(QRect(130, 260, 101, 31));
        Btn_Bust->setIcon(icon);
        Btn_Bust->setIconSize(QSize(100, 40));
        Btn_Bust->setCheckable(true);
        label_20 = new QLabel(groupBox_3);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(30, 230, 56, 12));
        label_21 = new QLabel(groupBox_3);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(40, 270, 56, 12));
        Btn_mission9 = new QPushButton(groupBox_3);
        Btn_mission9->setObjectName(QString::fromUtf8("Btn_mission9"));
        Btn_mission9->setGeometry(QRect(130, 180, 101, 31));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(270, 680, 691, 101));
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
        tabWidget->addTab(tab_2, QString());
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(970, 10, 211, 61));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(970, 280, 211, 501));
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
        lcdNumber->setFrameShadow(QFrame::Plain);
        lcdNumber->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_2 = new QLCDNumber(groupBox_4);
        lcdNumber_2->setObjectName(QString::fromUtf8("lcdNumber_2"));
        lcdNumber_2->setGeometry(QRect(130, 60, 64, 23));
        lcdNumber_2->setFrameShadow(QFrame::Plain);
        lcdNumber_2->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_3 = new QLCDNumber(groupBox_4);
        lcdNumber_3->setObjectName(QString::fromUtf8("lcdNumber_3"));
        lcdNumber_3->setGeometry(QRect(130, 90, 64, 23));
        lcdNumber_3->setFrameShadow(QFrame::Plain);
        lcdNumber_3->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_4 = new QLCDNumber(groupBox_4);
        lcdNumber_4->setObjectName(QString::fromUtf8("lcdNumber_4"));
        lcdNumber_4->setGeometry(QRect(130, 120, 64, 23));
        lcdNumber_4->setFrameShadow(QFrame::Plain);
        lcdNumber_4->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_5 = new QLCDNumber(groupBox_4);
        lcdNumber_5->setObjectName(QString::fromUtf8("lcdNumber_5"));
        lcdNumber_5->setGeometry(QRect(130, 150, 64, 23));
        lcdNumber_5->setFrameShadow(QFrame::Plain);
        lcdNumber_5->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_6 = new QLCDNumber(groupBox_4);
        lcdNumber_6->setObjectName(QString::fromUtf8("lcdNumber_6"));
        lcdNumber_6->setGeometry(QRect(130, 180, 64, 23));
        lcdNumber_6->setFrameShape(QFrame::Box);
        lcdNumber_6->setFrameShadow(QFrame::Plain);
        lcdNumber_6->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_7 = new QLCDNumber(groupBox_4);
        lcdNumber_7->setObjectName(QString::fromUtf8("lcdNumber_7"));
        lcdNumber_7->setGeometry(QRect(130, 210, 64, 23));
        lcdNumber_7->setFrameShadow(QFrame::Plain);
        lcdNumber_7->setSegmentStyle(QLCDNumber::Flat);
        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 270, 101, 16));
        label_14 = new QLabel(groupBox_4);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(20, 300, 101, 16));
        lcdNumber_8 = new QLCDNumber(groupBox_4);
        lcdNumber_8->setObjectName(QString::fromUtf8("lcdNumber_8"));
        lcdNumber_8->setGeometry(QRect(130, 260, 61, 23));
        lcdNumber_8->setFrameShadow(QFrame::Plain);
        lcdNumber_8->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_9 = new QLCDNumber(groupBox_4);
        lcdNumber_9->setObjectName(QString::fromUtf8("lcdNumber_9"));
        lcdNumber_9->setGeometry(QRect(130, 290, 61, 23));
        lcdNumber_9->setFrameShadow(QFrame::Plain);
        lcdNumber_9->setSegmentStyle(QLCDNumber::Flat);
        line = new QFrame(groupBox_4);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(20, 240, 171, 16));
        line->setFrameShadow(QFrame::Raised);
        line->setFrameShape(QFrame::HLine);
        label_15 = new QLabel(groupBox_4);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(20, 330, 101, 16));
        lcdNumber_10 = new QLCDNumber(groupBox_4);
        lcdNumber_10->setObjectName(QString::fromUtf8("lcdNumber_10"));
        lcdNumber_10->setGeometry(QRect(130, 320, 61, 23));
        lcdNumber_10->setFrameShadow(QFrame::Plain);
        lcdNumber_10->setSegmentStyle(QLCDNumber::Flat);
        line_2 = new QFrame(groupBox_4);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(20, 350, 171, 16));
        line_2->setFrameShadow(QFrame::Raised);
        line_2->setFrameShape(QFrame::HLine);
        label_16 = new QLabel(groupBox_4);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(20, 370, 101, 16));
        label_17 = new QLabel(groupBox_4);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(20, 400, 101, 16));
        label_18 = new QLabel(groupBox_4);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(20, 430, 101, 16));
        label_19 = new QLabel(groupBox_4);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(20, 460, 101, 16));
        lcdNumber_11 = new QLCDNumber(groupBox_4);
        lcdNumber_11->setObjectName(QString::fromUtf8("lcdNumber_11"));
        lcdNumber_11->setGeometry(QRect(130, 370, 61, 23));
        lcdNumber_11->setFrameShadow(QFrame::Plain);
        lcdNumber_11->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_12 = new QLCDNumber(groupBox_4);
        lcdNumber_12->setObjectName(QString::fromUtf8("lcdNumber_12"));
        lcdNumber_12->setGeometry(QRect(130, 400, 61, 23));
        lcdNumber_12->setFrameShadow(QFrame::Plain);
        lcdNumber_12->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_13 = new QLCDNumber(groupBox_4);
        lcdNumber_13->setObjectName(QString::fromUtf8("lcdNumber_13"));
        lcdNumber_13->setGeometry(QRect(130, 430, 61, 23));
        lcdNumber_13->setFrameShadow(QFrame::Plain);
        lcdNumber_13->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_14 = new QLCDNumber(groupBox_4);
        lcdNumber_14->setObjectName(QString::fromUtf8("lcdNumber_14"));
        lcdNumber_14->setGeometry(QRect(130, 460, 61, 23));
        lcdNumber_14->setFrameShadow(QFrame::Plain);
        lcdNumber_14->setMode(QLCDNumber::Dec);
        lcdNumber_14->setSegmentStyle(QLCDNumber::Flat);
        rt_plot = new QCustomPlot(centralWidget);
        rt_plot->setObjectName(QString::fromUtf8("rt_plot"));
        rt_plot->setGeometry(QRect(269, 19, 691, 651));
        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 620, 241, 161));
        AriadneLogo = new QLabel(centralWidget);
        AriadneLogo->setObjectName(QString::fromUtf8("AriadneLogo"));
        AriadneLogo->setGeometry(QRect(30, 20, 241, 71));
        AriadneClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(AriadneClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1179, 21));
        AriadneClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(AriadneClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        AriadneClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(AriadneClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        AriadneClass->setStatusBar(statusBar);

        retranslateUi(AriadneClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AriadneClass);
    } // setupUi

    void retranslateUi(QMainWindow *AriadneClass)
    {
        AriadneClass->setWindowTitle(QApplication::translate("AriadneClass", "Ariadne", nullptr));
        groupBox->setTitle(QApplication::translate("AriadneClass", "Sensor Connect Status", nullptr));
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
        Btn_scnn_restart->setText(QApplication::translate("AriadneClass", "RESTART", nullptr));
        groupBox_3->setTitle(QApplication::translate("AriadneClass", "Mission", nullptr));
        Btn_Mission1->setText(QApplication::translate("AriadneClass", "1. Parking", nullptr));
        Btn_mission2->setText(QApplication::translate("AriadneClass", "2. InterReady", nullptr));
        Btn_mission3->setText(QApplication::translate("AriadneClass", "3. InterLeft", nullptr));
        Btn_mission4->setText(QApplication::translate("AriadneClass", "4. InterRight", nullptr));
        Btn_mission8->setText(QApplication::translate("AriadneClass", "8. DynamicOb", nullptr));
        Btn_mission6->setText(QApplication::translate("AriadneClass", "6. InterStop", nullptr));
        Btn_mission5->setText(QApplication::translate("AriadneClass", "5. InterStraight", nullptr));
        Btn_mission7->setText(QApplication::translate("AriadneClass", "7. StaticOb", nullptr));
        Btn_kidsafe->setText(QString());
        Btn_Bust->setText(QString());
        label_20->setText(QApplication::translate("AriadneClass", "KidSafe", nullptr));
        label_21->setText(QApplication::translate("AriadneClass", "Bust", nullptr));
        Btn_mission9->setText(QApplication::translate("AriadneClass", "9. Basic", nullptr));
        Btn_up->setText(QApplication::translate("AriadneClass", "\342\226\262", nullptr));
        Btn_down->setText(QApplication::translate("AriadneClass", "\342\226\274", nullptr));
        Btn_right->setText(QApplication::translate("AriadneClass", "\342\226\266", nullptr));
        Btn_left->setText(QApplication::translate("AriadneClass", "\342\227\200", nullptr));
        Btn_gearInput->setText(QApplication::translate("AriadneClass", "gear input", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("AriadneClass", "PC Con", nullptr));
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
        label_15->setText(QApplication::translate("AriadneClass", "Heading", nullptr));
        label_16->setText(QApplication::translate("AriadneClass", "LeftLeftLine", nullptr));
        label_17->setText(QApplication::translate("AriadneClass", "LeftLine", nullptr));
        label_18->setText(QApplication::translate("AriadneClass", "RightLine", nullptr));
        label_19->setText(QApplication::translate("AriadneClass", "RightRightLine", nullptr));
        groupBox_5->setTitle(QApplication::translate("AriadneClass", "YOLO", nullptr));
        AriadneLogo->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AriadneClass: public Ui_AriadneClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARIADNE_H
