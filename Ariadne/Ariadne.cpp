#include "Ariadne.h"
#include <iostream>
#include "math.h"
#include "qevent.h"
#include <codecvt>
#include <string>

using utf8_str = std::string;
using u16_str = std::wstring;

// This file has same function as HyeAhn View
using namespace std;

Ui::AriadneClass* Ariadne::ui = new Ui::AriadneClass;

Ariadne::Ariadne(QWidget *parent)
	: QMainWindow(parent)
{
	ui->setupUi(this);

	dataContainer = DataContainer::getInstance();
	
	QSystemTrayIcon TrayIcon(QIcon("C:/Users/D-Ace/Documents/DYibre/temp.ico"));

	QPixmap pix("C:/Users/D-Ace/Documents/DYibre/temp.png");
	ui->AriadneLogo->setPixmap(pix);

	//  -------------------  Sensor Thread control ------------------------- //

	platformCom = new PlatformCom;
	platformThread = new QThread;
	platformCom->moveToThread(platformThread);
	connect(platformThread, SIGNAL(started()), platformCom, SLOT(comPlatform()));
	connect(platformCom, SIGNAL(PlatformExit()), platformCom, SLOT(comPlatform()));

	gpsCom = new GPSCom;
	gpsThread = new QThread;
	gpsCom->moveToThread(gpsThread);
	connect(gpsThread, SIGNAL(started()), gpsCom, SLOT(comGPS()));
	connect(gpsCom, SIGNAL(GPSExit()), gpsCom, SLOT(comGPS()));

	lidarCom = new LidarCom;
	lidarThread = new QThread;
	lidarCom->moveToThread(lidarThread);
	connect(lidarThread, SIGNAL(started()), lidarCom, SLOT(comLidar()));
	connect(lidarCom, SIGNAL(LidarExit()), lidarCom, SLOT(comLidar()));

	scnn = new Scnn;
	scnnThread = new QThread;
	scnn->moveToThread(scnnThread);
	connect(scnnThread, SIGNAL(started()), scnn, SLOT(boostScnn()));
	//connect(yolo, SIGNAL(yoloStarted()), this, SLOT(scnnStart()));
	connect(scnn, SIGNAL(drivingEnabled()), this, SLOT(onDrivingEnabled()));

	yolo = new Yolo;
	yoloThread = new QThread;
	yolo->moveToThread(yoloThread);
	connect(yoloThread, SIGNAL(started()), yolo, SLOT(comYolo()));

	//  -------------------  Driving control ------------------------- //

	driving = new Driving;
	drivingThread = new QThread;
	driving->moveToThread(drivingThread);
	connect(drivingThread, SIGNAL(started()), driving, SLOT(autoMode()));

	view = new View;
	connect(driving, SIGNAL(send2View(int)), view, SLOT(comView(int)));

	//  -------------------  UI control ------------------------- //

	ui->comboBox_6->addItems({ "Drive" , "Neutral", "Reverse" }); // gaer items input
	ui->Btn_mission1->setStyleSheet("Text-align:left");
	ui->Btn_mission3->setStyleSheet("Text-align:left");
	ui->Btn_mission4->setStyleSheet("Text-align:left");
	ui->Btn_mission5->setStyleSheet("Text-align:left");
	ui->Btn_mission6->setStyleSheet("Text-align:left");
	ui->Btn_mission7->setStyleSheet("Text-align:left");
	ui->Btn_mission8->setStyleSheet("Text-align:left");
	ui->Btn_mission9->setStyleSheet("Text-align:left");


	// --------------------- Platform control Using UI ---------------------------------//

	QObject::connect(ui->Btn_mission1, SIGNAL(clicked()), this, SLOT(clicked_btn_mission1()));
	QObject::connect(ui->Btn_mission9_2, SIGNAL(clicked()), this, SLOT(clicked_btn_mission2()));
	QObject::connect(ui->Btn_mission3, SIGNAL(clicked()), this, SLOT(clicked_btn_mission3()));
	QObject::connect(ui->Btn_mission4, SIGNAL(clicked()), this, SLOT(clicked_btn_mission4()));
	QObject::connect(ui->Btn_mission5, SIGNAL(clicked()), this, SLOT(clicked_btn_mission5()));
	QObject::connect(ui->Btn_mission6, SIGNAL(clicked()), this, SLOT(clicked_btn_mission6()));
	QObject::connect(ui->Btn_mission7, SIGNAL(clicked()), this, SLOT(clicked_btn_mission7()));
	QObject::connect(ui->Btn_mission8, SIGNAL(clicked()), this, SLOT(clicked_btn_mission8()));
	QObject::connect(ui->Btn_mission9, SIGNAL(clicked()), this, SLOT(clicked_btn_mission9()));
	QObject::connect(ui->btn_mission_exit, SIGNAL(clicked()), this, SLOT(clicked_btn_missionExit()));

	QObject::connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(clicked_btn_sensor()));
	QObject::connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(clicked_btn_driving_mid()));
	QObject::connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(clicked_btn_driving_fin()));
	QObject::connect(ui->Btn_gearInput, SIGNAL(clicked()), this, SLOT(gear_input()));
	QObject::connect(ui->Btn_up, SIGNAL(clicked()), this, SLOT(clicked_speed_up()));
	QObject::connect(ui->Btn_down, SIGNAL(clicked()), this, SLOT(clicked_speed_down()));
	QObject::connect(ui->Btn_left, SIGNAL(clicked()), this, SLOT(clicked_steer_left()));
	QObject::connect(ui->Btn_right, SIGNAL(clicked()), this, SLOT(clicked_steer_right()));
	QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(clicked_E_stop()));
	QObject::connect(ui->Btn_auto, SIGNAL(clicked()), this, SLOT(clicked_auto()));
	QObject::connect(ui->Btn_manual, SIGNAL(clicked()), this, SLOT(clicked_manual()));
	QObject::connect(ui->Btn_Bust, SIGNAL(clicked(bool)), this, SLOT(clicked_btn_bust(bool)));
	QObject::connect(ui->Btn_kidsafe, SIGNAL(clicked(bool)), this, SLOT(clicked_btn_kidsafe(bool)));
	QObject::connect(ui->shiftOK, SIGNAL(clicked()), this, SLOT(clicked_btn_shift()));

	// ------------------- UI update for Platform & GPS status ----------------------//

	connect(platformCom, SIGNAL(AorMChanged(int)), this, SLOT(onAorMChanged(int)));
	connect(platformCom, SIGNAL(EStopChanged(int)), this, SLOT(onEStopChanged(int)));
	connect(platformCom, SIGNAL(GearChanged(int)), this, SLOT(onGearChanged(int)));
	connect(platformCom, SIGNAL(SpeedChanged(int)), this, SLOT(onSpeedChanged(int)));
	connect(platformCom, SIGNAL(SteerChanged(int)), this, SLOT(onSteerChanged(int)));
	connect(platformCom, SIGNAL(BreakChanged(int)), this, SLOT(onBreakChanged(int)));
	connect(platformCom, SIGNAL(EncChanged(int)), this, SLOT(onEncChanged(int)));

	connect(gpsCom, SIGNAL(latitudeChanged(int)), this, SLOT(onLatitudeChanged(int)));
	connect(gpsCom, SIGNAL(longitudeChanged(int)), this, SLOT(onLongitudeChanged(int)));
	connect(gpsCom, SIGNAL(headingChanged(double)), this, SLOT(onHeadingChanged(double)));

	// ------------------- UI update for Mission ----------------------//

	QObject::connect(driving, SIGNAL(currentMission(int)), this, SLOT(onCurrentMission(int)));
	QObject::connect(driving, SIGNAL(exitMission(int)), this, SLOT(onExitMission(int)));
	QObject::connect(yolo, SIGNAL(BustExist(bool)), this, SLOT(onBustExist(bool)));
	QObject::connect(yolo, SIGNAL(KidsafeExist(bool)), this, SLOT(onKidsafeExist(bool)));

	TimerUIUpdate = new QTimer(this);
	QTimer::connect(TimerUIUpdate, &QTimer::timeout, this, &Ariadne::updateUI);
	TimerSensorStatus = new QTimer(this);
	QTimer::connect(TimerSensorStatus, &QTimer::timeout, this, &Ariadne::updateSensorStatus);
}

