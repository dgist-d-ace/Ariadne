#include "SensorStatus.h"
#include "ScnnFunc.h"
#include <tchar.h>
#include "missionTrigger.h"

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


State getStartState(int startX, int startY, int startTheta, int startSteer, int startSpeed) {
	//to do: read from yml file
	//start point: center of bottom, theta: always 18.
	return State(startX, startY, startTheta, startSteer, startSpeed);
}

State getTargetState(int targetX, int targetY, int targetTheta, int targetSteer, int targetSpeed) {
	//to do: read from yml file
	//target point: center of top, alpha: random
	return State(targetX, targetY, targetTheta, targetSteer, targetSpeed);
}

Map getMatMap(Mat &imgLiDAR) {
	//to do: read from yml file
	//start point: center of bottom, theta: always 18.
	return Map(imgLiDAR);
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
	//ZeroMemory(&si, sizeof(si));
	//si.cb = sizeof(si);
	//ZeroMemory(&pi, sizeof(pi)); // assign program memory

	////change webcam or video here
	////TCHAR commandLine[] = TEXT("C:\\ProgramData\\Anaconda3\\Scripts\\activate_torch.bat");
	//TCHAR commandLine[] = TEXT("C:\\ProgramData\\Anaconda3\\Scripts\\activate_torch_cam.bat");
	//if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
	//}
	//else {
	//	tid = pi.hThread;
	//	SuspendThread(tid);
	//}

	//WSADATA wsa;
	//if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	//	cout << "error\n";

	//server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//sockaddr_in addr = { 0 };
	//addr.sin_family = AF_INET;
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//addr.sin_port = htons(2222);

	//if (::bind(server, (sockaddr*)&addr, sizeof(addr))==SOCKET_ERROR)
	//	cout<<"binding fail\n";

	//if (listen(server, SOMAXCONN) == SOCKET_ERROR)
	//	cout << "listening fail\n";
}

