#include "Ariadne.h"
#include <iostream>
#include "atlstr.h"

// HyeAhn View와 같은 역할을 함
using namespace std;

Ariadne::Ariadne(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    dataContainer = DataContainer::getInstance();
    platformComThread = new PlatformComThread;
	lidarComThread = new LidarComThread;
	rtkComThread = new RTKComThread;
    ///  -------------------  UI 선택지 넣기 및 꾸미기 ------------------------- ///

	Paint_base();
	Paint_school();
	ui.rt_plot->replot();

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

	lidarComThread->start();

	rtkComThread->start();

	TimerSensorStatus = new QTimer(this);
	QTimer::connect(TimerSensorStatus, &QTimer::timeout, this, &Ariadne::updateSensorStatus);
	TimerSensorStatus->start(1000);
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
		cout << "플랫폼과 연결되었습니다.\n";
    }
    else if (dataContainer->getValue_platform_status() > 0)
    {
        cout << "플랫폼과 통신이 지연되고 있습니다.\n";
    }
    else if (dataContainer->getValue_platform_status() == 0)
    {
        cout << "플랫폼과 통신이 실패하였습니다.\n";
    }
	dataContainer->setValue_platform_status(0);

    // LiDAR 연결 상태
    if (dataContainer->getValue_lidar_status() > 5)
    {
        cout << "라이다와 연결되었습니다.\n";
    }
    else if (dataContainer->getValue_lidar_status() > 0)
    {
        cout << "라이다와 통신이 지연되고 있습니다.\n";
    }
    else if (dataContainer->getValue_lidar_status() == 0)
    {
        cout << "라이다와 통신이 실패하였습니다.\n";
    }

    dataContainer->setValue_lidar_status(0);

	/*
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
	*/

    // GPS 연결상태
    if (dataContainer->getValue_gps_status() > 5)
    {
        cout << "GPS와 연결되었습니다.\n";
    }
    else if (dataContainer->getValue_gps_status() > 0)
    {
        cout << "GPS와 통신이 지연되고 있습니다.\n";
    }
    else if (dataContainer->getValue_gps_status() == 0)
    {
        cout << "GPS와 통신이 실패하였습니다.\n";
    }
	dataContainer->setValue_gps_status(0);
}

#define threshold 0.5
#define PI 3.14159265358979323846
#define radiRatio 1 / 298.257223563
#define lamda 129.0*PI/180.0
#define Eoff 500000
#define k0 0.9996
#define radi 6378137

bool sign; // 외적의 부호 , true =양수, false = 음수

CSerialPort _gps;
vector <double>UTM(double lat, double lng);
bool GEOFENCE(double x, double y, vector<vector<double>> map_link, double heading);


void Ariadne::Paint_base() // 기본 축 설정
{
	ui.rt_plot->addGraph();
	ui.rt_plot->graph(0)->rescaleAxes();
	ui.rt_plot->axisRect()->setupFullAxesBox();
}

void Ariadne::Paint_school() {
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
	
	ui.rt_plot->xAxis->setRange(450589, 450700);// range min to max // 상하좌우 20
	ui.rt_plot->yAxis->setRange(3951700, 3951800);

	QCPScatterStyle myScatter2; // 빈 원, 검은색, 사이즈 5
	myScatter2.setShape(QCPScatterStyle::ssCircle);
	myScatter2.setPen(QPen(Qt::black));
	myScatter2.setSize(5);
	ui.rt_plot->graph(0)->setScatterStyle(myScatter2);
	ui.rt_plot->addGraph();

	ui.rt_plot->graph(0)->setLineStyle(QCPGraph::lsNone);
	ui.rt_plot->graph(0)->setData(x, y);
	ui.rt_plot->replot();
	ui.rt_plot->update();

	ifstream gpsfile1("C:\\Qt\\txtfile\\filteredMapSch_link.csv");
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


void HeroNeverDies() {
	_gps.ClosePort();
	if (_gps.OpenPort(L"COM4")) {
		_gps.ConfigurePortW(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT);
		_gps.SetCommunicationTimeouts(0, 0, 0, 0, 0);
	}
}

void Ariadne::Paint_school_rt() {
	QVector<double> temp1;
	QVector<double> temp2;
	QVector<double> store_x;
	QVector<double> store_y;
	if (_gps.OpenPort(L"COM4")) {

		_gps.ConfigurePortW(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT);
		_gps.SetCommunicationTimeouts(0, 0, 0, 0, 0);

		string tap;
		string tap2;
		vector<string> vec;

		while (1) {
			BYTE * pByte = new BYTE[2028];

			if (_gps.ReadByte(pByte, 2028)) {
				pByte[2027] = '\0';

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

							temp1.push_back(lat);
							temp2.push_back(lng);
							//store_x.push_back(lat);
							//store_y.push_back(lng);
							ui.rt_plot->xAxis->setRange(lat - 10, lat + 10);// range min to max // 상하좌우 20
							ui.rt_plot->yAxis->setRange(lng - 10, lng + 10);  //

							QCPScatterStyle myScatter4; //꽉찬 원, 빨간색, 사이즈 10
							myScatter4.setShape(QCPScatterStyle::ssDisc);
							myScatter4.setPen(QPen(Qt::red));
							myScatter4.setSize(5);
							ui.rt_plot->graph(1)->setScatterStyle(myScatter4);
							ui.rt_plot->addGraph();
							ui.rt_plot->graph(1)->setLineStyle(QCPGraph::lsNone);
							ui.rt_plot->graph(1)->setData(temp1, temp2);
							ui.rt_plot->replot();
							ui.rt_plot->update();
							temp1.clear();
							temp2.clear();

							//QCPScatterStyle myScatter5; //엑스표시, 빨간색, 사이즈 10
							//myScatter5.setShape(QCPScatterStyle::ssCross);
							//myScatter5.setPen(QPen(Qt::red));
							//myScatter5.setSize(5);
							//ui->rt_plot->graph(2)->setScatterStyle(myScatter5);
							//ui->rt_plot->addGraph();
							//ui->rt_plot->graph(2)->setLineStyle(QCPGraph::lsNone);
							//ui->rt_plot->graph(2)->setData(store_x, store_y);
							//ui->rt_plot->replot();
							//ui->rt_plot->update();

							//Geofence = GEOFENCE(lat,lng,map_link,heading);
							//if (Geofence ==false){
							//    GF = "safe" ;
							//}
							//else{
							//    GF = "danger";
							//}

							//ui->label->setText(GF);
						}
					}
					vec.clear();
				}
			}
			else {
				HeroNeverDies();
			}

		}
	}
}

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