void Ariadne::closeEvent(QCloseEvent *e) {
	//delete platformCom;
	//delete lidarCom;
	//delete gpsCom;
	//delete scnn;
	delete yolo;
	//delete view;
}
// This function is to change comport numbers from CString to QString.
CString ConvertQstringtoCString(QString qs)
{
	std::string utf8_text = qs.toUtf8().constData();
	CString cs(utf8_text.c_str());
	return cs;
}
void Ariadne::clicked_auto() {
	dataContainer->setValue_UtoP_AorM(1);
}

void Ariadne::clicked_manual() {
	dataContainer->setValue_UtoP_AorM(0);
}

void Ariadne::scnnStart() {
	if (!scnnThread->isRunning()) {
		ui->pushButton_3->setEnabled(false); 
		scnnThread->start();
	}
}

void Ariadne::onDrivingEnabled() {
	Sleep(1500);
	ui->pushButton_3->setEnabled(true);
}

void Ariadne::onCurrentMission(int id) {
	ui->btn_mission_exit->setEnabled(true);
	ui->Btn_mission1->setEnabled(true);
	ui->Btn_mission3->setEnabled(true);
	ui->Btn_mission4->setEnabled(true);
	ui->Btn_mission5->setEnabled(true);
	ui->Btn_mission6->setEnabled(true);
	ui->Btn_mission7->setEnabled(true);
	ui->Btn_mission8->setEnabled(true);
	ui->Btn_mission9->setEnabled(true);
	ui->Btn_mission9_2->setEnabled(true);

	switch (id) {
	case 0:
		ui->btn_mission_exit->setEnabled(false);
		ui->plainTextEdit->appendPlainText("Mission Exit");
		break;
	case PARKING:
		ui->Btn_mission1->setEnabled(false);
		ui->plainTextEdit->appendPlainText("Parking Mission Start");
		parkingOn = true;
		break;
	case BUSLANE:
		ui->Btn_mission9_2->setEnabled(false);
		ui->plainTextEdit->appendPlainText("BusLane Mission Start");
		break;
	case INTER_LEFT:
		ui->Btn_mission3->setEnabled(false);
		ui->plainTextEdit->appendPlainText("Intersection left mission start");
		break;
	case INTER_RIGHT:
		ui->Btn_mission4->setEnabled(false);
		ui->plainTextEdit->appendPlainText("Intersection right mission start");
		break;
	case INTER_STRAIGHT:
		ui->Btn_mission5->setEnabled(false);
		ui->plainTextEdit->appendPlainText("intersection straight mission start");
		break;
	case INTER_STOP:
		ui->Btn_mission6->setEnabled(false);
		ui->plainTextEdit->appendPlainText("intersection stop mission start");
		break;
	case STATIC_OBSTACLE:
		ui->Btn_mission7->setEnabled(false);
		ui->plainTextEdit->appendPlainText("Static Obstacle Mission Start");
		break;
	case DYNAMIC_OBSTACLE:
		ui->Btn_mission8->setEnabled(false);
		ui->plainTextEdit->appendPlainText("Dynamic Obstacle Mission Start");
		break;
	case BASIC:
		ui->Btn_mission9->setEnabled(false);
		ui->plainTextEdit->appendPlainText("Basic PASIV algorithm start");
		break;
	}

}
void Ariadne::onExitMission(int id) {
	switch (id) {
	case PARKING:
		parkingOn = false;
		//ui->parking->clear();
		break;
	case INTER_LEFT:
		break;
	case INTER_RIGHT:
		break;
	case INTER_STRAIGHT:
		break;
	case INTER_STOP:
		break;
	case STATIC_OBSTACLE:
		break;
	case DYNAMIC_OBSTACLE:
		break;
	case BASIC:
		break;
	}
}
void Ariadne::onBustExist(bool bust) {
	if (bust)
		ui->label_21->setStyleSheet("background-color: rgb(255, 51, 51)");
	else
		ui->label_21->setStyleSheet("background-color: rgb(0, 204, 102)");
}

