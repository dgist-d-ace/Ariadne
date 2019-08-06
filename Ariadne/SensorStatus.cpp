#include "SensorStatus.h"
#include "ScnnFunc.h"

#include <tchar.h>


#define UPDATE_PLATFORM_STATUS 100
#define UPDATE_SENSOR_CONNECTION 101
#define UPDATE_SENSOR_STATUS 102
#define UPDATE_SENSOR_AUTOSTARTUP 103

#define PIPE_NAME "\\\\.\\pipe\\test_pipe" 

LidarComThread::LidarComThread() {
	dataContainer = DataContainer::getInstance();
}

int LidarComThread::comLidar() {
    LastOfLiDAR lol;

    if (!lol.Initialize()) {
        cout << "Connect ERROR!!!" << endl;
        return -1;
    }

    lol.StartCapture();

    while (1) {
        if (lol.m_bDataAvailable) {

            dataContainer->updateValue_lidar_status();

            
			lol.GetValidDataRTheta(lol.validScans);
			lol.Conversion(lol.validScans, lol.finQVecXY);
			lol.Average(lol.finQVecXY, lol.finVecXY);
			lol.Clustering(lol.finVecXY, lol.finObjData);
			lol.Vector(lol.finObjData, lol.finVecData, lol.finBoolData);
			
			dataContainer->setValue_lidar_BoolData(lol.finBoolData);
			dataContainer->setValue_lidar_Data(lol.finObjData);
			dataContainer->setValue_lidar_VecXY(lol.finVecXY);
			dataContainer->setValue_lidar_VecData(lol.finVecData);

            //lol.DrawData(lol.finVecXY, lol.finLiDARData, lol.finVecData, lol.finBoolData, lol.imgLiDAR);

            //ov.PlatformVector(lol.finLiDARData, ov.finVecData, ov.finBoolData);
            //ov.DrawVector(lol.finLiDARData, ov.finVecData, lol.imgLiDAR);

            // cout << "Reset" << endl;

           
        }


    }

    lol.StopCapture();
    lol.UnInitialize();
    return 0;
}

void LidarComThread::run() {
    cout << "Lidar Thread run\n";
    comLidar();
}


//LidarCom::LidarCom() {
//
//}
//
//
//void LidarCom::comLidar() {
//    std::cout << "lidar start\n";
//
//    LastOfLiDAR lol;
//    ObjectVector ov;
//
//    if (!lol.Initialize()) {
//        std::cout << "lidar not connect\n";
//        emit(LidarExit());
//        return;
//    }
//
//    lol.StartCapture();
//
//    cout << "라이다컴 함수에서 호출: " << lol.m_bDataAvailable << endl;
//
//    while (1) {
//        if (lol.m_bDataAvailable) {
//
//            cout << "while loop now" << endl;
//
//            dataContainer->updateValue_lidar_status();
//
//            lol.imgLiDAR = cv::Mat::zeros(768, 1366, CV_8UC3);
//
//            lol.GetValidDataRTheta(lol.validScans);
//            lol.Conversion(lol.validScans, lol.finQVecXY, 5);
//            lol.Average(lol.finQVecXY, lol.finVecXY, 5);
//            lol.Clustering(lol.finVecXY, lol.finLiDARData);
//            lol.Vector(lol.finLiDARData, lol.finVecData, lol.finBoolData);
//            lol.DrawData(lol.finVecXY, lol.finLiDARData, lol.finVecData, lol.finBoolData, lol.imgLiDAR);
//
//            //ov.PlatformVector(lol.finLiDARData, ov.finVecData, ov.finBoolData);
//            //ov.DrawVector(lol.finLiDARData, ov.finVecData, lol.imgLiDAR);
//
//            std::cout << "Reset" << std::endl;
//
//            cv::imshow("DrawLiDARData", lol.imgLiDAR);
//        }
//        else {
//            cout << "라이다 종료" << endl;
//            emit(LidarExit());
//            return;
//        }
//
//        int key = cv::waitKey(1);
//
//        if (key == 27) {
//            break;
//        }
//    }
//    lol.StopCapture();
//    lol.UnInitialize();
//}


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
	SetCurrentDirectory(_T("C:\\Users\\D-Ace\\darknet-master\\build\\darknet\\x64"));
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
	dataContainer = DataContainer::getInstance();
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


