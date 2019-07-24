#include "Ariadne.h"
#include <iostream>
#include "atlstr.h"
#include "qevent.h"

// HyeAhn View�� ���� ������ ��
using namespace std;

Ui::AriadneClass* Ariadne::ui = new Ui::AriadneClass;

Ariadne::Ariadne(QWidget *parent)
    : QMainWindow(parent)
{
    ui->setupUi(this);

    dataContainer = DataContainer::getInstance();

    platformCom = new PlatformCom;
	platformThread = new QThread;
	platformCom->moveToThread(platformThread);
	connect(platformThread, SIGNAL(started()), platformCom, SLOT(comPlatform()));
	connect(platformCom, SIGNAL(PlatformExit()), platformCom, SLOT(comPlatform()));

	lidarCom = new LidarCom;
	lidarThread = new QThread;
	lidarCom->moveToThread(lidarThread);
	connect(lidarThread, SIGNAL(started()), lidarCom, SLOT(comLidar()));
	connect(lidarCom, SIGNAL(LidarExit()), lidarCom, SLOT(comLidar()));

	rtkCom = new RTKCom;
	rtkThread = new QThread;
	rtkCom->moveToThread(rtkThread);
	connect(rtkThread, SIGNAL(started()), rtkCom, SLOT(comRTK()));
	connect(rtkCom, SIGNAL(RTKExit()), rtkCom, SLOT(comRTK()));

	scnn = new Scnn;
	scnnThread = new QThread;
	scnn->moveToThread(scnnThread);
	connect(scnnThread, SIGNAL(started()), scnn, SLOT(comScnn()));

    ///  -------------------  UI ������ �ֱ� �� �ٹ̱� ------------------------- ///

    for (int i = 1; i < 7; i++) // combobox�� ������ �ֱ�. �ش� ���������� ���� ���ſ� ����Ʈ�� ������.
    {
        ui->comboBox->addItem("COM" + QString::number(i));
        ui->comboBox_2->addItem("COM" + QString::number(i));
        ui->comboBox_3->addItem("COM" + QString::number(i));
        ui->comboBox_4->addItem("COM" + QString::number(i));
        ui->comboBox_5->addItem("COM" + QString::number(i));
    } // combobox ����: https://www.bogotobogo.com/Qt/Qt5_QComboBox.php

    ui->comboBox_6->addItems({ "front" , "JoongLib", "HooJin"}); /// gaer ������ input

    /// --------------------- Qobject�� ��ư connect ---------------------------------///

    QObject::connect(ui->Btn_Mission0, SIGNAL(clicked()), this, SLOT(clicked_btn_mission0()));
    QObject::connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(clicked_btn_confirm()));
    QObject::connect(ui->Btn_gearInput, SIGNAL(clicked()), this, SLOT(gear_input()));
    QObject::connect(ui->Btn_up, SIGNAL(clicked()), this, SLOT(clicked_speed_up()));
    QObject::connect(ui->Btn_down, SIGNAL(clicked()), this, SLOT(clicked_speed_down()));
    QObject::connect(ui->Btn_left, SIGNAL(clicked()), this, SLOT(clicked_steer_left()));
    QObject::connect(ui->Btn_right, SIGNAL(clicked()), this, SLOT(clicked_steer_right()));
    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(clicked_E_stop()));

    /// ------------------- thread and signals for UI update ----------------------///
    connect(platformCom, SIGNAL(AorMChanged(int)), this, SLOT(onAorMChanged(int)));
    connect(platformCom, SIGNAL(EStopChanged(int)), this, SLOT(onEStopChanged(int)));
    connect(platformCom, SIGNAL(GearChanged(int)), this, SLOT(onGearChanged(int)));
    connect(platformCom, SIGNAL(SpeedChanged(int)), this, SLOT(onSpeedChanged(int)));
    connect(platformCom, SIGNAL(SteerChanged(int)), this, SLOT(onSteerChanged(int)));
    connect(platformCom, SIGNAL(BreakChanged(int)), this, SLOT(onBreakChanged(int)));
    connect(platformCom, SIGNAL(EncChanged(int)), this, SLOT(onEncChanged(int)));

	//connect(platformCom, SIGNAL(PlatformExit()), this, SLOT(onPlatformExit()));
	//connect(lidarCom, SIGNAL(LidarExit()), this, SLOT(onLidarExit()));
	//connect(rtkCom, SIGNAL(RTKExit()), this, SLOT(onRTKExit()));
}