void Ariadne::onKidsafeExist(bool kidsafe) {
	if (kidsafe)
		ui->label_20->setStyleSheet("background-color: rgb(255, 51, 51)");
	else
		ui->label_20->setStyleSheet("background-color: rgb(0, 204, 102)");
}

// This function is to start communication with sensor.
void Ariadne::clicked_btn_sensor() {
	//SENSOR SWITCH

	AutoPortFinder();

	if (!yoloThread->isRunning()) { yoloThread->start(); }

	if (!scnnThread->isRunning()) { ui->pushButton_3->setEnabled(false); scnnThread->start(); }

	if (!platformThread->isRunning()) { platformThread->start(); }
	
	if (!lidarThread->isRunning()) { lidarThread->start(); }

	if (!gpsThread->isRunning()) { gpsThread->start(); }

	if (!TimerSensorStatus->isActive()) {
		TimerSensorStatus->start(1000);
	}
		
}

void Ariadne::clicked_btn_bust(bool bust) {
	/// if bust is on(true), speed ratio will be decreased.
	if (bust) { dataContainer->setValue_speed_ratio_bust(SPEED_RATIO_LOW);
	ui->label_21->setStyleSheet("background-color: rgb(255, 51, 51)");
	}
	else { dataContainer->setValue_speed_ratio_bust(1); 
	ui->label_21->setStyleSheet("background-color: rgb(0, 204, 102)");
	}
}

void Ariadne::clicked_btn_kidsafe(bool kidsafe) {
	/// if kidsafe is on(true) speed ratio will be decreased.
	if (kidsafe) { dataContainer->setValue_speed_ratio_kid(SPEED_RATIO_LOW);
	ui->label_20->setStyleSheet("background-color: rgb(255, 51, 51)");
	}
	else { dataContainer->setValue_speed_ratio_kid(1); 
	ui->label_20->setStyleSheet("background-color: rgb(0, 204, 102)");
	}
}


