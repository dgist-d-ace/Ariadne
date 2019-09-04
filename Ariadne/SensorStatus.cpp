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

	//Mat picLiDAR = imread("maps/intersection1.png", 0); // exeption error: ���� �̸� �߸��Ȱ���

	// ������ 1�� ��θ� ���� ���� �� ������ ����
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
	//scnn.attr("scnn_init")("C:/Users/D-Ace/Documents/Ariadne/Ariadne/kcity_aug2_crop_pass4.pth", 0, true);
	scnn.attr("scnn_init")("C:/Users/D-Ace/Documents/Ariadne/Ariadne/kcity_aug2_crop_pass4.pth", "C:/Users/D-Ace/Documents/Ariadne/Ariadne/test2.mp4", true);
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

	client = accept(server, NULL, NULL); /// ������ ������ �����Ҷ����� ��ٸ��ڴ� <- ������ Ȯ���� �õ��ߴٰ� �� ��
	/// �߸� ��ٰ� Ariadne�� ���� ���� ����. cout�� �ȶ�� �̷��� accept�� �߸��Ǿ��� ���ɼ��� ����

	char message[5000];
	int strLen;
	int n;
	int i;
	while (1) {
		ZeroMemory(&message, sizeof(message));
		strLen = recv(client, (char*)message, sizeof(message) - 1, 0);
		/// �޾ƿ��� �޽����� ������
		//n = atoi(message);
		cout << message << endl;
		vector<vector<cv::Point2i>> lanes(4);
		vector<int> existLanes(4); /// ������ �ִ��� ������
		stringstream in; /// string���� �޾ƿ� ���� �������� -> �� �ϳ� ���� ���� ���� ����
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
			cout << existLanes[i] << endl;; /// lane�� 4�� �����µ� ���ʺ��� ������ �ִ� ���� ��.
			/// ���� �ϳ��� �� ����. ���� 4�� ����
		}

		dataContainer->setValue_scnn_lanes(lanes);
		dataContainer->setValue_scnn_existLanes(existLanes);
	}
	closesocket(client); /// ��� ����
	closesocket(server); /// ��� ����
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

	TCHAR commandLine[] = TEXT("darknet.exe detector demo data\\obj.data cfg\\yolov3_please.cfg yolov3_22000_0903.weights");
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


		/*if (trigger[5] && trigger[5] < 4) {
			dataContainer->setValue_yolo_missionID(STATIC_OBSTACLE);
		}*/
		if (trigger[6] < 4) {
			dataContainer->setValue_yolo_missionID(DYNAMIC_OBSTACLE);
		}
		// tigger[1] == 0 �� ��, �÷������κ��� ���������� 2m ������
		// INTER_STOP ���� õõ�� �����ִ°� �����Ǿ� ������ ��
		else if (trigger[1] == 0) {
			if (trigger[0] == 0) {
				dataContainer->setValue_yolo_missionID(INTER_STOP);
			}
			else if (trigger[0] == 1) {
				dataContainer->setValue_yolo_missionID(INTER_LEFT);

			}
			else if (trigger[0] == 2) {
				dataContainer->setValue_yolo_missionID(INTER_STRAIGHT);

			}
			else if (trigger[0] == 3) {
				dataContainer->setValue_yolo_missionID(INTER_RIGHT);

			}
			else if (trigger[3] == 0) {
				dataContainer->setValue_yolo_missionID(BUSLANE);
			}
		}
		else if (trigger[3] == 0) { //parking
			dataContainer->setValue_yolo_missionID(PARKING);
		}
		else {
			dataContainer->setValue_yolo_missionID(BASIC);
		}

		vector<int> mission; 

		for (int i = 0; i < 9; i++) {
			mission.push_back(trigger[i]);
		}

		dataContainer->setValue_yolo_missions(mission);

		dataContainer->updateValue_yolo_status();

		if (!trigger[8]) {
			dataContainer->setValue_speed_ratio_bust(SPEED_RATIO_LOW);
			bustCon = clock();
			emit(BustExist(true));
		}

		if (bustCon && clock() - bustCon > 1000) {
			dataContainer->setValue_speed_ratio_bust(1);
			bustCon = 0;
			emit(BustExist(false));
		}

		if (!trigger[7]) {
			dataContainer->setValue_speed_ratio_kid(SPEED_RATIO_LOW);
			emit(KidsafeExist(true));
		}
		else {
			dataContainer->setValue_speed_ratio_kid(1);
			emit(KidsafeExist(false));
		}
		//cout << trigger[0] << " " << trigger[1] << " " << trigger[2] << " " << trigger[3] << " " << trigger[4] << " " << trigger[5] << " " << trigger[6] << " " << trigger[7] << " " << trigger[8] << " " << endl;
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
��Ʈ 8888
���� ������Ÿ�� : ����
1 ���
2 ��
3 ��
4 ����
5 �������
6 �������
7 �õ�
���ڷ� �����ϸ��,
���� ��û ���� - Ư�� �̺�Ʈ�� ������ ���� 1�� ��õ����� ��ȯ ��Ź.
*/

View::View() {
	dataContainer = DataContainer::getInstance();
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi)); // assign program memory
	TCHAR commandLine[] = TEXT(""); // VIEW commandâ���� ������ �� �ִ� ��ɾ�
	if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
	}

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		cout << "View error\n"; /// ��� ��Ʈ �ʱ�ȭ ���� �𸣰����� �ؾ���

	server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); /// local host�� ��ȣ(������ �̰�)
	addr.sin_port = htons(8888); // ���� ��Ʈ ��ȣ

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
