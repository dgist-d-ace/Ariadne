#include "SensorStatus.h"
#include "scnn.h"

#include <iostream>



#define UPDATE_PLATFORM_STATUS 100
#define UPDATE_SENSOR_CONNECTION 101
#define UPDATE_SENSOR_STATUS 102
#define UPDATE_SENSOR_AUTOSTARTUP 103

PlatformCom::PlatformCom() // 추후 인자로 CString이 들어갈 것
{
    dataContainer = DataContainer::getInstance();
    //플랫폼 통신 코드입니다.
  
}

void PlatformCom::comPlatform() {
	cout << "platform start\n";

	if (_platform.OpenPort(L"COM6"))   // 실제 사용될 COM Port 를 넣어야합니다.  
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


LidarCom::LidarCom() {

}

void LidarCom::comLidar() {
	cout << "lidar start\n";

	LastOfLiDAR lol;
	ObjectVector ov;

	if (!lol.Initialize()) {
		cout << "lidar not connect\n";
		emit(LidarExit());
		return;
	}
	else {
		cout << "lidar else\n";
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

			cout << "Reset" << endl;

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
}

void Scnn::comScnn() {
	cout << "scnn start\n";
	while(1)
		mainfun();
}