// This function is to start driving
void Ariadne::clicked_btn_driving_mid() {
	
	//lidar, scnn 켰을 때만 실행
	if (!drivingThread->isRunning()) {
		driving->parkingNum = 1;
		drivingThread->start();
	}

	if(!TimerUIUpdate->isActive())    
		TimerUIUpdate->start(20);
}

void Ariadne::clicked_btn_driving_fin() {

	//lidar, scnn 켰을 때만 실행
	driving->parkingNum = 0;
	if (!drivingThread->isRunning()) {
		drivingThread->start();
	}

	if (!TimerUIUpdate->isActive())
		TimerUIUpdate->start(20);
	
}

void Ariadne::updateUI() {

	vector<int> arr;
	QString str;

	if (yoloThread->isRunning()) {
		arr = dataContainer->getValue_yolo_missions();
		if (arr[0] == GREEN) str = QString("GREEN");
		else if (arr[0] == GREEN_LEFT) str = QString("GREEN LEFT");
		else if (arr[0] == RED) str = QString("RED");
		else if (arr[0] == RED_LEFT) str = QString("RED LEFT");
		else if (arr[0] == RED_YELLOW) str = QString("RED YELLOW");
		else if (arr[0] == YELLOW) str = QString("YELLOW");
		else str = QString("None");

		ui->label_28->setText(str);
		ui->lcdNumber_16->display(arr[1]);
		ui->lcdNumber_18->display(arr[3]);
		ui->lcdNumber_19->display(arr[5]);
		ui->lcdNumber_20->display(arr[6]);
		ui->lcdNumber_21->display(arr[7]);
		ui->lcdNumber_22->display(arr[8]);
		ui->lcdNumber_23->display(arr[4]);
	}
	
	if (scnnThread->isRunning()) {
		arr = dataContainer->getValue_scnn_existLanes();

		if (arr[0] == 1) str = QString("White Lane");
		else if (arr[0] == 2) str = QString("Yellow Lane");
		else if (arr[0] == 3) str = QString("Bus Lane");
		else str = QString("None");
		ui->label_31->setText(str);

		if (arr[1] == 1) str = QString("White Lane");
		else if (arr[1] == 2) str = QString("Yellow Lane");
		else if (arr[1] == 3) str = QString("Bus Lane");
		else str = QString("None");
		ui->label_32->setText(str);

		if (arr[2] == 1) str = QString("White Lane");
		else if (arr[2] == 2) str = QString("Yellow Lane");
		else if (arr[2] == 3) str = QString("Bus Lane");
		else str = QString("None");
		ui->label_33->setText(str);

		if (arr[3] == 1) str = QString("White Lane");
		else if (arr[3] == 2) str = QString("Yellow Lane");
		else if (arr[3] == 3) str = QString("Bus Lane");
		else str = QString("None");
		ui->label_34->setText(str);
	}

	if (drivingThread->isRunning()) {
		ui->pathmap->setPixmap(QPixmap::fromImage(dataContainer->getValue_ui_pathmap()));
	}
	if (parkingOn) {
		ui->parking->setPixmap(QPixmap::fromImage(dataContainer->getValue_ui_parking()));
	}
}


void Ariadne::clicked_btn_mission1() {
	AutoPortFinder();
	dataContainer->setValue_yolo_missionID(PARKING);
}

void Ariadne::clicked_btn_mission2() {
	dataContainer->setValue_yolo_missionID(BUSLANE);
}

void Ariadne::clicked_btn_mission3() {
	dataContainer->setValue_yolo_missionID(INTER_LEFT);	
}

void Ariadne::clicked_btn_mission4() {
	dataContainer->setValue_yolo_missionID(INTER_RIGHT);
}

void Ariadne::clicked_btn_mission5() {
	dataContainer->setValue_yolo_missionID(INTER_STRAIGHT);
}

void Ariadne::clicked_btn_mission6() {
	dataContainer->setValue_yolo_missionID(INTER_STOP);
}

void Ariadne::clicked_btn_mission7() {
	dataContainer->setValue_yolo_missionID(STATIC_OBSTACLE);	
}

void Ariadne::clicked_btn_mission8() {
	dataContainer->setValue_yolo_missionID(DYNAMIC_OBSTACLE);
}

void Ariadne::clicked_btn_mission9() {
	dataContainer->setValue_yolo_missionID(BASIC);
}

void Ariadne::clicked_btn_missionExit() {
	dataContainer->setValue_yolo_missionID(0);
}

