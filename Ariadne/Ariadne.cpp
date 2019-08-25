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

	yolo = new Yolo;
	yoloThread = new QThread;
	yolo->moveToThread(yoloThread);
	connect(yoloThread, SIGNAL(started()), yolo, SLOT(comYolo()));

	//  -------------------  Driving control ------------------------- //

	driving = new Driving;
	drivingThread = new QThread;
	driving->moveToThread(drivingThread);

	//SWITCH LIDAR CONTROLING.
	connect(drivingThread, SIGNAL(started()), driving, SLOT(autoMode()));
	//connect(drivingThread, SIGNAL(started()), driving, SLOT(LOS()));

	missionUpdate = new MissionUpdate;
	missionUpdateThread = new QThread;
	missionUpdate->moveToThread(missionUpdateThread);
	connect(missionUpdateThread, SIGNAL(started()), missionUpdate, SLOT(MissionIDUpdate()));

	view = new View;
	connect(driving, SIGNAL(send2View(int)), view, SLOT(comView(int)));

    //  -------------------  UI control ------------------------- //


    ui->comboBox_6->addItems({ "Drive" , "Neutral", "Reverse"}); // gaer items input

    // --------------------- Platform control Using UI ---------------------------------//

    QObject::connect(ui->Btn_Mission1, SIGNAL(clicked()), this, SLOT(clicked_btn_mission1()));
	QObject::connect(ui->Btn_mission3, SIGNAL(clicked()), this, SLOT(clicked_btn_mission3()));
	QObject::connect(ui->Btn_mission4, SIGNAL(clicked()), this, SLOT(clicked_btn_mission4()));
	QObject::connect(ui->Btn_mission5, SIGNAL(clicked()), this, SLOT(clicked_btn_mission5()));
	QObject::connect(ui->Btn_mission6, SIGNAL(clicked()), this, SLOT(clicked_btn_mission6()));
	QObject::connect(ui->Btn_mission7, SIGNAL(clicked()), this, SLOT(clicked_btn_mission7()));
	QObject::connect(ui->Btn_mission8, SIGNAL(clicked()), this, SLOT(clicked_btn_mission8()));
	QObject::connect(ui->Btn_mission9, SIGNAL(clicked()), this, SLOT(clicked_btn_mission9()));

    QObject::connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(clicked_btn_sensor()));
	QObject::connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(clicked_btn_driving()));
    QObject::connect(ui->Btn_gearInput, SIGNAL(clicked()), this, SLOT(gear_input()));
    QObject::connect(ui->Btn_up, SIGNAL(clicked()), this, SLOT(clicked_speed_up()));
    QObject::connect(ui->Btn_down, SIGNAL(clicked()), this, SLOT(clicked_speed_down()));
    QObject::connect(ui->Btn_left, SIGNAL(clicked()), this, SLOT(clicked_steer_left()));
    QObject::connect(ui->Btn_right, SIGNAL(clicked()), this, SLOT(clicked_steer_right()));
    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(clicked_E_stop()));

	QObject::connect(ui->Btn_Traffic, SIGNAL(clicked(bool)), this, SLOT(clicked_btn_traffic(bool)));


    // ------------------- UI update for Platform & GPS status ----------------------//


    connect(platformCom, SIGNAL(AorMChanged(int)), this, SLOT(onAorMChanged(int)));
    connect(platformCom, SIGNAL(EStopChanged(int)), this, SLOT(onEStopChanged(int)));
    connect(platformCom, SIGNAL(GearChanged(int)), this, SLOT(onGearChanged(int)));
    connect(platformCom, SIGNAL(SpeedChanged(int)), this, SLOT(onSpeedChanged(int)));
    connect(platformCom, SIGNAL(SteerChanged(int)), this, SLOT(onSteerChanged(int)));
    connect(platformCom, SIGNAL(BreakChanged(int)), this, SLOT(onBreakChanged(int)));
    connect(platformCom, SIGNAL(EncChanged(int)), this, SLOT(onEncChanged(int)));

    connect(gpsCom, SIGNAL(latitudeChanged(double)), this, SLOT(onLatitudeChanged(double)));
    connect(gpsCom, SIGNAL(longitudeChanged(double)), this, SLOT(onLongitudeChanged(double)));

	///connect(missionUpdate, SIGNAL(greenLight(bool)), this, SLOT(onGreenLight(bool)));

 }

