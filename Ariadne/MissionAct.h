
#include "DataContainer.h"
#include "Planner.hpp"
#include <iostream>
// header for GPS
#include <string> 
#include <sstream>
#include <fstream>

//for origin PASIV
#define steerRatio  1.0
#define speedHigh	12.0
#define speedLow	5.0

//Tuning for Dynamic Mission
#define stopDist 1200.0 //for Dynamic Obstacle mission
#define DynamicMaxSpeed 8.0

//Tuning for getLaneData
#define scoreStep 5
#define itvLane	20

class Driving : public QObject
{
	Q_OBJECT
protected:
	/////////////////
	////FOR PASIV////
	/////////////////
	Mat getLaneData(int scorestep);
	void getGpsData(int scorestep);

	Mat imgPath = Mat::zeros(600, 600, CV_8UC1);	//final Path imgae in PASIV
	Mat scoreMap;

	Mat outRange = Mat::zeros(600, 600, CV_8UC1);	//out of steering angle range
	
	double cenX = (double)imgPath.cols * 0.5, cenY = (double)imgPath.rows *1.0; //the location of LiDAR in the map.
	double scale = cenY / (SICK_SCAN_ROI_Y + 50);
	
	double carW = CAR_WIDTH * scale, carH = CAR_HEIGH * scale;
	uchar onestep = (CAR_WIDTH)* scale;
	double leftEndX = cenX - SICK_SCAN_ROI_X * scale;
	double rightEndX = cenX + SICK_SCAN_ROI_X * scale;
	double topEndY = cenY - SICK_SCAN_ROI_Y * scale;
	double bottomEndY = cenY + SICK_SCAN_ROI_Y * scale;

	vector<Mat> checkImgs;
	vector<int> checkTheta = { 0, -6, 6, -4, 4, -2, 2, -8, 8, -14, 14, -11, 11, -20, 20, -17, 17, -28, 28, -24, 24};
	vector<int> checkTheta2= { 0, -4,4,     -8,8,     -12,12,       -16,16 }; //9

	//////////////////////////////////////////////
	//////////////////////////////////////////////

public:
	DataContainer *dataContainer;
	Driving();
    double rad2deg(double);
    double deg2rad(double);
	
	void PASIVcontrol(double desired_speed, double goTheta1, double goTheta2, double desired_brake = 0);

	// functions for setting GPS way points
	void setPath(); // 모든 주행경로 저장
	vector<vector<double>> path;
	vector<vector<double>> forPASIV_path(double x_p, double y_p, vector<vector<double>> path); //현재 위치와 가장 가까운 한 점을 찾은 후 이 점으로 부터 n개의 좌표를 저장
	vector<vector<double>> getWaypoint(double x_p, double y_p, double heading, vector<vector<double>> forPASIV_path); // PASIV 주행을 위한 좌표계 변환
	//vector<vector<double>> gpsWayPoint;
	vector<vector<double>> WaySimul_straight();
	vector<vector<double>> WaySimul_turn();


	Planner *aster;
signals:
	void send2View(int id);
	void currentMission(int id);

public slots:
	void DrawData();
	void LOS();

	void autoMode();

	void Basic(int missionId);
	void BasicGPS(int missionId);
	void MissionParking(); // 주차
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
	int brake = 50;

public slots:
	void MissionIDUpdate();

signals:
	void greenRight(bool);
};