// comport�� ���� ������ CString�� �̿��ǹǷ� QString�� CString���� �ٲ��ִ� �Լ��� ��������.
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
    Temp = ui->comboBox->currentText();
    std::wcout << (const wchar_t*)ConvertQstringtoCString(Temp) << std::endl;

    QString Temp1, Temp2, Temp3, Temp4, Temp5;
    Temp1 = ui->comboBox->currentText();
    Temp2 = ui->comboBox_2->currentText();
    Temp3 = ui->comboBox_3->currentText();
    Temp4 = ui->comboBox_4->currentText();
    Temp5 = ui->comboBox_5->currentText();

    ui->plainTextEdit->appendPlainText("I love you");
    ui->plainTextEdit->appendPlainText("I love you very much");
    ui->plainTextEdit->appendPlainText("will you going on a date with me?");

    //platformComThread->run(ConvertQstringtoCString(Temp1));
    ui->plainTextEdit->appendPlainText("I thanks you so much How can I appreciate it?");

	if (!scnnThread->isRunning())
		scnnThread->start();

	if(!platformThread->isRunning())
		platformThread->start();

	if(!lidarThread->isRunning())
		lidarThread->start();

	if(!rtkThread->isRunning())
		rtkThread->start();

	TimerSensorStatus = new QTimer(this);
	QTimer::connect(TimerSensorStatus, &QTimer::timeout, this, &Ariadne::updateSensorStatus);
	TimerSensorStatus->start(1000);

}

void Ariadne::clicked_btn_mission0() {
    //mission.doMission0();
}