// This function is to change comport numbers from CString to QString.
CString ConvertQstringtoCString(QString qs)
{
    std::string utf8_text = qs.toUtf8().constData();
    CString cs(utf8_text.c_str());
    return cs;
}

// This function is to start communication with sensor.
void Ariadne::clicked_btn_sensor() {
	//SENSOR SWITCH

	AutoPortFinder();

	if (!scnnThread->isRunning()) { scnnThread->start(); }

	//if (!yoloThread->isRunning()){ yoloThread->start(); }

	//if(!platformThread->isRunning()) { platformThread->start(); }
	//dataContainer->setValue_UtoP_AorM(1);

	if (!lidarThread->isRunning()) { lidarThread->start(); }

    
	//if (!gpsThread->isRunning()) { gpsThread->start(); }

	TimerSensorStatus = new QTimer(this);
	QTimer::connect(TimerSensorStatus, &QTimer::timeout, this, &Ariadne::updateSensorStatus);
	TimerSensorStatus->start(1000);

}

void Ariadne::onLidarExit()
{
    cout << "onLidarExit is called" << endl;

}

void Ariadne::clicked_lidar_restart() {cout << "no function here" << endl;}
void Ariadne::clicked_lidar_stop() { cout << "no functions here" << endl; }
void Ariadne::clicked_yolo_restart() { yolo->ResumeYolo(); }
void Ariadne::clicked_yolo_stop() { yolo->SuspendYolo(); }
void Ariadne::clicked_scnn_restart() { scnn->ResumeScnn(); }
void Ariadne::clicked_scnn_stop() { scnn->SuspendScnn(); }


// This function is to start driving
void Ariadne::clicked_btn_driving() {
	if (!drivingThread->isRunning())
		drivingThread->start();

	//if (!missionUpdateThread->isRunning())
	//	missionUpdateThread->start();
	//cout << "called in clicked_Btn_driving" << endl;
}

void Ariadne::clicked_btn_mission1() {
	AutoPortFinder();
	dataContainer->setValue_yolo_missionID(PARKING);
	ui->plainTextEdit->appendPlainText("Parking Mission Start");
}

void Ariadne::clicked_btn_mission2() {
	dataContainer->setValue_yolo_missionID(INTER_READY);
	ui->plainTextEdit->appendPlainText("Intersection Ready mission start");
}

void Ariadne::clicked_btn_mission3() {
	dataContainer->setValue_yolo_missionID(INTER_LEFT);
	ui->plainTextEdit->appendPlainText("Intersection left mission start");
}

void Ariadne::clicked_btn_mission4() {
	dataContainer->setValue_yolo_missionID(INTER_RIGHT);
	ui->plainTextEdit->appendPlainText("Intersection right mission start");
}

void Ariadne::clicked_btn_mission5() {
	dataContainer->setValue_yolo_missionID(INTER_STRAIGHT) ;
	ui->plainTextEdit->appendPlainText("intersection straight mission start");
}

void Ariadne::clicked_btn_mission6() {
	dataContainer->setValue_yolo_missionID(INTER_STOP);
	ui->plainTextEdit->appendPlainText("intersection stop mission start");
}

void Ariadne::clicked_btn_mission7() {
	dataContainer->setValue_yolo_missionID(STATIC_OBSTACLE);
	cout << dataContainer->getValue_yolo_missionID() << endl;
	ui->plainTextEdit->appendPlainText("Static Obstacle Mission Start");
}

void Ariadne::clicked_btn_mission8() {
	dataContainer->setValue_yolo_missionID(DYNAMIC_OBSTACLE);
	ui->plainTextEdit->appendPlainText("Dynamic Obstacle Mission Start");
}

void Ariadne::clicked_btn_mission9() {
	dataContainer->setValue_yolo_missionID(BASIC);
	ui->plainTextEdit->appendPlainText("Basic PASIV algorithm start");
}