void Ariadne::clicked_btn_shift() {
	bool ok;
	double shift;
	shift = ui->latitudeShift->toPlainText().toDouble(&ok);
	if(ok) dataContainer->setValue_gps_latitude_shift(shift);
	ui->plainTextEdit->appendPlainText(QString::number(shift));
	shift = ui->longitudeShift->toPlainText().toDouble(&ok);
	if(ok) 	dataContainer->setValue_gps_longitude_shift(shift);
	ui->plainTextEdit->appendPlainText(QString::number(shift));
	ui->plainTextEdit->appendPlainText("gps shift");
	
}

// These functions is to control gplatform
void Ariadne::gear_input()
{
	QString qs;
	qs = ui->comboBox_6->currentText();
	if (qs == "Drive") { dataContainer->setValue_UtoP_GEAR(0); }
	else if (qs == "Neutral") { dataContainer->setValue_UtoP_GEAR(1); }
	else { dataContainer->setValue_UtoP_GEAR(2); }
}

void Ariadne::clicked_speed_up() { dataContainer->setValue_UtoP_SPEED(dataContainer->getValue_UtoP_SPEED() + 1); }
void Ariadne::clicked_speed_down() { dataContainer->setValue_UtoP_SPEED(dataContainer->getValue_UtoP_SPEED() - 1); }
void Ariadne::clicked_steer_left() { dataContainer->setValue_UtoP_STEER(dataContainer->getValue_UtoP_STEER() - 100); }
void Ariadne::clicked_steer_right() { dataContainer->setValue_UtoP_STEER(dataContainer->getValue_UtoP_STEER() + 100); }
void Ariadne::clicked_E_stop()
{
	if (dataContainer->getValue_UtoP_E_STOP() == 0) { dataContainer->setValue_UtoP_E_STOP(1); }
	else { dataContainer->setValue_UtoP_E_STOP(0); }
}

void Ariadne::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Left) { clicked_steer_left(); }
	if (event->key() == Qt::Key_Right) { clicked_steer_right(); }
	if (event->key() == Qt::Key_Up) { clicked_speed_up(); }
	if (event->key() == Qt::Key_Down) { clicked_speed_down(); }
	if (event->key() == Qt::Key_Enter) { clicked_E_stop(); }
}

//  _    _ _____    _    _ _____  _____       _______ ______
// | |  | |_   _|  | |  | |  __ \|  __ \   /\|__   __|  ____|
// | |  | | | |    | |  | | |__) | |  | | /  \  | |  | |__
// | |  | | | |    | |  | |  ___/| |  | |/ /\ \ | |  |  __|
// | |__| |_| |_   | |__| | |    | |__| / ____ \| |  | |____
//  \____/|_____|   \____/|_|    |_____/_/    \_\_|  |______|
// this part is for UI updating functions; display, slots... etc

void Ariadne::onAorMChanged(int Number) { ui->lcdNumber->display(Number); }
void Ariadne::onEStopChanged(int Number) { ui->lcdNumber_2->display(Number); }
void Ariadne::onGearChanged(int Number) { ui->lcdNumber_3->display(Number); }
void Ariadne::onSpeedChanged(int Number) { ui->lcdNumber_4->display(Number); }
void Ariadne::onSteerChanged(int Number) { ui->lcdNumber_5->display(Number); }
void Ariadne::onBreakChanged(int Number) { ui->lcdNumber_6->display(Number); }
void Ariadne::onEncChanged(int Number) { ui->lcdNumber_7->display(Number); }
void Ariadne::onLatitudeChanged(int Number) { ui->lcdNumber_8->display(Number); }

void Ariadne::onLongitudeChanged(int Number) { ui->lcdNumber_9->display(Number); }
void Ariadne::onHeadingChanged(double Number) { ui->lcdNumber_10->display(Number); }

