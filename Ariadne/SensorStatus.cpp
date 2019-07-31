#include "SensorStatus.h"
#include "ScnnFunc.h"

#include <iostream>
#include <list>
#include <tchar.h>


#define UPDATE_PLATFORM_STATUS 100
#define UPDATE_SENSOR_CONNECTION 101
#define UPDATE_SENSOR_STATUS 102
#define UPDATE_SENSOR_AUTOSTARTUP 103

LidarCom::LidarCom() {

}

void LidarCom::comLidar() {
    std::cout << "lidar start\n";

    LastOfLiDAR lol;
    ObjectVector ov;

    if (!lol.Initialize()) {
        std::cout << "lidar not connect\n";
        emit(LidarExit());
        return;
    }

    lol.StartCapture();

    while (1) {
        if (lol.m_bDataAvailable) {

            dataContainer->updateValue_lidar_status();

            lol.imgLiDAR = cv::Mat::zeros(768, 1366, CV_8UC3);

            lol.GetValidDataRTheta(lol.validScans);
            lol.Conversion(lol.validScans, lol.finQVecXY, 5);
            lol.Average(lol.finQVecXY, lol.finVecXY, 5);
            lol.Clustering(lol.finVecXY, lol.finLiDARData);
            lol.Vector(lol.finLiDARData, lol.finVecData, lol.finBoolData);
            lol.DrawData(lol.finVecXY, lol.finLiDARData, lol.finVecData, lol.finBoolData, lol.imgLiDAR);

            //ov.PlatformVector(lol.finLiDARData, ov.finVecData, ov.finBoolData);
            //ov.DrawVector(lol.finLiDARData, ov.finVecData, lol.imgLiDAR);

            std::cout << "Reset" << std::endl;

            cv::imshow("DrawLiDARData", lol.imgLiDAR);
        }
        else {
            emit(LidarExit());
            return;
        }

        int key = cv::waitKey(1);

        if (key == 27) {
            break;
        }
    }
    lol.StopCapture();
    lol.UnInitialize();
}


Scnn::Scnn() {
	dataContainer = DataContainer::getInstance();
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi)); // assign program memory
	TCHAR commandLine[] = TEXT("C:\Python\Python36");

	//if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
	//}

	//tid = pi.hThread;

	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(23000);

	if (::bind(server, (sockaddr*)&addr, sizeof(addr))==SOCKET_ERROR)
		cout<<"binding fail\n";

	if (listen(server, SOMAXCONN) == SOCKET_ERROR)
		cout << "listening fail\n";
}

void Scnn::comScnn() {
		
	ResumeThread(tid);

	client = accept(server, NULL, NULL);

	char message[500];
	int strLen;
	list<int>* data;

	while (1) {
		strLen = recv(client, message, sizeof(message) - 1, 0);
		if (strLen == 0) continue;
		message[strLen] = '\0';
		data = (list<int>*)message;
		cout << data << endl;
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
	
	TCHAR commandLine[] = TEXT("darknet detector demo data\\coco.data yolov3.cfg yolov3.weights data\\race1_cut.mp4");
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
	addr.sin_port = htons(8020);

	if (::bind(server, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		cout << "binding fail\n";

	if (listen(server, SOMAXCONN) == SOCKET_ERROR)
		cout << "listening fail\n";
	dataContainer = DataContainer::getInstance();
}

void Yolo::comYolo() {
	//
	// To Do : integraiting
	//
	//SuspendThread(tid);
	ResumeThread(tid);
	cout << "try com\n";

	client = accept(server, NULL, NULL);
	if (client == INVALID_SOCKET)
		cout << "invalid socket\n";
	else cout << "valid socket\n";

	char* message;
	int strLen;
	float* data;
	cout << "communication1\n";

	while (1) {
		cout << "communication2\n";
		strLen = recv(client, message, sizeof(message) - 1, 0);
		if (strLen == 0) continue;
		message[strLen] = '\0';
		data = (float*)message;
		cout<< *data << endl;
	}

	closesocket(client);
	closesocket(server);
}