void Ariadne::gear_input() /// gear input ��ư�� ������ �ش� combobox�� ���뿡 �°� gear value�� ������
{
    QString qs;
    qs = ui->comboBox_6->currentText();
    if (qs == "front") { dataContainer->setValue_UtoP_GEAR(0); }
    else if (qs == "JoongLib") { dataContainer->setValue_UtoP_GEAR(1); }
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

///  _    _ _____    _    _ _____  _____       _______ ______
/// | |  | |_   _|  | |  | |  __ \|  __ \   /\|__   __|  ____|
/// | |  | | | |    | |  | | |__) | |  | | /  \  | |  | |__
/// | |  | | | |    | |  | |  ___/| |  | |/ /\ \ | |  |  __|
/// | |__| |_| |_   | |__| | |    | |__| / ____ \| |  | |____
///  \____/|_____|   \____/|_|    |_____/_/    \_\_|  |______|
// this part is for UI updating functions; display, slots... etc

void Ariadne::onAorMChanged(int Number)
{
    ui->lcdNumber->display(Number);
}

void Ariadne::onEStopChanged(int Number)
{
    ui->lcdNumber_2->display(Number);
}

void Ariadne::onGearChanged(int Number)
{
    ui->lcdNumber_3->display(Number);
}

void Ariadne::onSpeedChanged(int Number)
{
    ui->lcdNumber_4->display(Number);
}

void Ariadne::onSteerChanged(int Number)
{
    ui->lcdNumber_5->display(Number);
}

void Ariadne::onBreakChanged(int Number)
{
    ui->lcdNumber_6->display(Number);
}

void Ariadne::onEncChanged(int Number)
{
    ui->lcdNumber_7->display(Number);
}

/// TODO: GPS location display �߰�

void Ariadne::updateSensorStatus()
{
    using namespace std;

    DataContainer *dataContainer;
    dataContainer = DataContainer::getInstance();

    if (dataContainer->getValue_platform_status() > 5)
    {
		//cout << "�÷����� �����Ǿ����ϴ�.\n";
    }
    else if (dataContainer->getValue_platform_status() > 0)
    {
        //cout << "�÷����� ������ �����ǰ� �ֽ��ϴ�.\n";
    }
    else if (dataContainer->getValue_platform_status() == 0)
    {
        //cout << "�÷����� ������ �����Ͽ����ϴ�.\n";
    }
	dataContainer->setValue_platform_status(0);

    // LiDAR ���� ����
    if (dataContainer->getValue_lidar_status() > 5)
    {
        //cout << "���̴ٿ� �����Ǿ����ϴ�.\n";
    }
    else if (dataContainer->getValue_lidar_status() > 0)
    {
        //cout << "���̴ٿ� ������ �����ǰ� �ֽ��ϴ�.\n";
    }
    else if (dataContainer->getValue_lidar_status() == 0)
    {
        //cout << "���̴ٿ� ������ �����Ͽ����ϴ�.\n";
    }

    dataContainer->setValue_lidar_status(0);

	/*
    // CAMERA1 ���� ����
    if (dataContainer->getValue_camera1_status() > 5)
    {
        cout << "ī�޶�1�� �����Ǿ����ϴ�." << endl;
    }
    else if (dataContainer->getValue_camera1_status() > 0)
    {
        cout << "ī�޶�1�� ������ �����ǰ� �ֽ��ϴ�." << endl;
    }
    else if (dataContainer->getValue_camera1_status() == 0)
    {
        cout << "ī�޶�1�� ������ �����Ͽ����ϴ�." << endl;
    }
	dataContainer->setValue_camera1_status(0);
	*/

    // GPS ��������
    if (dataContainer->getValue_gps_status() > 5)
    {
        //cout << "GPS�� �����Ǿ����ϴ�.\n";
    }
    else if (dataContainer->getValue_gps_status() > 0)
    {
        //cout << "GPS�� ������ �����ǰ� �ֽ��ϴ�.\n";
    }
    else if (dataContainer->getValue_gps_status() == 0)
    {
        //cout << "GPS�� ������ �����Ͽ����ϴ�.\n";
    }
	dataContainer->setValue_gps_status(0);
}

Ui::AriadneClass* Ariadne::getUI() { return ui;}

#define threshold 0.5
#define PI 3.14159265358979323846
#define radiRatio 1 / 298.257223563
#define lamda 129.0*PI/180.0
#define Eoff 500000
#define k0 0.9996
#define radi 6378137
#define COM L"COM5" // GPS comport

bool sign; // ������ ��ȣ , true =����, false = ����

CSerialPort _gps;
void HeroNeverDies();
vector <double>UTM(double lat, double lng);
bool GEOFENCE(double x, double y, vector<vector<double>> map_link, double heading);

RTKCom::RTKCom() {
	ui = Ariadne::getUI();
	dataContainer = DataContainer::getInstance();
	//Paint_base();
	//Paint_school();
	//ui->rt_plot->replot();
}

void RTKCom::Paint_base() // �⺻ �� ����
{
	ui->rt_plot->addGraph();
	ui->rt_plot->graph(0)->rescaleAxes();
	ui->rt_plot->axisRect()->setupFullAxesBox();
}

void RTKCom::Paint_school() {
	ifstream gpsfile("C:\\Users\\bokyung\\Desktop\\Autonomous\\txtfile\\filteredMapSch.txt");   //littleUTM , largeUTM, 30up, 123123, techALL,filteredMapSch

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

	ui->rt_plot->xAxis->setRange(450589, 450700);// range min to max // �����¿� 20
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

	ifstream gpsfile1("C:\\Users\\bokyung\\Desktop\\Autonomous\\txtfile\\filteredMapSch_link.csv");
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

 void RTKCom::comRTK() {
	 cout << "rtk start\n";

	 if (_gps.OpenPort(L"COM5")) {

		_gps.ConfigurePortW(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT);
		_gps.SetCommunicationTimeouts(0, 0, 0, 0, 0);

		while (1) {
            cout << "in RTK" << endl;
			BYTE*pByte = new BYTE[128];

			if (_gps.ReadByte(pByte, 128)) {

				dataContainer->updateValue_gps_status();

				pByte[127] = '\0';
				char*p = (char*)pByte;

				char* context = NULL;
				char* tok = strtok_s(p, "$", &context);
				tok = strtok_s(NULL, "$", &context);

				char* tok2 = strtok_s(tok, ",", &context);
				if (tok2[2] == 'R' && tok2[3] == 'M' && tok2[4] == 'C') {
					string temp;
					vector<string> list;

					for (int i = 0; i < strlen(context); i++) {
						if (context[i] != ',') {
							temp += context[i];
						}
						else {
							list.push_back(temp);
							temp.clear();
						}
					}

					if (list.at(1) == "V") {
						dataContainer->count_gps_valid();
					}
					else if (list.at(1) == "A") {
						dataContainer->resetValue_gps_valid();
						dataContainer->setValue_gps_latitude(atof(list.at(2).c_str()));
						dataContainer->setValue_gps_longitude(atof(list.at(4).c_str()));
						dataContainer->setValue_gps_velocity(atof(list.at(6).c_str()));
						dataContainer->setValue_gps_heading(atof(list.at(7).c_str()));
					}
				}
			}
			else {
				_gps.ClosePort();
				emit(RTKExit());
				return;
			}
		}
	 }
	 else {
		 cout << "rtk not connect\n";
		 emit(RTKExit());
		 return;
	 }
}


/*
void HeroNeverDies() {
	_gps.ClosePort();
	if (_gps.OpenPort(COM)) {
		_gps.ConfigurePortW(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT);
		_gps.SetCommunicationTimeouts(0, 0, 0, 0, 0);
	}
}
*/

vector <double>UTM(double lat, double lng) {
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

	//fuck offset
	double N = T1 + pow(dellng, 2)*T2 + pow(dellng, 4)*T3 + pow(dellng, 6)*T4 + pow(dellng, 8)*T5 + 1.32;
	double E = Eoff + dellng * T6 + pow(dellng, 3)*T7 + pow(dellng, 5)*T8 + pow(dellng, 7)*T9;

	vector<double> utm;
	utm.push_back(E);
	utm.push_back(N);
	return utm;
}

// L1������ �Ÿ��Դϴ�. (L1 = |x1-x2| + |y1-y2|) �츮�� �Ϲ������� �˰��ִ� �Ÿ����ϴ� ������ L2�Դϴ�.
// L1�������� �Ÿ��� ���ϴ� ������ ���귮�� �ſ� ���� ������ �̿��߽��ϴ�.
double L1Distance(vector<double> coor1, vector<double> coor2) { // ������ ���� ã�� �� �̿��ϴ� �Ÿ�
	double L1 = 0;
	L1 = abs(coor1[0] - coor2[0]) + abs(coor1[1] - coor2[1]);

	return L1;
}

double L2Distance(double x2, double y2, double x1 = 0, double y1 = 0) { // ���� ���弱�� ������ ��(3��)������ �Ÿ� ���� ��
	double L2 = 0;
	L2 = pow(pow(x2 - x1, 2) + pow(y2 - y1, 2), 0.5);

	return L2;
}


// ���� ������ ���� ã�� �Լ� (������ ������ǥ���� �տ� 2���� ��, �ڿ� 1���� �����־����� �ֳ��ϸ� GEOFENCE�� �Ϻ��ϰ� �����ϱ�����)
vector<int> mins(double x, double y, vector<vector<double>> map_link_cut, vector<double> unitHeading) {

	//min, smin ...�� ���� ������ ���� �ε��� �ѹ���
	int min = 0;
	int smin = 0;
	int ssmin = 0;
	int sssmin = 0;
	vector <double> rt_postion{ x,y };
	double temp = 1000000000;


	//���� ������ 4���� ���� ã�� �� ���� ������ 2���� ���� �����ϰ�
	//3,4��°�� �ش��ϴ� ������ �ص����Ϳ� �����ؼ� ������ ���� �̿���
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
	int lastPoint = 0; // 3,4��° ������ ���� ������ ������ ��

	//3,4��° ������ ���� ������ ���� ������ �� �Ǻ� �� ����
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