void Ariadne::AutoPortFinder() {

	SP_DEVINFO_DATA devInfoData = {};
	devInfoData.cbSize = sizeof(devInfoData);

	// get the tree containing the info for the ports
	HDEVINFO hDeviceInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS,
		0,
		nullptr,
		DIGCF_PRESENT
	);
	if (hDeviceInfo == INVALID_HANDLE_VALUE)
	{
		return;
	}

	// iterate over all the devices in the tree
	int nDevice = 0;
	while (SetupDiEnumDeviceInfo(hDeviceInfo,            // Our device tree
		nDevice++,            // The member to look for
		&devInfoData))
	{
		DWORD regDataType;
		DWORD reqSize = 0;

		//428843652
		// find the size required to hold the device info
		SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_HARDWAREID, nullptr, nullptr, 0, &reqSize);
		BYTE* hardwareId = new BYTE[(reqSize > 1) ? reqSize : 1];
		// now store it in a buffer

		if (SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_HARDWAREID, &regDataType, hardwareId, sizeof(hardwareId) * reqSize, nullptr))
		{
			// find the size required to hold the friendly name
			reqSize = 0;
			SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_FRIENDLYNAME, nullptr, nullptr, 0, &reqSize);

			BYTE* friendlyName = new BYTE[(reqSize > 1) ? reqSize : 1];
			//TCHAR friendly_name[256];
			// now store it in a buffer


			if (!SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_FRIENDLYNAME, nullptr, friendlyName, sizeof(friendlyName) * reqSize, nullptr))
			{
				// device does not have this property set
				memset(friendlyName, 0, reqSize > 1 ? reqSize : 1);
			}
			else {
				CString hwID((const wchar_t*)hardwareId);
				CString strPort((const wchar_t*)friendlyName);
				CString port = strPort.Mid(reqSize / 2 - 6, 4);
				//printf("port = %S\n port name = %S\n port num = %S\n", strPort, portName, port);
				if (hwID == "AX99100MF\\AX99100_COM") {
					dataContainer->setValue_gps_port(port);
				}
				else if (hwID == "USB\\VID_067B&PID_2303&REV_0300") {
				}
				dataContainer->setValue_platform_port(L"COM1");
			}
			// use friendlyName here
			delete[] friendlyName;

		}
		delete[] hardwareId;
	}
}

// This function is to change UI according to Sensor communication status
void Ariadne::updateSensorStatus()
{
	// Platform communication
	if (dataContainer->getValue_platform_status() > 10) {
		ui->statusPlatform->setStyleSheet("background-color: rgb(0, 204, 102);");
		ui->statusPlatform->setFixedWidth(60);
	}
	else if (dataContainer->getValue_platform_status() > 0) {
		ui->statusPlatform->setStyleSheet("background-color: yellow;");
		ui->statusPlatform->setFixedWidth(40);
	}
	else {
		ui->statusPlatform->setStyleSheet("background-color: rgb(255, 51, 51);");
		ui->statusPlatform->setFixedWidth(20);
	}
	dataContainer->setValue_platform_status(0);

	// LiDAR communication code
	if (dataContainer->getValue_lidar_status() > 5) {
		ui->statusLidar->setStyleSheet("background-color: rgb(0, 204, 102);");
		ui->statusLidar->setFixedWidth(60);
	}
	else if (dataContainer->getValue_lidar_status() > 0) {
		ui->statusLidar->setStyleSheet("background-color: yellow;");
		ui->statusLidar->setFixedWidth(40);
	}
	else {
		ui->statusLidar->setStyleSheet("background-color: rgb(255, 51, 51);");
		ui->statusLidar->setFixedWidth(20);
	}
	dataContainer->setValue_lidar_status(0);

	// GPS communication code
	if (dataContainer->getValue_gps_status() > 5) {
		ui->statusGps->setStyleSheet("background-color: rgb(0, 204, 102);");
		ui->statusGps->setFixedWidth(60);
	}
	else if (dataContainer->getValue_gps_status() > 0) {
		ui->statusGps->setStyleSheet("background-color: yellow;");
		ui->statusGps->setFixedWidth(40);
	}
	else {
		ui->statusGps->setStyleSheet("background-color: rgb(255, 51, 51);");
		ui->statusGps->setFixedWidth(20);
	}
	dataContainer->setValue_gps_status(0);

	// scnn communication code
	if (dataContainer->getValue_scnn_status() > 5) {
		ui->statusScnn->setStyleSheet("background-color: rgb(0, 204, 102);");
		ui->statusScnn->setFixedWidth(60);
	}
	else if (dataContainer->getValue_scnn_status() > 0) {
		ui->statusScnn->setStyleSheet("background-color: yellow;");
		ui->statusScnn->setFixedWidth(40);
	}
	else {
		ui->statusScnn->setStyleSheet("background-color: rgb(255, 51, 51);");
		ui->statusScnn->setFixedWidth(20);
	}
	dataContainer->setValue_scnn_status(0);

	// yolo communication code
	if (dataContainer->getValue_yolo_status() > 5) {
		ui->statusYolo->setStyleSheet("background-color: rgb(0, 204, 102);");
		ui->statusYolo->setFixedWidth(60);
	}
	else if (dataContainer->getValue_yolo_status() > 0) {
		ui->statusYolo->setStyleSheet("background-color: yellow;");
		ui->statusYolo->setFixedWidth(40);
	}
	else {
		ui->statusYolo->setStyleSheet("background-color: rgb(255, 51, 51);");
		ui->statusYolo->setFixedWidth(20);
	}
	dataContainer->setValue_yolo_status(0);
}

Ui::AriadneClass* Ariadne::getUI() { return ui; }

/// ----------------- Platform Communication Function -------------- ///

PlatformCom::PlatformCom()
{
	dataContainer = DataContainer::getInstance();
	ui = Ariadne::getUI();
}