void Ariadne::clicked_btn_traffic(bool light)
{
	dataContainer->setValue_yolo_go(light);
	cout << "light is on! light :  " << light << endl;
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
void Ariadne::onLatitudeChanged(double Number) { ui->lcdNumber_8->display(Number); }
void Ariadne::onLongitudeChanged(double Number) { ui->lcdNumber_9->display(Number); }
void Ariadne::onGreenLight(bool light) 
{
	/// TODO: 여기서 true가 나오면 ui 버튼도 업데이트할 것.
}

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
			reqSize =0;
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
					dataContainer->setValue_platform_port(port);
				}				
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
	if (dataContainer->getValue_platform_status() > 5) {
		ui->statusPlatform->setStyleSheet("background-color: rgb(0, 153, 76);");
		ui->statusPlatform->setFixedWidth(60);
	}
	else if (dataContainer->getValue_platform_status() > 0) {
		ui->statusPlatform->setStyleSheet("background-color: yellow;"); 
		ui->statusPlatform->setFixedWidth(40);
	}
	else{
		ui->statusPlatform->setStyleSheet("background-color: red;"); 
		ui->statusPlatform->setFixedWidth(20);
	}
	dataContainer->setValue_platform_status(0);

	// LiDAR communication code
	if (dataContainer->getValue_lidar_status() > 5) {
		ui->statusLidar->setStyleSheet("background-color: rgb(0, 153, 76);"); 
		ui->statusLidar->setFixedWidth(60);
	}
	else if (dataContainer->getValue_lidar_status() > 0) {
		ui->statusLidar->setStyleSheet("background-color: yellow;");
		ui->statusLidar->setFixedWidth(40);
	}
	else {
		ui->statusLidar->setStyleSheet("background-color: red;"); 
		ui->statusLidar->setFixedWidth(20);
	}
	dataContainer->setValue_lidar_status(0);

	// GPS communication code
	if (dataContainer->getValue_gps_status() > 5) {
		ui->statusGps->setStyleSheet("background-color: rgb(0, 153, 76);");
		ui->statusGps->setFixedWidth(60);
	}
	else if (dataContainer->getValue_gps_status() > 0) {
		ui->statusGps->setStyleSheet("background-color: yellow;");
		ui->statusGps->setFixedWidth(40);
	}
	else{
		ui->statusGps->setStyleSheet("background-color: red;"); 
		ui->statusGps->setFixedWidth(20);
	}
	dataContainer->setValue_gps_status(0);

	// scnn communication code
	if (dataContainer->getValue_scnn_status() > 5) {
		ui->statusScnn->setStyleSheet("background-color: rgb(0, 153, 76);");
		ui->statusScnn->setFixedWidth(60);
	}
	else if (dataContainer->getValue_scnn_status() > 0) {
		ui->statusScnn->setStyleSheet("background-color: yellow;"); 
		ui->statusScnn->setFixedWidth(40);
	}
	else {
		ui->statusScnn->setStyleSheet("background-color: red;");
		ui->statusScnn->setFixedWidth(20);
	}
	dataContainer->setValue_scnn_status(0);

	// yolo communication code
	if (dataContainer->getValue_yolo_status() > 5) {
		ui->statusYolo->setStyleSheet("background-color: rgb(0, 153, 76);");
		ui->statusYolo->setFixedWidth(60);
	}
	else if (dataContainer->getValue_yolo_status() > 0) {
		ui->statusYolo->setStyleSheet("background-color: yellow;");
		ui->statusYolo->setFixedWidth(40);
	}
	else{
		ui->statusYolo->setStyleSheet("background-color: red;");
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
	
	/// dataContainer->getValue_platform_port()
    if (_platform.OpenPort(dataContainer->getValue_platform_port()))
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

            Sleep(100);
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

bool sign;

CSerialPort _gps;
///void HeroNeverDies();
vector <double>UTM(double lat, double lng);
bool GEOFENCE(double x, double y, vector<vector<double>> map_link, double heading);

GPSCom::GPSCom() {
    ui = Ariadne::getUI();
    dataContainer = DataContainer::getInstance();
    Paint_base();
    Paint_school();
    ui->rt_plot->replot();
}

void GPSCom::Paint_base() // �⺻ �� ����
{
    ui->rt_plot->addGraph();
	ui->rt_plot->graph(0)->rescaleAxes();
    ui->rt_plot->axisRect()->setupFullAxesBox();
}

void GPSCom::Paint_school() {
    ifstream gpsfile("txtfile\\filteredMapSch.txt");   //littleUTM , largeUTM, 30up, 123123, techALL,filteredMapSch
    /// "C:\\Users\\bokyung\\Desktop\\Autonomous\\txtfile\\filteredMapSch.txt"
    char line[200];
    string tap;
    vector<string> vec;

    if (gpsfile.is_open()) {
        while (gpsfile.getline(line, sizeof(line), '\n')) {
            stringstream str(line);

            while (getline(str, tap, '\t')) {  // sch1 >> ' ' , filteredMap, filteredMapsch >> '\t'
                vec.push_back(tap);
            }
            x.push_back(atof(vec[0].c_str()));
            y.push_back(atof(vec[1].c_str()));
            vec.clear();
        }
    }
    gpsfile.close();

    ui->rt_plot->xAxis->setRange(450589, 450700);// range min to max
    ui->rt_plot->yAxis->setRange(3951700, 3951800);

    QCPScatterStyle myScatter2; // �� ��, ������, ������ 5
    myScatter2.setShape(QCPScatterStyle::ssCircle);
    myScatter2.setPen(QPen(Qt::black));
    myScatter2.setSize(5);
    ui->rt_plot->graph(0)->setScatterStyle(myScatter2);
    ui->rt_plot->addGraph();

    ui->rt_plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->rt_plot->graph(0)->setData(x, y);
    ui->rt_plot->replot();
    ui->rt_plot->update();

    ifstream gpsfile1("txtfile\\filteredMapSch_link.csv");
    char line1[200];
    string tap1;
    vector<string> vec1;
    vector<double> vecd;
    if (gpsfile1.is_open()) {

        while (gpsfile1.getline(line1, sizeof(line1), '\n')) {

            stringstream str1(line1);

            while (getline(str1, tap1, ',')) {

                vec1.push_back(tap1);
            }
            vecd.push_back(atof(vec1[0].c_str()));
            vecd.push_back(atof(vec1[1].c_str()));
            map_link.push_back(vecd);
            //cout << map_link[0][1];
            vec1.clear();
            vecd.clear();
        }
    }
    gpsfile1.close();
}

//얘가 실시간 좌표찍는 애임 , 클릭버튼했을 대 이 함수호출되도록했음
void GPSCom::comGPS() { // rt ; Real Time
    QVector<double> temp1;
    QVector<double> temp2;
    QVector<double> store_x;
    QVector<double> store_y;

    cout << "gps communication now" << endl;
	// 
    if (_gps.OpenPort(dataContainer->getValue_gps_port())) {

        _gps.ConfigurePortW(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT);
        _gps.SetCommunicationTimeouts(0, 0, 0, 0, 0);
        string tap;
        string tap2;
        vector<string> vec;

        while (1) {
            BYTE * pByte = new BYTE[128]; // 2028

            if (_gps.ReadByte(pByte, 128)) {

				dataContainer->updateValue_gps_status();

                pByte[127] = '\0'; // 2027

                const char * p = (const char*)pByte;
                //cout << p;

                stringstream str(p);

                while (getline(str, tap, '\n')) {
                    //cout << tap;
                    stringstream qwe(tap);

                    while (getline(qwe, tap2, ',')) {
                        vec.push_back(tap2);
                    }
                    //cout << vec[0] << endl;

                    if (vec.size() > 8) {
                        if (vec[0] == "$GNRMC" && vec[2] == "A") {
                            //ofile << vec[0] << ',' << vec[3] << ',' << vec[5]  << endl;
                            _lat = ((atof(vec[3].c_str()) - 3500) / 60) + 35; // 35랑 128은 상황에 따라 바꿔줘야함
                            _lng = ((atof(vec[5].c_str()) - 12800) / 60) + 128;
                            heading = atof(vec[8].c_str());

                            vector<double >utm = UTM(_lat, _lng);
                            lat = utm[0];
                            lng = utm[1];

							dataContainer->resetValue_gps_valid();
							dataContainer->setValue_gps_heading(heading);
							dataContainer->setValue_gps_latitude(lat);
							dataContainer->setValue_gps_longitude(lng);

                            emit latitudeChanged(lat/1000);
                            emit longitudeChanged(lng/1000); /// 숫자가 너무 커서 나눴음

                            cout << lat << "    " << lng << endl;
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

double L1Distance(vector<double> coor1, vector<double> coor2) {
    double L1 = 0;
    L1 = abs(coor1[0] - coor2[0]) + abs(coor1[1] - coor2[1]);

    return L1;
}

double L2Distance(double x2, double y2, double x1 = 0, double y1 = 0) {
    double L2 = 0;
    L2 = pow(pow(x2 - x1, 2) + pow(y2 - y1, 2), 0.5);

    return L2;
}

vector<int> mins(double x, double y, vector<vector<double>> map_link_cut, vector<double> unitHeading) {

    int min = 0;
    int smin = 0;
    int ssmin = 0;
    int sssmin = 0;
    vector <double> rt_postion{ x,y };
    double temp = 1000000000;

    for (int i = 0; i < map_link_cut.size(); i++) {
        double ref = L1Distance(rt_postion, map_link_cut[i]);
        if (ref <= temp) {
            sssmin = ssmin;
            ssmin = smin;
            smin = min;
            min = i;
            temp = ref;
        }
    }
    int lastPoint = 0;

    if ((unitHeading[0] * (map_link_cut[ssmin][0] - x) + unitHeading[1] * (map_link_cut[ssmin][1]) - y) >= 0) {
        lastPoint = ssmin;
    }
    if ((unitHeading[0] * (map_link_cut[ssmin][0] - x) + unitHeading[1] * (map_link_cut[ssmin][1]) - y) < 0) {
        if ((unitHeading[0] * (map_link_cut[sssmin][0] - x) + unitHeading[1] * (map_link_cut[sssmin][1]) - y) >= 0) {
            lastPoint = sssmin;
        }
        else {
            lastPoint = ssmin;
        }
    }

    vector<int> temp2{ lastPoint, smin, min };
    return temp2;
}


//�������� ���͸� ����
vector<double> makeVector(double x1, double y1, double x2, double y2) {
    double _x = x1 - x2;
    double _y = y1 - y2;
    vector<double> temp{ _x,_y };

    return temp;
}

//���� ����
double outerProduct(double x1, double y1, double x2, double y2) {
    double temp = x1 * y2 - y1 * x2; // 2���� ��������

    // �����̸� ������ �߾��� �����ʹ���, �����̸� ���ʹ���
    return temp;
}

//������ �̿��ؼ� �Ÿ����ϴ� �˰���������
double getDistance(vector<double> a, vector<double> h) {
    vector<double> _d;

    _d.push_back(a[0] - (a[0] * h[0] + a[1] * h[1])*h[0]);
    _d.push_back(a[1] - (a[0] * h[0] + a[1] * h[1])*h[1]);
    //(unitHeading[0], unitHeading[1], vec1[0], vec1[1]);
    double d = outerProduct(h[0], h[1], a[0], a[1]);
    return d;
}

double getError(double x, double y, vector<vector<double>> map_link, vector<double> unitHeading) {

    vector<vector<double>> map_link_cut;

    //map_link���� ������ǥ�� �������� �ȿ� �����ִ� ��ǥ���� ���� ����
    //��, ������ġ�� ���� ����������ŭ �ڸ��� ����
    for (int i = 0; i < map_link.size(); i++) {
        if ((map_link[i][0] >= x - 50 && map_link[i][0] <= x + 50) && (map_link[i][1] >= y - 50 && map_link[i][1] <= y + 50)) {
            map_link_cut.push_back(map_link[i]);
        }
    }

    vector<int> _mins = mins(x, y, map_link_cut, unitHeading); // return {min , smin, ssmin} �ε��� �ѹ�

    //������ ���� ������ ���� ���ͻ���
    vector<double> vec1 = makeVector(map_link_cut[_mins[0]][0], map_link_cut[_mins[0]][1], x, y);
    vector<double> vec2 = makeVector(map_link_cut[_mins[1]][0], map_link_cut[_mins[1]][1], x, y);
    vector<double> vec3 = makeVector(map_link_cut[_mins[2]][0], map_link_cut[_mins[2]][1], x, y);

    //������ ���鿡 ���� �Ÿ�
    double d1 = getDistance(vec1, unitHeading);
    double d2 = getDistance(vec2, unitHeading);
    double d3 = getDistance(vec3, unitHeading);

    double Error = (abs(d1) + abs(d2) + abs(d3)) / 3;

    return Error;
}

// threshole�� 0.9���� ���Ƿ� ����, ���������� �ϴ� 0.9���� ������(�ʿ信 ���� �ٲ� �� ����)
// threshold = 0.9
// if Error > threshold , GEOFENCE = True
// else GEOFENCE = FALUSE
bool GEOFENCE(double x, double y, vector<vector<double>> map_link, double heading) {

    vector<double> unitHeading{ sin(heading*(PI / 180)),cos(heading*(PI / 180)) }; // ������ ���ֺ��� ����

    double Error = getError(x, y, map_link, unitHeading);
    if (Error > 0) {
        sign = true;
    }
    else {
        sign = false;
    }
    if (abs(Error) >= threshold) {
        return true;
    }
    else {
        return false;
    }
}