/// this function is for ruuning python scnn program in Ariadne
int Scnn::boostScnn() {

	using namespace std;

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
	//scnn.attr("scnn_init")("C:/Users/D-Ace/Documents/Ariadne/Ariadne/kcity_big_crop_pass4_best.pth", 0, true);
	scnn.attr("scnn_init")("C:/Users/D-Ace/Documents/Ariadne/Ariadne/kcity_big_crop_pass4_best.pth", "C:/Users/D-Ace/Documents/Ariadne/Ariadne/test2.mp4", true);
	//scnn.attr("scnn_init")("C:/Users/D-Ace/Documents/Ariadne/Ariadne/k_city_crop_exp1_best_pass4.pth", "C:/Users/D-Ace/Pictures/Camera Roll/4.mp4", true);

	//scnn.attr("scnn_init")("exp1_kcity_best_50.pth", 0, true);

	emit(drivingEnabled());
	while (1)
	{
		cout << 1 << endl;
		dataContainer->updateValue_scnn_status();
		py::object params = scnn.attr("scnn_run")();
		cout << 2 << endl;
		vector<int> lists = to_std_vector<int>(params);
		cout << 3 << endl;
		vector<int> existLanes;
		vector<vector<Point2i>> lanes(4);

		auto first = lists.cbegin();
		auto end = lists.cbegin() + 691200/64;

		vector<int32_t> img1d(first, end);
		vector<int> LANES(end + 1, lists.cend());

		auto image(QImage((uchar *)img1d.data(), 100, 36, 100*3, QImage::Format_RGB32));
		//QImage img;
		//img.loadFromData((uchar*)img1d.data(), img1d.size());
		//img.scaled(QSize(300, 300), Qt::KeepAspectRatio);

		////QImage img = QImage((uchar)img1d.data(), img1d.size(), QImage::Format_RGB888);
		////img.scaled(QSize(800/8, 288/8), Qt::KeepAspectRatio);
		dataContainer->setValue_ui_scnn(image);

		int i, j = 0;

		for (i = 0; i < 4; i++) {
			existLanes.push_back(LANES[i]);
		}

		dataContainer->setValue_scnn_existLanes(existLanes);
		//cout << "existLanes : ";

		for (i = 0; i < 4; i++)
		{
			//cout << lists[i] << " ";
		}
		//cout << endl;

		//cout << "existLanes : ";
		for (i = 0; i < 4; i++)
		{
			//cout << existLanes[i] << " ";
		}
		//cout << endl;

		//cout << "existLanes : ";
		for (i = 0; i < 4; i++)
		{
			//cout << dataContainer->getValue_scnn_existLanes()[i] << " ";
		}
		//cout << endl;

		j = 0;
		for (i = 5; i < LANES.size(); i++) {
			if (LANES[i] == -1) {
				j++;
			}
			else {
				cv::Point2i coordinate(LANES[i], LANES[i + 1]);
				lanes[j].push_back(coordinate);
				i++;
			}
		}


		dataContainer->setValue_scnn_lanes(lanes);

		for (int i = 0; i < 4; i++)
		{
			//cout << "lane " << i << " : ";
			for (int j = 0; j < dataContainer->getValue_scnn_lanes()[i].size(); j++) {}
			//cout << dataContainer->getValue_scnn_lanes()[i][j] << " ";
		//cout << endl;
		}

		existLanes.clear();
		lanes.clear();
	}

	scnn.attr("scnn_destroy")();
	print("good");


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

	TCHAR commandLine[] = TEXT("darknet.exe detector demo data\\obj.data cfg\\yolov3_please.cfg yolov3_54000.weights data\\middle.mp4");
	SetCurrentDirectory(_T("C:\\Users\\D-Ace\\darknet-master\\build\\darknet\\x64")); // Darknet program start command
	//if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
	//}
	if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		cout << "error\n";
	}
	else {
		cout << "open ok";
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

	cout << "try com\n";

	Sleep(5000);
	client = accept(server, NULL, NULL);
	if (client == INVALID_SOCKET)
		cout << "invalid socket\n";
	else cout << "valid socket\n";


	char message[8192];
	int strLen;
	float* data;
	cout << "communication1\n";

	static MissionContainer trigger;

	/// TODO: control yolo flag in Ariadne.cpp
	while (Yolo_Com) {


		strLen = recv(client, message, 8192, 0);
		if (strLen == -1) cout << "send error\n";

		int* trigger = new int;
		trigger = (int*)message;


		if (trigger[5] && trigger[5] < 4) {
			dataContainer->setValue_yolo_missionID(STATIC_OBSTACLE);
		}
		else if (trigger[6] < 4) {
			dataContainer->setValue_yolo_missionID(DYNAMIC_OBSTACLE);
		}
		else if (trigger[0] == 0 && trigger[1] && trigger[1] < 4) {
			dataContainer->setValue_yolo_missionID(INTER_STOP);
		}
		else if (trigger[0] == 2 && trigger[1] && trigger[1] < 4) {
			dataContainer->setValue_yolo_missionID(INTER_STRAIGHT);
		}
		else if (trigger[0] == 1 && trigger[1] && trigger[1] < 4) {
			dataContainer->setValue_yolo_missionID(INTER_LEFT);
		}
		else if (trigger[0] == 3 && trigger[1] && trigger[1] < 4) {
			dataContainer->setValue_yolo_missionID(INTER_RIGHT);
		}
		else if (trigger[3] && trigger[3] < 4) { //parking
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
		bool bustOn = (trigger[8] == 0);
		bool kidsafeOn = (trigger[7] == 0);
		if ( bustOn || kidsafeOn ) { dataContainer->setValue_yolo_speed_ratio(SPEED_RATIO_LOW); } //<- bust is on
		else { dataContainer->setValue_yolo_speed_ratio(1); }
		emit(BustExist(bustOn)); // if bust is in front of the car, UI bust button will be automatically switched.
		emit(KidsafeExist(kidsafeOn));
	}

	closesocket(client);
	closesocket(server);
}

Yolo::~Yolo() {
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	TerminateProcess(pi.hProcess, 0);
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

/// ///// ���� ���α׷� �ۼ��� �ֵ���

/// SCNN�� ���� ������ ���� ������ �˰������� ����� ���̺귯���� ������ġ�̸� ���̽����� �ۼ��Ǿ����ϴ�. (���̽� SCNN ������: ����ȯ)
/// ������ ������ġ�� c++���� �� �� ���� C++�� ���̺귯���� ������ġ�� ����Ϸ��� �߽��ϴ�.
/// �׷��� ������ġ�� ������(2019.08)�� ���ߵ� �� �� �ȵǴ� ���̺귯���� ������ġ�� �̿��� �Լ� ��� �����
/// ������ġ�� �̿��� �Լ� ��� ����� �ٸ��ٴ� ������ �־����ϴ�. 
/// ���� ���α׷� �Ƹ��Ƶ�׿��� scnn, lidar, yolo, platform, drive thread�� ������ ���ؼ��� scnn���� ������ ���� ������ �ʿ��߽��ϴ�
/// �׷����� ������ ������ ������ �ذ����� ���� ó������ Ariadne.exe ���ϰ� scnn.exe ������ TCP/IP ������� ������ �ְ�޴� ������� �ذ��ϰ��� �߽��ϴ�.
/// �ش� �ڵ尡 comScnn()�Դϴ�.
/// �׷��� ���̽� �ν�Ʈ�� c++���� import�Ͽ� �� �� �ִ� ����� �����Ͽ� ������ TCP/IP ��� ����� ���� �ʰ� �Ƹ��Ƶ�� ���ο��� SCNN�� ������ �Ǿ����ϴ�.
/// �ش� �ڵ�� boostScnn() �Դϴ�.
/// boostSCNN()�� SCNN���� ������ ���� ������ std::vector�� ��Ƽ� ���� �Լ��Դϴ�.
/// 
///
///
///
///
///