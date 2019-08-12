#include "SensorStatus.h"
#include "ScnnFunc.h"
#include <tchar.h>


#define UPDATE_PLATFORM_STATUS 100
#define UPDATE_SENSOR_CONNECTION 101
#define UPDATE_SENSOR_STATUS 102
#define UPDATE_SENSOR_AUTOSTARTUP 103

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

			//imshow("DrawLiDARData", resizeLiDAR);

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
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi)); // assign program memory
	TCHAR commandLine[] = TEXT("C:\\ProgramData\\Anaconda3\\Scripts\\activate_torch.bat");
	if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
	}
	else {
		tid = pi.hThread;
		SuspendThread(tid);
	}

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		cout << "error\n";

	server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(2222);

	if (::bind(server, (sockaddr*)&addr, sizeof(addr))==SOCKET_ERROR)
		cout<<"binding fail\n";

	if (listen(server, SOMAXCONN) == SOCKET_ERROR)
		cout << "listening fail\n";
}

void Scnn::comScnn() {
		
	ResumeThread(tid);

	client = accept(server, NULL, NULL);

	char message[5000];
	int strLen;
	int n;
	int i;
	while (1) {
		ZeroMemory(&message, sizeof(message));
		strLen = recv(client, (char*)message, sizeof(message) - 1, 0);

		//n = atoi(message);
		cout << message << endl;
		vector<vector<cv::Point2i>> lanes(4);
		vector<int> existLanes(4);
		stringstream in;
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
			cout << existLanes[i] << endl;;
		}

		dataContainer->setValue_camera1_lanes(lanes);
		dataContainer->setValue_camera1_existLanes(existLanes);
	}
	closesocket(client);
	closesocket(server);
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
	
	TCHAR commandLine[] = TEXT("darknet detector demo data\\obj.data cfg\\yolov3_please.cfg yolov3_34500.weights data\\race1_cut.mp4");
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

	//TCHAR pipe_name[] = TEXT("\\\\.\\pipe\\test_pipe");
	//hNamePipe = CreateNamedPipe(pipe_name,
	//	PIPE_ACCESS_DUPLEX,
	//	PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
	//	PIPE_UNLIMITED_INSTANCES,
	//	0,
	//	0,
	//	20000,       // 대기 Timeout 시간
	//	NULL
	//);

	//if (hNamePipe == INVALID_HANDLE_VALUE)
	//	printf("CreateNamePipe error! \n");
	//



}

void Yolo::comYolo() {
	//
	// To Do : integraiting
	//
	//SuspendThread(tid);
	ResumeThread(tid);

	//while(1) {
	//	if (!ConnectNamedPipe(hNamePipe, NULL))
	//		CloseHandle(hNamePipe);
	//	else {
	//		if (ConnectClient(hNamePipe) == -1)
	//			break;
	//	}
	//}
	//DisconnectNamedPipe(hNamePipe);
	//CloseHandle(hNamePipe);

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

	/// TODO: control yolo flag in Ariadne.cpp
	while (Yolo_Com) {
		cout << "communication2\n";

		strLen = recv(client, message, sizeof(message) - 1, 0);
		if (strLen == -1) cout << "send error\n";
		cout << message << endl;
	}

	closesocket(client);
	closesocket(server);
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
    TCHAR commandLine[] = TEXT(" "); // VIEW command창에서 실행할 수 있는 명령어
    if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
    }
    else {
        tid = pi.hThread;
        SuspendThread(tid);
    }

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        cout << "View error\n";

    server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8888); // 내부 포트 번호

    if (::bind(server, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
        cout << "View binding fail\n";

    if (listen(server, SOMAXCONN) == SOCKET_ERROR)
        cout << "View listening fail\n";
}

void View::comView() {
}

void View::SuspendView() {
    SuspendThread(tid);
}

void View::ResumeView() {
    ResumeThread(tid);
}

int ConnectClient(HANDLE hNamePipe)
{
	TCHAR recvMessage[100];
	TCHAR sendMessage[100];
	DWORD recvSize;
	DWORD sendSize;

	while (1)
	{
		_tprintf(_T("Input Send Message : "));
        _tscanf(_T("%s"), sendMessage);

		//sendSize -> NULL 포함한 바이트 수

		if (!(WriteFile(

			hNamePipe,
            sendMessage,
            (_tcslen(sendMessage) + 1) * sizeof(TCHAR),
            &sendSize,
            NULL

		)))          // 

		{
            _tprintf(_T("WriteFile error! \n"));
            return -1;
        }

		FlushFileBuffers(hNamePipe);
        
		//recvSize -> NULL 포함한 바이트 수

		if (!(ReadFile(
            hNamePipe,
			recvMessage,
			sizeof(recvMessage) - sizeof(TCHAR) * 1,
			&recvSize,
			NULL
		)))

		{
			printf("ReadFile error! \n");
			return -1;
		}

		recvMessage[recvSize / sizeof(TCHAR) - 1] = _T('\x00');
		_tprintf(_T("Recv Message : %s \n"), recvMessage);
	}

	return 1;
}


