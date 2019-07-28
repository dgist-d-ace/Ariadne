#include "SensorStatus.h"
#include "ScnnFunc.h"

#include <iostream>


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
}

void Scnn::comScnn() {
    cout << "scnn start\n";
    while (1)
        mainfun();
}

Yolo::Yolo() {
	dataContainer = DataContainer::getInstance();
}

void Yolo::comYolo() {
	//
	// To Do : integraiting
	//
}

