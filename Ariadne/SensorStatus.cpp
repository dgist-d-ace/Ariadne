#include "SensorStatus.h"
#include "scnn.h"

#include <iostream>



#define UPDATE_PLATFORM_STATUS 100
#define UPDATE_SENSOR_CONNECTION 101
#define UPDATE_SENSOR_STATUS 102
#define UPDATE_SENSOR_AUTOSTARTUP 103

PlatformCom::PlatformCom() // ���� ���ڷ� CString�� ��� ��
{
    dataContainer = DataContainer::getInstance();
    //�÷��� ���� �ڵ��Դϴ�.
    ComPlatform _serial;

    if (_serial.OpenPort(L"COM6"))   // ���� ������ COM Port �� �־����մϴ�.
    {
        _serial.ConfigurePort(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT);
        _serial.SetCommunicationTimeouts(0, 0, 0, 0, 0);

        while (loopStatusPlatform)
        {
			if (_serial.MyCommRead()) {}
			else {
                _serial.ClosePort();
                break;
            }
            _serial.MyCommWrite();
            dataContainer->updateValue_platform_status();

            emit(AorMChanged(dataContainer->getValue_PtoU_AorM()));
            emit(EStopChanged(dataContainer->getValue_PtoU_E_STOP()));
            emit(SpeedChanged(dataContainer->getValue_PtoU_SPEED()));
            emit(SteerChanged(dataContainer->getValue_PtoU_STEER()));
            emit(GearChanged(dataContainer->getValue_PtoU_GEAR()));
            emit(BreakChanged(dataContainer->getValue_PtoU_BRAKE()));
            emit(EncChanged(dataContainer->getValue_PtoU_ENC()));

            this->msleep(100);
        }
    }
}

void PlatformComThread::run() {
    cout << "Platform Thread run\n";
    CString temp;
    comPlatform();
	emit(PlatformExit());
}

LidarComThread::LidarComThread() {
    LidarComThread::dataContainer = DataContainer::getInstance();
}

int LidarComThread::comLidar() {

    LastOfLiDAR lol;
    ObjectVector ov;

    if (!lol.Initialize()) {
        cout << "Connect ERROR!!!" << endl;
        return -1;
    }

    lol.StartCapture();

    while (1) {
        cout << "lidar working\n";
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

        int key = cv::waitKey(1);

        if (key == 27) {
            break;
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

void ScnnThread::run() {
	cout << "scnn thread run\n";
    while (1) {
        mainfun();
    }
}



	if (_platform.OpenPort(L"COM6"))   // ���� ������ COM Port �� �־����մϴ�.
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