void PlatformCom::comPlatform() {
	cout << "platform start" << endl;

	if (_platform.OpenPort(L"COM3"))
	{
		_platform.ConfigurePort(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT);
		_platform.SetCommunicationTimeouts(0, 0, 0, 0, 0);

		while (loopStatusPlatform)
		{
			if (_platform.MyCommRead()) {}
			else {
				_platform.ClosePort();
				emit(PlatformExit());
				return;
			}
			_platform.MyCommWrite();
			dataContainer->updateValue_platform_status();

			emit(AorMChanged(dataContainer->getValue_PtoU_AorM()));
			emit(EStopChanged(dataContainer->getValue_PtoU_E_STOP()));
			emit(SpeedChanged(dataContainer->getValue_PtoU_SPEED()));
			emit(SteerChanged(dataContainer->getValue_PtoU_STEER()));
			emit(GearChanged(dataContainer->getValue_PtoU_GEAR()));
			emit(BreakChanged(dataContainer->getValue_PtoU_BRAKE()));
			emit(EncChanged(dataContainer->getValue_PtoU_ENC()));

			cout << "platform communication" << endl;
			//Sleep(20);
		}
	}
	else {
		cout << "platform not connect\n";
		emit(PlatformExit());
		return;
	}
}

/// ------------------ GPS Communication Function -----------------///

#define threshold 0.5
#define PI 3.14159265358979323846
#define radiRatio 1 / 298.257223563
#define lamda 129.0*PI/180.0
#define Eoff 500000
#define k0 0.9996
#define radi 6378137
#define GPSCOM L"COM14"

bool sign;

CSerialPort _gps;
///void HeroNeverDies();
vector <double>UTM(double lat, double lng);
bool GEOFENCE(double x, double y, vector<vector<double>> map_link, double heading);

GPSCom::GPSCom() {
	ui = Ariadne::getUI();
	dataContainer = DataContainer::getInstance();

}

//얘가 실시간 좌표찍는 애임 , 클릭버튼했을 대 이 함수호출되도록했음
void GPSCom::comGPS() { // rt ; Real Time

	// dataContainer->getValue_gps_port()
	if (_gps.OpenPort(GPSCOM)) {

		_gps.ConfigurePortW(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT);
		_gps.SetCommunicationTimeouts(0, 0, 0, 0, 0);
		string tap;
		string tap2;
		vector<string> vec;
		while (1) {
			BYTE * pByte = new BYTE[512]; // 2028
			if (_gps.ReadByte(pByte, 512)) {

				dataContainer->updateValue_gps_status();

				pByte[511] = '\0'; // 2027

				const char * p = (const char*)pByte;

				stringstream str(p);

				while (getline(str, tap, '\n')) {
					//cout << tap;
					stringstream qwe(tap);

					while (getline(qwe, tap2, ',')) {
						vec.push_back(tap2);
					}
					//cout << vec[0] << endl;

					//cout << vec.size() << endl;

					if (vec.size() > 8) {
						if (vec[0] == "$GNRMC" && vec[2] == "A") {
							//ofile << vec[0] << ',' << vec[3] << ',' << vec[5]  << endl;
							_lat = ((atof(vec[3].c_str()) - 3700) / 60) + 37; // 35랑 128은 상황에 따라 바꿔줘야함
							_lng = ((atof(vec[5].c_str()) - 12600) / 60) + 126; // 37, 126
							heading = atof(vec[8].c_str()) * CV_PI / 180; //[rad]

							vector<double >utm = UTM(_lat, _lng);
							lat = utm[0];
							lng = utm[1];

							dataContainer->resetValue_gps_valid();
							dataContainer->setValue_gps_heading(heading);
							dataContainer->setValue_gps_latitude(lat);
							dataContainer->setValue_gps_longitude(lng);

							emit latitudeChanged(dataContainer->getValue_gps_latitude());
							emit longitudeChanged(dataContainer->getValue_gps_longitude()); /// 숫자가 너무 커서 나눴음
							emit headingChanged(dataContainer->getValue_gps_heading());

							//cout << lat << "    " << lng << endl;
						}
						else if (vec[2] == "V") {
							dataContainer->count_gps_valid();

						}
					}
					vec.clear();
				}
			}
			else {
				cout << "GPS not connect" << endl;
				_gps.ClosePort();
				emit(GPSExit());
				return;
			}
			Sleep(100);
		}
	}
	else {
		cout << "port not connect" << endl;
		emit(GPSExit());
	}
}

