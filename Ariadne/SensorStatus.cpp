#include "SensorStatus.h"
#include "ScnnFunc.h"
#include <tchar.h>
#include "missionTrigger.h"
#include "Utils.hpp"

#define UPDATE_PLATFORM_STATUS 100
#define UPDATE_SENSOR_CONNECTION 101
#define UPDATE_SENSOR_STATUS 102
#define UPDATE_SENSOR_AUTOSTARTUP 103

/// for Scnn: this part is run by boosted python: scnn python program is included in Ariadne program
/// for more information: last line in this cpp file
#define BOOST_PYTHON_STATIC_LIB
//#define BOOST_LIB_NAME "boost_numpy35"
//#include <boost/config/auto_link.hpp>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <iostream>
#include <boost/python/stl_iterator.hpp>

namespace py = boost::python;
namespace np = boost::python::numpy;

template< typename T >
std::vector< T > to_std_vector(const py::object& iterable)
{
	return std::vector< T >(py::stl_input_iterator< T >(iterable),
		py::stl_input_iterator< T >());
}

/// ////////////////////////////////////////////////////////////////////

LidarCom::LidarCom() {
	dataContainer = DataContainer::getInstance();
}


int LidarCom::comLidar() {
	LastOfLiDAR lol;

	/**/
	if (!lol.Initialize()) {
		cout << "Connect ERROR!!!" << endl;
		return -1;
	}

	lol.StartCapture();

	//Mat picLiDAR = imread("maps/intersection1.png", 0); // exeption error: 파일 이름 잘못된거임

	// 교차로 1번 경로를 위한 시작 및 종착점 설정
	//int startX = MAPX / 2, startY = MAPY - BOT_L, startTheta = 305, startSteer = 0, startSpeed = 60; // It will be multiplied by 5, so 18 is 90 degree
	//int targetX = MAPX / 2, targetY = BOT_L, targetTheta = 270, targetSteer = 0, targetSpeed = 60;

	while (1) {
		if (lol.m_bDataAvailable) { // lol.m_bDataAvailable
			/**/
			dataContainer->updateValue_lidar_status();

			Mat M(MAPY, MAPX, CV_8UC3, Scalar::all(255));
			lol.imgLiDAR = M; //default == 2160,3840
			lol.GetValidDataRTheta(lol.validScans);
			lol.Conversion(lol.validScans, lol.finQVecXY);
			lol.Average(lol.finQVecXY, lol.finVecXY);
			lol.Clustering(lol.finVecXY, lol.finObjData);
			lol.Vector(lol.finObjData, lol.finVecData, lol.finBoolData);

			dataContainer->setValue_lidar_BoolData(lol.finBoolData);
			dataContainer->setValue_lidar_Data(lol.finObjData);
			dataContainer->setValue_lidar_VecXY(lol.finVecXY);
			dataContainer->setValue_lidar_VecData(lol.finVecData);


			//dataContainer->setValue_lidar_ImgData(lol.imgLiDAR);
			//lol.DrawData(lol.finVecXY, lol.finObjData, lol.finVecData, lol.finBoolData, lol.imgLiDAR);

			//cout << "Reset" << endl;

			//imshow("DrawLiDARData", lol.imgLiDAR);

		}

		int key = waitKey(1);

		if (key == 27) {
			break;
		}
	}

	lol.StopCapture();
	lol.UnInitialize();

	return 0;
}

Scnn::Scnn() {
	dataContainer = DataContainer::getInstance();
}