// L1방식의 거리입니다. (L1 = |x1-x2| + |y1-y2|) 우리가 일반적으로 알고있는 거리구하는 공식은 L2입니다.
// L1방식으로 거리를 구하는 이유는 연산량이 매우 적기 때문에 이용했습니다.
double L1Distance(vector<double> coor1, vector<double> coor2) { // 가까운 세점 찾을 때 이용하는 거리
	double L1 = 0;
	L1 = abs(coor1[0] - coor2[0]) + abs(coor1[1] - coor2[1]);

	return L1;
}

double L2Distance(double x2, double y2, double x1 = 0, double y1 = 0) { // 헤딩 연장선과 가까운 점(3개)사이의 거리 구할 떄
	double L2 = 0;
	L2 = pow(pow(x2 - x1, 2) + pow(y2 - y1, 2), 0.5);

	return L2;
}


// 가장 가까운 세점 찾는 함수 (무조건 현재좌표에서 앞에 2개의 점, 뒤에 1개의 점이있어야함 왜냐하면 GEOFENCE를 완벽하게 구현하기위해)
vector<int> mins(double x, double y, vector<vector<double>> map_link_cut, vector<double> unitHeading) {

	//min, smin ...은 가장 가까운 점의 인덱스 넘버임
	int min = 0;
	int smin = 0;
	int ssmin = 0;
	int sssmin = 0;
	vector <double> rt_postion{ x,y };
	double temp = 1000000000;


	//가장 가까운 4개의 점을 찾은 후 제일 가까운 2점은 먼저 저장하고
	//3,4번째에 해당하는 점들은 해딩벡터와 내적해서 양수의 값을 이용함
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
	int lastPoint = 0; // 3,4번째 점에서 벡터 내적이 양수인 값

	//3,4번째 점에서 벡터 내적을 구해 양수인 값 판별 후 저장
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


//두점으로 벡터를 만듦
vector<double> makeVector(double x1, double y1, double x2, double y2) {
	double _x = x1 - x2;
	double _y = y1 - y2;
	vector<double> temp{ _x,_y };

	return temp;
}

//외적 공식
double outerProduct(double x1, double y1, double x2, double y2) {
	double temp = x1 * y2 - y1 * x2; // 2차원 외적공식

	// 양수이면 차선의 중앙의 오른쪽방향, 음수이면 왼쪽방향
	return temp;
}

//외적을 이용해서 거리구하는 알고리즘적용
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

	//map_link에서 현재좌표의 일정범위 안에 들어있는 좌표들을 먼저 구함
	//즉, 현재위치에 대해 일정범위만큼 자르는 행위
	for (int i = 0; i < map_link.size(); i++) {
		if ((map_link[i][0] >= x - 50 && map_link[i][0] <= x + 50) && (map_link[i][1] >= y - 50 && map_link[i][1] <= y + 50)) {
			map_link_cut.push_back(map_link[i]);
		}
	}

	vector<int> _mins = mins(x, y, map_link_cut, unitHeading); // return {min , smin, ssmin} 인덱스 넘버

	//가까운 점들 각각에 대해 벡터생성
	vector<double> vec1 = makeVector(map_link_cut[_mins[0]][0], map_link_cut[_mins[0]][1], x, y);
	vector<double> vec2 = makeVector(map_link_cut[_mins[1]][0], map_link_cut[_mins[1]][1], x, y);
	vector<double> vec3 = makeVector(map_link_cut[_mins[2]][0], map_link_cut[_mins[2]][1], x, y);

	//가까운 점들에 대한 거리
	double d1 = getDistance(vec1, unitHeading);
	double d2 = getDistance(vec2, unitHeading);
	double d3 = getDistance(vec3, unitHeading);

	double Error = (abs(d1) + abs(d2) + abs(d3)) / 3;

	return Error;
}

// threshole가 0.9라고 임의로 설정, 시행착오로 일단 0.9라고 지정함(필요에 의해 바꿀 수 있음)
// threshold = 0.9
// if Error > threshold , GEOFENCE = True
// else GEOFENCE = FALUSE
bool GEOFENCE(double x, double y, vector<vector<double>> map_link, double heading) {

	vector<double> unitHeading{ sin(heading*(PI / 180)),cos(heading*(PI / 180)) }; // 헤딩의 유닛벡터 만듦

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


void RTKComThread::comRTK() {
	Ariadne ariadne;
	ariadne.Paint_school_rt();
}


void RTKComThread::run() {
	cout << "RTK 스레드가 생성되었습니다.\n";
	comRTK();
}