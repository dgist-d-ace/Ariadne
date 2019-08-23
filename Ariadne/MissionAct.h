
#include "DataContainer.h"
#include "Planner.hpp"
#include <iostream>

#define steerRatio  1.0
#define speedHigh	12.0
#define speedLow	5.0

#define stopDist 1500.0 //for Dynamic Obstacle mission
#define DynamicMaxSpeed 8.0

class Driving : public QObject
{
	Q_OBJECT
protected:
	/////////////////
	////FOR PASIV////
	/////////////////
	Mat getLaneData(int scorestep);
	void getGpsData(int scorestep);
	Mat imgPath = Mat::zeros(600, 600, CV_8UC1);
	Mat outRange = Mat::zeros(600, 600, CV_8UC1);
	double cenX = imgPath.cols * 0.5, cenY = imgPath.rows *1.0; //the location of LiDAR in the map.
	double scale = cenY / (SICK_SCAN_ROI_Y + 50);
	double carW = CAR_WIDTH * scale, carH = CAR_HEIGH * scale;
	uchar onestep = (CAR_WIDTH)* scale;

	Mat scoreMap;
	vector<Mat> checkImgs;
	//vector<int> checkTheta = { 0, -2,2, -4, 4, -6, 6, -8, 8, -10,10, -12,12, -14, 14, -16,16,-18,18, -20, 20, -22,22,-24,24,-26, 26 };//The steering angle candidates
	//vector<int> checkTheta = { 0, -5,5,-10,10,-15,15,-20,20,-25,25,-30,30 };//The steering angle candidates
	//vector<int> checkTheta = { 0, -3,3,-6,6,-9,9,-12,12,-15,15,-18,18,-21,21,-24,24,-27,27 };
	//vector<int> checkTheta2 = { 0,-6,6,-12,12,-18,18,-24,24 };
	vector<int> checkTheta = { 0, -4,4,-2,2,-8,8,-6,6,-12,12,-10,10,-18,18,-14,14, -26,26,-22,22};
	vector<int> checkTheta2 = { 0,-4,4,-8,8,-12,12,-16,16 };

	//////////////////////////////////////////////
	//////////////////////////////////////////////

public:
	DataContainer *dataContainer;
	Driving();
    double rad2deg(double);
    double deg2rad(double);
	
	void PASIVcontrol(int desired_speed, int desired_steering, int brake = 0);


	vector<vector<double>> gpsWayPoint;

	Planner *aster;
signals:
	void send2View(int id);

public slots:
	void DrawData();
	void LOS();

	void autoMode();

	void Basic(int missionId);
	void BasicGPS(int missionId);
	void MissionParking(); // ����
    void MissionIntReady(); // intersection ready
    void MissionIntLeft(); // intersectin left
    void MissionIntRight(); // intersection right
    void MissionIntStraight(); // intersection straight
	void MissionIntStop(); // intersection stop
	void MissionStaticObs(); // static obstacle
	void MissionDynamicObs(); // dynamic obstacle
	
};

class MissionUpdate : public QObject
{
	Q_OBJECT
protected:

public:
	DataContainer *dataContainer;
	MissionUpdate();

public slots:
	void MissionIDUpdate();

signals:
	void greenRight(bool);
};