/// this function is for ruuning python scnn program in Ariadne
int Scnn::boostScnn() {

	using namespace std;

	while (dataContainer->getValue_yolo_status() == 0) { cout << "yolo not start\n";  Sleep(500); }

	Py_SetPythonHome(L"C:/Users/D-Ace/.conda/envs/py35");

	Py_Initialize();
	np::initialize();

	py::object main_module = py::import("__main__");
	py::object main_namespace = main_module.attr("__dict__");
	py::object print = py::import("__main__").attr("__builtins__").attr("print");


	//py::exec("import simple", main_namespace);
	py::exec("import scnn_c_implementation_v2", main_namespace);

	py::object scnn = py::import("scnn_c_implementation_v2");


	//scnn.attr("scnn_init")("C:/Users/D-Ace/Documents/Ariadne/Ariadne/exp1_kcity_best_50.pth", 0, true);
	scnn.attr("scnn_init")("C:/Users/D-Ace/Documents/Ariadne/Ariadne/kcity_aug2_crop_pass4.pth", 1, true);
	//scnn.attr("scnn_init")("C:/Users/D-Ace/Documents/Ariadne/Ariadne/kcity_aug2_crop_pass4.pth", "C:/Users/D-Ace/Documents/Ariadne/Ariadne/test2.mp4", true);
	//scnn.attr("scnn_init")("C:/Users/D-Ace/Documents/Ariadne/Ariadne/kcity_big_crop_pass4_best.pth", 0, true);
	//scnn.attr("scnn_init")("C:/Users/D-Ace/Documents/Ariadne/Ariadne/kcity_big_crop_pass4_best.pth", "C:/Users/D-Ace/Documents/Ariadne/Ariadne/test2.mp4", true);
	//scnn.attr("scnn_init")("C:/Users/D-Ace/Documents/Ariadne/Ariadne/kcity_aug_crop_pass4.pth", "C:/Users/D-Ace/Documents/Ariadne/Ariadne/test2.mp4", true);
	//scnn.attr("scnn_init")("C:/Users/D-Ace/Documents/Ariadne/Ariadne/k_city_crop_exp1_best_pass4.pth", "C:/Users/D-Ace/Pictures/Camera Roll/4.mp4", true);

	//scnn.attr("scnn_init")("exp1_kcity_best_50.pth", 0, true);

	emit(drivingEnabled());
	while (1)
	{
		dataContainer->updateValue_scnn_status();

		py::object params = scnn.attr("scnn_run")();
		vector<int> lists = to_std_vector<int>(params);
		vector<int> existLanes;
		vector<vector<Point2i>> lanes(4);

		int i, j = 0;

		for (i = 0; i < 4; i++) {
			existLanes.push_back(lists[i]);
		}

		for (i = 5; i < lists.size(); i++) {
			if (lists[i] == -1) {
				j++;
			}
			else {
				cv::Point2i coordinate(lists[i], lists[i + 1]);
				lanes[j].push_back(coordinate);
				i++;
			}
		}

		dataContainer->setValue_scnn_lanes(lanes);
		dataContainer->setValue_scnn_existLanes(existLanes);


		
		/*cout << "existLanes : ";

		for (i = 0; i < 4; i++)
		{
			cout << lists[i] << " ";
		}
		cout << endl;

		cout << "existLanes : ";
		for (i = 0; i < 4; i++)
		{
			cout << existLanes[i] << " ";
		}
		cout << endl;

		cout << "existLanes : ";
		for (i = 0; i < 4; i++)
		{
			cout << dataContainer->getValue_scnn_existLanes()[i] << " ";
		}
		cout << endl;
		
		for (int i = 0; i < 4; i++)
		{
			cout << "lane " << i << " : ";
			for (int j = 0; j < dataContainer->getValue_scnn_lanes()[i].size(); j++) {
				cout << dataContainer->getValue_scnn_lanes()[i][j] << " ";
			}
		cout << endl;
		}*/
		existLanes.clear();
		lanes.clear();
	}

	scnn.attr("scnn_destroy")();

	return 0;
}

/// this function is for TCP/IP communication by WinSOCK: not used now.
void Scnn::comScnn() {

	ResumeThread(tid);

	client = accept(server, NULL, NULL); /// 누군가 나에게 연결할때까지 기다리겠다 <- 연결을 확실히 시도했다고 들 때
	/// 잘못 썼다가 Ariadne가 멈출 수도 있음. cout이 안뜬다 이러면 accept가 잘못되었을 가능성이 있음

	char message[5000];
	int strLen;
	int n;
	int i;
	while (1) {
		ZeroMemory(&message, sizeof(message));
		strLen = recv(client, (char*)message, sizeof(message) - 1, 0);
		/// 받아오는 메시지의 사이즈
		//n = atoi(message);
		cout << message << endl;
		vector<vector<cv::Point2i>> lanes(4);
		vector<int> existLanes(4); /// 레인이 있는지 없는지
		stringstream in; /// string으로 받아옴 띄어쓰기 기준으로 -> 뭐 하나 오고 띄우고 띄우고 띄우고
		in.str(message);
		for (i = 0; i < 4; i++) {
			int n;
			in >> n;
			cout << n << endl;

			for (int j = 0; j < n; j++) {
				int x, y;
				in >> x >> y;
				lanes[i].push_back(cv::Point2i(x, y));
			}
		}
		for (i = 0; i < 4; i++) {
			in >> existLanes[i];
			cout << existLanes[i] << endl;; /// lane이 4개 들어오는데 왼쪽부터 차선에 있는 점을 줌.
			/// 벡터 하나가 한 차선. 벡터 4개 받음
		}

		dataContainer->setValue_scnn_lanes(lanes);
		dataContainer->setValue_scnn_existLanes(existLanes);
	}
	closesocket(client); /// 통신 끊음
	closesocket(server); /// 통신 끊음
}