vector <double>UTM(double lat, double lng) { /// This function is to calculate UTM parameters.
	double lat_rad = lat * PI / 180.0;
	double lng_rad = lng * PI / 180.0;

	double n = radiRatio / (2 - radiRatio);
	double A = radi * (1 - n + 5.0 / 4.0 * (pow(n, 2) - pow(n, 3)) + 81.0 / 64.0 * (pow(n, 4) - pow(n, 5)));
	double B = 3.0 / 2.0 * radi*(n - pow(n, 2) + 7.0 / 8.0 * (pow(n, 3) - pow(n, 4)) + 55.0 / 64.0* pow(n, 5));
	double C = 15.0 / 16.0 * radi*(pow(n, 2) - pow(n, 3) + 3.0 / 4.0 * (pow(n, 4) - pow(n, 5)));
	double D = 35.0 / 48.0 *radi*(pow(n, 3) - pow(n, 4) + 11.0 / 16.0 * pow(n, 5));
	double Ed = 315.0 / 512.0 * radi*(pow(n, 4) - pow(n, 5));
	double S = A * lat_rad - B * sin(2 * lat_rad) + C * sin(4 * lat_rad) - D * sin(6 * lat_rad) + Ed * sin(8 * lat_rad);
	double esq = radiRatio * (2 - radiRatio);
	double v = radi / sqrt(1 - esq * pow(sin(lat_rad), 2));
	double esqd = radiRatio * (2 - radiRatio) / pow((1 - radiRatio), 2);

	double T1 = k0 * S;
	double T2 = v * sin(lat_rad)*cos(lat_rad)*k0 / 2.0;
	double T3 = v * sin(lat_rad)*pow(cos(lat_rad), 3)*k0 / 24.0 * (5.0 - pow(tan(lat_rad), 2) + 9.0 * esqd*pow(cos(lat_rad), 2) + 4.0 * pow(esqd, 2)*pow(cos(lat_rad), 4));
	double T4 = v * sin(lat_rad)*pow(cos(lat_rad), 5)*k0 / 720.0 * (61.0 - 58.0 * pow(tan(lat_rad), 2) + pow(tan(lat_rad), 4) + 270.0 * esqd*pow(cos(lat_rad), 2) - 330.0 * esqd*pow(tan(lat_rad)*cos(lat_rad), 2)
		+ 445.0 * pow(esqd, 2)*pow(cos(lat_rad), 4) + 324.0 * pow(esqd, 3)*pow(cos(lat_rad), 6) - 680.0 *pow(tan(lat_rad), 2)* pow(esqd, 2)*pow(cos(lat_rad), 4) + 88.0 * pow(esqd, 4)*pow(cos(lat_rad), 8) - 600.0 * pow(esqd, 3)*pow(tan(lat_rad), 2)*pow(cos(lat_rad), 6)
		- 192.0 * pow(esqd, 4)*pow(tan(lat_rad), 2)*pow(cos(lat_rad), 8));
	double T5 = v * sin(lat_rad)*pow(cos(lat_rad), 7)*k0 / 40320.0 * (1385.0 - 3111.0 * pow(tan(lat_rad), 2) + 543.0 * pow(tan(lat_rad), 4) - pow(tan(lat_rad), 6));
	double T6 = v * cos(lat_rad)*k0;
	double T7 = v * pow(cos(lat_rad), 3)*k0 / 6.0 * (1.0 - pow(tan(lat_rad), 2) + esqd * pow(cos(lat_rad), 2));
	double T8 = v * pow(cos(lat_rad), 5)*k0 / 120.0 * (5.0 - 18.0 * pow(tan(lat_rad), 2) + pow(tan(lat_rad), 4) + 14.0 * esqd*pow(cos(lat_rad), 2) - 58.0 * esqd*pow(tan(lat_rad)*cos(lat_rad), 2) + 13.0 * pow(esqd, 2)*pow(cos(lat_rad), 4) + 4.0 * pow(esqd, 3)*pow(cos(lat_rad), 6)
		- 64.0 * pow(esqd*tan(lat_rad), 2)*pow(cos(lat_rad), 4) - 24.0 * pow(tan(lat_rad), 2)*pow(esqd, 3)*pow(cos(lat_rad), 6));
	double T9 = v * pow(cos(lat_rad), 7)*k0 / 5040.0 * (61.0 - 479.0 * pow(tan(lat_rad), 2) + 179.0 * pow(tan(lat_rad), 4) - pow(tan(lat_rad), 6));

	double dellng = lng_rad - lamda;

	//offset
	double N = T1 + pow(dellng, 2)*T2 + pow(dellng, 4)*T3 + pow(dellng, 6)*T4 + pow(dellng, 8)*T5 + 1.32;
	double E = Eoff + dellng * T6 + pow(dellng, 3)*T7 + pow(dellng, 5)*T8 + pow(dellng, 7)*T9;

	vector<double> utm;
	utm.push_back(E);
	utm.push_back(N);
	return utm;
}

