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

QT_BEGIN_NAMESPACE

class Ui_AriadneClass
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QLabel *label_9;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *statusPlatform;
    QLabel *statusGps;
    QLabel *statusLidar;
    QLabel *statusYolo;
    QLabel *statusScnn;
    QGroupBox *groupBox_3;
    QPushButton *Btn_mission1;
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
    QPushButton *btn_mission_exit;
    QPlainTextEdit *plainTextEdit;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *Btn_up;
    QPushButton *Btn_down;
    QPushButton *Btn_right;
    QPushButton *Btn_left;
    QComboBox *comboBox_6;
    QPushButton *Btn_gearInput;
    QPushButton *Btn_auto;
    QPushButton *Btn_manual;
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
    QLabel *AriadneLogo;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QLabel *pathmap;
    QLabel *parking;
    QGroupBox *groupBox_2;
    QLCDNumber *lcdNumber_10;
    QLabel *label_14;
    QLabel *label_15;
    QLCDNumber *lcdNumber_8;
    QLCDNumber *lcdNumber_9;
    QLabel *label_5;
    QPlainTextEdit *longitudeShift;
    QPlainTextEdit *latitudeShift;
    QPushButton *shiftOK;
    QLabel *label_24;
    QLabel *label_35;
    QGroupBox *groupBox_5;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_31;
    QLabel *label_32;
    QLabel *label_33;
    QLabel *label_34;
    QGroupBox *groupBox_6;
    QLabel *label_30;
    QLabel *label_23;
    QLCDNumber *lcdNumber_19;
    QLabel *label_27;
    QLabel *label_22;
    QLCDNumber *lcdNumber_21;
    QLCDNumber *lcdNumber_16;
    QLCDNumber *lcdNumber_20;
    QLCDNumber *lcdNumber_22;
    QLabel *label_28;
    QLCDNumber *lcdNumber_18;
    QLabel *label_25;
    QLabel *label_26;
    QLabel *label_29;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *AriadneClass)
    {
        if (AriadneClass->objectName().isEmpty())
            AriadneClass->setObjectName(QString::fromUtf8("AriadneClass"));
        AriadneClass->setEnabled(true);
        AriadneClass->resize(1885, 1030);
        centralWidget = new QWidget(AriadneClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(30, 100, 151, 181));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        groupBox->setFont(font);
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(10, 30, 64, 15));
        label_9->setFont(font);
        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(10, 60, 64, 15));
        label_12->setFont(font);
        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 90, 51, 16));
        label_13->setFont(font);
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 120, 51, 16));
        label_10->setFont(font);
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 150, 51, 16));
        label_11->setFont(font);
        statusPlatform = new QLabel(groupBox);
        statusPlatform->setObjectName(QString::fromUtf8("statusPlatform"));
        statusPlatform->setGeometry(QRect(60, 30, 60, 15));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        statusPlatform->setFont(font1);
        statusPlatform->setStyleSheet(QString::fromUtf8(""));
        statusPlatform->setTextFormat(Qt::AutoText);
        statusGps = new QLabel(groupBox);
        statusGps->setObjectName(QString::fromUtf8("statusGps"));
        statusGps->setGeometry(QRect(60, 60, 60, 15));
        statusGps->setFont(font1);
        statusGps->setTextFormat(Qt::AutoText);
        statusLidar = new QLabel(groupBox);
        statusLidar->setObjectName(QString::fromUtf8("statusLidar"));
        statusLidar->setGeometry(QRect(60, 90, 60, 15));
        statusLidar->setFont(font1);
        statusLidar->setTextFormat(Qt::AutoText);
        statusYolo = new QLabel(groupBox);
        statusYolo->setObjectName(QString::fromUtf8("statusYolo"));
        statusYolo->setGeometry(QRect(60, 120, 60, 15));
        statusYolo->setFont(font1);
        statusYolo->setTextFormat(Qt::AutoText);
        statusScnn = new QLabel(groupBox);
        statusScnn->setObjectName(QString::fromUtf8("statusScnn"));
        statusScnn->setGeometry(QRect(60, 150, 60, 15));
        statusScnn->setFont(font1);
        statusScnn->setTextFormat(Qt::AutoText);
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setEnabled(true);
        groupBox_3->setGeometry(QRect(30, 300, 311, 331));
        Btn_mission1 = new QPushButton(groupBox_3);
        Btn_mission1->setObjectName(QString::fromUtf8("Btn_mission1"));
        Btn_mission1->setEnabled(true);
        Btn_mission1->setGeometry(QRect(20, 20, 120, 35));
        Btn_mission3 = new QPushButton(groupBox_3);
        Btn_mission3->setObjectName(QString::fromUtf8("Btn_mission3"));
        Btn_mission3->setGeometry(QRect(20, 60, 120, 35));
        Btn_mission4 = new QPushButton(groupBox_3);
        Btn_mission4->setObjectName(QString::fromUtf8("Btn_mission4"));
        Btn_mission4->setGeometry(QRect(20, 100, 120, 35));
        Btn_mission8 = new QPushButton(groupBox_3);
        Btn_mission8->setObjectName(QString::fromUtf8("Btn_mission8"));
        Btn_mission8->setGeometry(QRect(160, 100, 120, 35));
        Btn_mission6 = new QPushButton(groupBox_3);
        Btn_mission6->setObjectName(QString::fromUtf8("Btn_mission6"));
        Btn_mission6->setGeometry(QRect(160, 20, 120, 35));
        Btn_mission5 = new QPushButton(groupBox_3);
        Btn_mission5->setObjectName(QString::fromUtf8("Btn_mission5"));
        Btn_mission5->setGeometry(QRect(20, 140, 120, 35));
        Btn_mission7 = new QPushButton(groupBox_3);
        Btn_mission7->setObjectName(QString::fromUtf8("Btn_mission7"));
        Btn_mission7->setGeometry(QRect(160, 60, 120, 35));
        Btn_kidsafe = new QPushButton(groupBox_3);
        Btn_kidsafe->setObjectName(QString::fromUtf8("Btn_kidsafe"));
        Btn_kidsafe->setGeometry(QRect(160, 190, 120, 31));
        QIcon icon;
        icon.addFile(QString::fromUtf8("C:/Users/D-Ace/Documents/DYibre/Checkbox_Off.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8("C:/Users/D-Ace/Documents/DYibre/Checkbox_On.png"), QSize(), QIcon::Normal, QIcon::On);
        Btn_kidsafe->setIcon(icon);
        Btn_kidsafe->setIconSize(QSize(120, 55));
        Btn_kidsafe->setCheckable(true);
        Btn_Bust = new QPushButton(groupBox_3);
        Btn_Bust->setObjectName(QString::fromUtf8("Btn_Bust"));
        Btn_Bust->setEnabled(true);
        Btn_Bust->setGeometry(QRect(160, 230, 120, 31));
        QFont font2;
        font2.setKerning(true);
        Btn_Bust->setFont(font2);
        Btn_Bust->setAutoFillBackground(false);
        Btn_Bust->setIcon(icon);
        Btn_Bust->setIconSize(QSize(120, 55));
        Btn_Bust->setCheckable(true);
        Btn_Bust->setAutoDefault(false);
        Btn_Bust->setFlat(false);
        label_20 = new QLabel(groupBox_3);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(20, 190, 120, 30));
        label_20->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 204, 102)"));
        label_20->setAlignment(Qt::AlignCenter);
        label_21 = new QLabel(groupBox_3);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(20, 230, 120, 30));
        label_21->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 204, 102)"));
        label_21->setAlignment(Qt::AlignCenter);
        Btn_mission9 = new QPushButton(groupBox_3);
        Btn_mission9->setObjectName(QString::fromUtf8("Btn_mission9"));
        Btn_mission9->setGeometry(QRect(160, 140, 120, 35));
        btn_mission_exit = new QPushButton(groupBox_3);
        btn_mission_exit->setObjectName(QString::fromUtf8("btn_mission_exit"));
        btn_mission_exit->setEnabled(true);
        btn_mission_exit->setGeometry(QRect(20, 279, 261, 35));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(370, 719, 1191, 231));
        plainTextEdit->setMaximumSize(QSize(1380, 240));
        plainTextEdit->setCenterOnScroll(false);
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(30, 653, 300, 191));
        tabWidget->setLayoutDirection(Qt::LeftToRight);
        tabWidget->setUsesScrollButtons(false);
        tabWidget->setTabsClosable(false);
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
        comboBox_6->setGeometry(QRect(20, 120, 94, 28));
        Btn_gearInput = new QPushButton(tab);
        Btn_gearInput->setObjectName(QString::fromUtf8("Btn_gearInput"));
        Btn_gearInput->setGeometry(QRect(120, 120, 71, 30));
        Btn_auto = new QPushButton(tab);
        Btn_auto->setObjectName(QString::fromUtf8("Btn_auto"));
        Btn_auto->setGeometry(QRect(210, 40, 71, 30));
        Btn_manual = new QPushButton(tab);
        Btn_manual->setObjectName(QString::fromUtf8("Btn_manual"));
        Btn_manual->setGeometry(QRect(210, 80, 71, 30));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(30, 860, 300, 61));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(1590, 20, 300, 251));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy);
        groupBox_4->setMaximumSize(QSize(420, 1000));
        groupBox_4->setContextMenuPolicy(Qt::DefaultContextMenu);
        groupBox_4->setLayoutDirection(Qt::LeftToRight);
        groupBox_4->setAutoFillBackground(false);
        label_2 = new QLabel(groupBox_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 150, 64, 23));
        label_3 = new QLabel(groupBox_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(50, 90, 64, 23));
        label_4 = new QLabel(groupBox_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(50, 210, 101, 23));
        label_6 = new QLabel(groupBox_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(50, 30, 64, 23));
        label_7 = new QLabel(groupBox_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(50, 60, 64, 23));
        label = new QLabel(groupBox_4);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 180, 64, 23));
        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(50, 120, 64, 23));
        lcdNumber = new QLCDNumber(groupBox_4);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(180, 30, 70, 23));
        lcdNumber->setFrameShadow(QFrame::Plain);
        lcdNumber->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_2 = new QLCDNumber(groupBox_4);
        lcdNumber_2->setObjectName(QString::fromUtf8("lcdNumber_2"));
        lcdNumber_2->setGeometry(QRect(180, 60, 70, 23));
        lcdNumber_2->setFrameShadow(QFrame::Plain);
        lcdNumber_2->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_3 = new QLCDNumber(groupBox_4);
        lcdNumber_3->setObjectName(QString::fromUtf8("lcdNumber_3"));
        lcdNumber_3->setGeometry(QRect(180, 90, 70, 23));
        lcdNumber_3->setFrameShadow(QFrame::Plain);
        lcdNumber_3->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_4 = new QLCDNumber(groupBox_4);
        lcdNumber_4->setObjectName(QString::fromUtf8("lcdNumber_4"));
        lcdNumber_4->setGeometry(QRect(180, 120, 70, 23));
        lcdNumber_4->setFrameShadow(QFrame::Plain);
        lcdNumber_4->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_5 = new QLCDNumber(groupBox_4);
        lcdNumber_5->setObjectName(QString::fromUtf8("lcdNumber_5"));
        lcdNumber_5->setGeometry(QRect(180, 150, 70, 23));
        lcdNumber_5->setFrameShadow(QFrame::Plain);
        lcdNumber_5->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_6 = new QLCDNumber(groupBox_4);
        lcdNumber_6->setObjectName(QString::fromUtf8("lcdNumber_6"));
        lcdNumber_6->setGeometry(QRect(180, 180, 70, 23));
        lcdNumber_6->setFrameShape(QFrame::Box);
        lcdNumber_6->setFrameShadow(QFrame::Plain);
        lcdNumber_6->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_7 = new QLCDNumber(groupBox_4);
        lcdNumber_7->setObjectName(QString::fromUtf8("lcdNumber_7"));
        lcdNumber_7->setGeometry(QRect(180, 210, 70, 23));
        lcdNumber_7->setFrameShadow(QFrame::Plain);
        lcdNumber_7->setSegmentStyle(QLCDNumber::Flat);
        AriadneLogo = new QLabel(centralWidget);
        AriadneLogo->setObjectName(QString::fromUtf8("AriadneLogo"));
        AriadneLogo->setGeometry(QRect(30, 20, 241, 71));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(200, 170, 100, 35));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setEnabled(true);
        pushButton_3->setGeometry(QRect(200, 210, 100, 35));
        pathmap = new QLabel(centralWidget);
        pathmap->setObjectName(QString::fromUtf8("pathmap"));
        pathmap->setGeometry(QRect(460, 150, 500, 500));
        pathmap->setMaximumSize(QSize(1380, 1280));
        pathmap->setLayoutDirection(Qt::LeftToRight);
        parking = new QLabel(centralWidget);
        parking->setObjectName(QString::fromUtf8("parking"));
        parking->setGeometry(QRect(980, 150, 500, 500));
        parking->setMaximumSize(QSize(1380, 1280));
        parking->setLayoutDirection(Qt::LeftToRight);
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(1590, 280, 301, 171));
        lcdNumber_10 = new QLCDNumber(groupBox_2);
        lcdNumber_10->setObjectName(QString::fromUtf8("lcdNumber_10"));
        lcdNumber_10->setGeometry(QRect(169, 80, 81, 23));
        lcdNumber_10->setFrameShadow(QFrame::Plain);
        lcdNumber_10->setSegmentStyle(QLCDNumber::Flat);
        label_14 = new QLabel(groupBox_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(50, 55, 101, 16));
        label_15 = new QLabel(groupBox_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(50, 85, 101, 16));
        lcdNumber_8 = new QLCDNumber(groupBox_2);
        lcdNumber_8->setObjectName(QString::fromUtf8("lcdNumber_8"));
        lcdNumber_8->setGeometry(QRect(169, 20, 81, 23));
        lcdNumber_8->setFrameShadow(QFrame::Plain);
        lcdNumber_8->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_9 = new QLCDNumber(groupBox_2);
        lcdNumber_9->setObjectName(QString::fromUtf8("lcdNumber_9"));
        lcdNumber_9->setGeometry(QRect(169, 50, 81, 23));
        lcdNumber_9->setFrameShadow(QFrame::Plain);
        lcdNumber_9->setSegmentStyle(QLCDNumber::Flat);
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(50, 25, 101, 16));
        longitudeShift = new QPlainTextEdit(groupBox_2);
        longitudeShift->setObjectName(QString::fromUtf8("longitudeShift"));
        longitudeShift->setGeometry(QRect(110, 120, 61, 25));
        longitudeShift->viewport()->setProperty("cursor", QVariant(QCursor(Qt::IBeamCursor)));
        longitudeShift->setAcceptDrops(true);
        longitudeShift->setUndoRedoEnabled(true);
        latitudeShift = new QPlainTextEdit(groupBox_2);
        latitudeShift->setObjectName(QString::fromUtf8("latitudeShift"));
        latitudeShift->setGeometry(QRect(40, 120, 61, 25));
        latitudeShift->viewport()->setProperty("cursor", QVariant(QCursor(Qt::IBeamCursor)));
        latitudeShift->setMouseTracking(false);
        latitudeShift->setAcceptDrops(true);
        latitudeShift->setLayoutDirection(Qt::LeftToRight);
        latitudeShift->setUndoRedoEnabled(true);
        shiftOK = new QPushButton(groupBox_2);
        shiftOK->setObjectName(QString::fromUtf8("shiftOK"));
        shiftOK->setGeometry(QRect(180, 120, 71, 25));
        label_24 = new QLabel(groupBox_2);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(50, 145, 51, 20));
        label_35 = new QLabel(groupBox_2);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setGeometry(QRect(113, 145, 61, 20));
        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(1590, 460, 301, 151));
        label_16 = new QLabel(groupBox_5);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(50, 30, 101, 16));
        label_17 = new QLabel(groupBox_5);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(50, 60, 101, 16));
        label_18 = new QLabel(groupBox_5);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(50, 90, 101, 16));
        label_19 = new QLabel(groupBox_5);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(50, 120, 101, 16));
        label_31 = new QLabel(groupBox_5);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(180, 30, 70, 12));
        label_32 = new QLabel(groupBox_5);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(180, 60, 70, 12));
        label_33 = new QLabel(groupBox_5);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(180, 90, 70, 12));
        label_34 = new QLabel(groupBox_5);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setGeometry(QRect(180, 120, 70, 12));
        groupBox_6 = new QGroupBox(centralWidget);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setGeometry(QRect(1590, 620, 301, 241));
        label_30 = new QLabel(groupBox_6);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(20, 210, 170, 16));
        label_23 = new QLabel(groupBox_6);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(20, 60, 170, 16));
        lcdNumber_19 = new QLCDNumber(groupBox_6);
        lcdNumber_19->setObjectName(QString::fromUtf8("lcdNumber_19"));
        lcdNumber_19->setGeometry(QRect(220, 120, 64, 23));
        lcdNumber_19->setFrameShadow(QFrame::Plain);
        lcdNumber_19->setSegmentStyle(QLCDNumber::Flat);
        label_27 = new QLabel(groupBox_6);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(20, 150, 170, 16));
        label_22 = new QLabel(groupBox_6);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(20, 30, 170, 16));
        lcdNumber_21 = new QLCDNumber(groupBox_6);
        lcdNumber_21->setObjectName(QString::fromUtf8("lcdNumber_21"));
        lcdNumber_21->setGeometry(QRect(220, 180, 64, 23));
        lcdNumber_21->setFrameShadow(QFrame::Plain);
        lcdNumber_21->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_16 = new QLCDNumber(groupBox_6);
        lcdNumber_16->setObjectName(QString::fromUtf8("lcdNumber_16"));
        lcdNumber_16->setGeometry(QRect(220, 60, 64, 23));
        lcdNumber_16->setFrameShadow(QFrame::Plain);
        lcdNumber_16->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_20 = new QLCDNumber(groupBox_6);
        lcdNumber_20->setObjectName(QString::fromUtf8("lcdNumber_20"));
        lcdNumber_20->setGeometry(QRect(220, 150, 64, 23));
        lcdNumber_20->setFrameShadow(QFrame::Plain);
        lcdNumber_20->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_22 = new QLCDNumber(groupBox_6);
        lcdNumber_22->setObjectName(QString::fromUtf8("lcdNumber_22"));
        lcdNumber_22->setGeometry(QRect(220, 210, 64, 23));
        lcdNumber_22->setFrameShadow(QFrame::Plain);
        lcdNumber_22->setSegmentStyle(QLCDNumber::Flat);
        label_28 = new QLabel(groupBox_6);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(220, 30, 56, 12));
        lcdNumber_18 = new QLCDNumber(groupBox_6);
        lcdNumber_18->setObjectName(QString::fromUtf8("lcdNumber_18"));
        lcdNumber_18->setGeometry(QRect(220, 90, 64, 23));
        lcdNumber_18->setFrameShadow(QFrame::Plain);
        lcdNumber_18->setSegmentStyle(QLCDNumber::Flat);
        label_25 = new QLabel(groupBox_6);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(20, 90, 170, 16));
        label_26 = new QLabel(groupBox_6);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(20, 120, 170, 16));
        label_29 = new QLabel(groupBox_6);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(20, 180, 170, 16));
        AriadneClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(AriadneClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1885, 21));
        AriadneClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(AriadneClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        AriadneClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(AriadneClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        AriadneClass->setStatusBar(statusBar);

        retranslateUi(AriadneClass);

        Btn_Bust->setDefault(false);
        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AriadneClass);
    } // setupUi

    void retranslateUi(QMainWindow *AriadneClass)
    {
        AriadneClass->setWindowTitle(QApplication::translate("AriadneClass", "Ariadne", nullptr));
        groupBox->setTitle(QApplication::translate("AriadneClass", "Sensor State", nullptr));
        label_9->setText(QApplication::translate("AriadneClass", "Platform", nullptr));
        label_12->setText(QApplication::translate("AriadneClass", "GPS", nullptr));
        label_13->setText(QApplication::translate("AriadneClass", "LiDAR", nullptr));
        label_10->setText(QApplication::translate("AriadneClass", "YOLO", nullptr));
        label_11->setText(QApplication::translate("AriadneClass", "SCNN", nullptr));
        statusPlatform->setText(QString());
        statusGps->setText(QString());
        statusLidar->setText(QString());
        statusYolo->setText(QString());
        statusScnn->setText(QString());
        groupBox_3->setTitle(QApplication::translate("AriadneClass", "Mission", nullptr));
        Btn_mission1->setText(QApplication::translate("AriadneClass", "1. Parking", nullptr));
        Btn_mission3->setText(QApplication::translate("AriadneClass", "2. InterLeft", nullptr));
        Btn_mission4->setText(QApplication::translate("AriadneClass", "3. InterRight", nullptr));
        Btn_mission8->setText(QApplication::translate("AriadneClass", "7. DynamicOb", nullptr));
        Btn_mission6->setText(QApplication::translate("AriadneClass", "5. InterStop", nullptr));
        Btn_mission5->setText(QApplication::translate("AriadneClass", "4. InterStraight", nullptr));
        Btn_mission7->setText(QApplication::translate("AriadneClass", "6. StaticOb", nullptr));
        Btn_kidsafe->setText(QString());
        Btn_Bust->setText(QString());
        label_20->setText(QApplication::translate("AriadneClass", "KidSafe", nullptr));
        label_21->setText(QApplication::translate("AriadneClass", "Bust", nullptr));
        Btn_mission9->setText(QApplication::translate("AriadneClass", "8. Basic", nullptr));
        btn_mission_exit->setText(QApplication::translate("AriadneClass", "\353\257\270\354\205\230 \354\242\205\353\243\214", nullptr));
        Btn_up->setText(QApplication::translate("AriadneClass", "\342\226\262", nullptr));
        Btn_down->setText(QApplication::translate("AriadneClass", "\342\226\274", nullptr));
        Btn_right->setText(QApplication::translate("AriadneClass", "\342\226\266", nullptr));
        Btn_left->setText(QApplication::translate("AriadneClass", "\342\227\200", nullptr));
        Btn_gearInput->setText(QApplication::translate("AriadneClass", "gear input", nullptr));
        Btn_auto->setText(QApplication::translate("AriadneClass", "Auto", nullptr));
        Btn_manual->setText(QApplication::translate("AriadneClass", "Manual", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("AriadneClass", "PC", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("AriadneClass", "Remote Controller", nullptr));
        pushButton->setText(QApplication::translate("AriadneClass", "E-Stop", nullptr));
        groupBox_4->setTitle(QApplication::translate("AriadneClass", "Platform", nullptr));
        label_2->setText(QApplication::translate("AriadneClass", "STEER", nullptr));
        label_3->setText(QApplication::translate("AriadneClass", "GEAR", nullptr));
        label_4->setText(QApplication::translate("AriadneClass", "Encoder", nullptr));
        label_6->setText(QApplication::translate("AriadneClass", "AorM", nullptr));
        label_7->setText(QApplication::translate("AriadneClass", "E-Stop", nullptr));
        label->setText(QApplication::translate("AriadneClass", "BRAKE", nullptr));
        label_8->setText(QApplication::translate("AriadneClass", "SPEED", nullptr));
        AriadneLogo->setText(QString());
        pushButton_2->setText(QApplication::translate("AriadneClass", "\354\204\274\354\204\234 \355\206\265\354\213\240", nullptr));
        pushButton_3->setText(QApplication::translate("AriadneClass", "\354\243\274\355\226\211 \354\213\234\354\236\221", nullptr));
        pathmap->setText(QString());
        parking->setText(QString());
        groupBox_2->setTitle(QApplication::translate("AriadneClass", "GPS", nullptr));
        label_14->setText(QApplication::translate("AriadneClass", "Longitude", nullptr));
        label_15->setText(QApplication::translate("AriadneClass", "Heading", nullptr));
        label_5->setText(QApplication::translate("AriadneClass", "Latitude", nullptr));
        longitudeShift->setPlainText(QApplication::translate("AriadneClass", "0", nullptr));
        latitudeShift->setPlainText(QApplication::translate("AriadneClass", "0", nullptr));
        shiftOK->setText(QApplication::translate("AriadneClass", "SHIFT", nullptr));
        label_24->setText(QApplication::translate("AriadneClass", "Latitude", nullptr));
        label_35->setText(QApplication::translate("AriadneClass", "Longitude", nullptr));
        groupBox_5->setTitle(QApplication::translate("AriadneClass", "SCNN", nullptr));
        label_16->setText(QApplication::translate("AriadneClass", "LeftLeftLine", nullptr));
        label_17->setText(QApplication::translate("AriadneClass", "LeftLine", nullptr));
        label_18->setText(QApplication::translate("AriadneClass", "RightLine", nullptr));
        label_19->setText(QApplication::translate("AriadneClass", "RightRightLine", nullptr));
        label_31->setText(QApplication::translate("AriadneClass", "None", nullptr));
        label_32->setText(QApplication::translate("AriadneClass", "None", nullptr));
        label_33->setText(QApplication::translate("AriadneClass", "None", nullptr));
        label_34->setText(QApplication::translate("AriadneClass", "None", nullptr));
        groupBox_6->setTitle(QApplication::translate("AriadneClass", "YOLO", nullptr));
        label_30->setText(QApplication::translate("AriadneClass", "Bust (m)", nullptr));
        label_23->setText(QApplication::translate("AriadneClass", "Intersection Distance (m)", nullptr));
        label_27->setText(QApplication::translate("AriadneClass", "Dynamic Obstacle (m)", nullptr));
        label_22->setText(QApplication::translate("AriadneClass", "Intersection State", nullptr));
        label_28->setText(QApplication::translate("AriadneClass", "None", nullptr));
        label_25->setText(QApplication::translate("AriadneClass", "Parking (m)", nullptr));
        label_26->setText(QApplication::translate("AriadneClass", "Static Obstacle (m)", nullptr));
        label_29->setText(QApplication::translate("AriadneClass", "Kidsafe (m)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AriadneClass: public Ui_AriadneClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARIADNE_H