void Scnn::SuspendScnn() {
	SuspendThread(tid);
}

void Scnn::ResumeScnn() {
	ResumeThread(tid);
}

Yolo::Yolo() {
	dataContainer = DataContainer::getInstance();

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		cout << "error\n";

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi)); // assign program memory

	//TCHAR commandLine[] = TEXT("darknet.exe detector demo data\\obj.data cfg\\yolov3_please.cfg yolov3_53000_0905_2.weights");
	TCHAR commandLine[] = TEXT("darknet.exe detector demo data\\obj.data cfg\\yolov3_please.cfg yolov3_73250.weights");
	SetCurrentDirectory(_T("C:\\Users\\D-Ace\\darknet-master\\build\\darknet\\x64")); // Darknet program start command
	//if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
	//}
	
	if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		cout << "yolo create fail\n";
	}
	else {
		cout << "yolo create succese\n";
		tid = pi.hThread;
		SuspendThread(tid);
	}
	

	server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(800);

	if (::bind(server, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		cout << "binding fail\n";

	if (listen(server, SOMAXCONN) == SOCKET_ERROR)
		cout << "listening fail\n";

}

void Yolo::comYolo() {
	//
	// To Do : integraiting
	//
	//SuspendThread(tid);
	ResumeThread(tid);

	client = accept(server, NULL, NULL);
	if (client == INVALID_SOCKET)
		cout << "invalid socket\n";

	char message[50];
	int strLen;
	float* data;
	cout << "communication1\n";

	static MissionContainer trigger;

	/// TODO: control yolo flag in Ariadne.cpp
	while (1) {


		strLen = recv(client, message, 50, 0);
		if (strLen == -1) cout << "send error\n";

		int* trigger = new int;
		trigger = (int*)message;

		// 0: green
		// 1: green left
		// 2: red
		// 3: red left
		// 4: red yellow
		// 5: yellow


//{ "intersectionState", "intersectionDistance", "crossWalk", "parking", "busLane", "staticObstacle", "dynamicObstacle", "kidSafe", "bust" };
		/*if (trigger[5] && trigger[5] < 4) {
			dataContainer->setValue_yolo_missionID(STATIC_OBSTACLE);
		}*/

		dataContainer->setValue_yolo_trafficID(trigger[0]);

		if (trigger[3] == 0) { //parking
			dataContainer->setValue_yolo_missionID(PARKING);
		}
		else {
			dataContainer->setValue_yolo_missionID(BASIC);
		}

		dataContainer->updateValue_yolo_status();

	}

	closesocket(client);
	closesocket(server);
}

Yolo::~Yolo() {
	TerminateProcess(pi.hProcess, NULL);
}

void Yolo::SuspendYolo() {
	SuspendThread(tid);
}

void Yolo::ResumeYolo() {
	ResumeThread(tid);
}

/*
Ariadne to View Communication
포트 8888
전송 데이터타입 : 숫자
1 상시
2 좌
3 우
4 후진
5 비상정지
6 동적장애
7 시동
숫자로 전송하면됨,
통합 요청 사항 - 특정 이벤트가 끝나고 나면 1번 상시등으로 전환 부탁.
*/

View::View() {
	dataContainer = DataContainer::getInstance();
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi)); // assign program memory
	TCHAR commandLine[] = TEXT(""); // VIEW command창에서 실행할 수 있는 명령어
	if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
	}

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		cout << "View error\n"; /// 통신 포트 초기화 왠진 모르겠지만 해야함

	server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); /// local host의 번호(무조건 이거)
	addr.sin_port = htons(8888); // 내부 포트 번호

	if (::connect(server, (SOCKADDR*)&addr, sizeof(addr)) == -1) {
		cout << "Connent error!!" << endl;
	}
}

View::~View() {
	close(server);
}

void View::comView(int id) {
	//char* c;            
	//sprintf(c, "%d", id);
	std::string s = std::to_string(id);
	char const *pchar = s.c_str();
	int n = send(server, pchar, sizeof(pchar), 0);
	printf("send to view %s \n", pchar);
}

void View::SuspendView() {
	SuspendThread(tid);
}

void View::ResumeView() {
	ResumeThread(tid);
}
