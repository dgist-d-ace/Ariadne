
#include "DataContainer.h"
#include "Planner.hpp"
#include <iostream>
// header for GPS
#include <string> 
#include <sstream>
#include <fstream>

//Tuning point in getLanedata
#define LanescoreStep 5
///#define itvLane	20 //600*600
#define itvLane	15 //400*400

//Tuning point in getGpsData
#define GPSscoreStep 10
///#define itvGPS 30 //600*600
#define itvGPS 20 //400*400
#define numGPS 100

#define steerRatio  1.0
#define speedHigh	10.0
#define speedLow	7.0

//Tuning points in Dynamic Mission
#define stopDist 1200.0 //for Dynamic Obstacle mission
#define DynamicMaxSpeed 12.0

class Driving : public QObject
{
	Q_OBJECT
protected:
	/////////////////
	////FOR PASIV////
	/////////////////
	Mat getLaneData(int scorestep);
	Mat getGpsData(int scorestep);
	///Mat imgPath = Mat::zeros(600, 600, CV_8UC1);
	Mat imgPath = Mat::zeros(400, 400, CV_8UC1);
	Mat scoreMap;
	///Mat outRange = Mat::zeros(600, 600, CV_8UC1);
	Mat outRange = Mat::zeros(400, 400, CV_8UC1);
	double cenX = (double)imgPath.cols * 0.5, cenY = (double)imgPath.rows *1.0; //the location of LiDAR in the map.
	double scale = cenY / (SICK_SCAN_ROI_Y + 50);
	double carW = CAR_WIDTH * scale, carH = CAR_HEIGH * scale;
	uchar onestep = (CAR_WIDTH)* scale;

	//ROI AREA in the maps
	double leftEndX = cenX - SICK_SCAN_ROI_X * scale;
	double rightEndX = cenX + SICK_SCAN_ROI_X * scale;
	double topEndY = cenY - SICK_SCAN_ROI_Y * scale;
	double bottomEndY = cenY;
	//double bottomEndY = cenY + SICK_SCAN_ROI_Y * scale;

	vector<Mat> checkImgs;
	vector<int> checkTheta = { 0, -6, 6, -4, 4, -2, 2, -8, 8, -14, 14, -11, 11, -20, 20, -17, 17, -28, 28, -24, 24 };
	vector<int> checkTheta2 = { 0, -4,4,     -8,8,     -12,12,       -16,16 }; //9

	//////////////////////////////////////////////
	//////////////////////////////////////////////

public:
	DataContainer *dataContainer;
	Driving();
	double rad2deg(double);
	double deg2rad(double);

	void PASIVcontrol(Mat imgPath, double desired_speed, double steer1, double steer2, double desired_brake = 0);

	// functions for setting GPS way points
	void setPath(); // 모든 주행경로 저장

	int initialGPSpoint;
	int presentGPSpoint;
	vector<Point2d> path; //total way point.
	void getGPSinitial(double x_p, double y_p, vector<Point2d> path);
	vector<Point2d> forPASIV_path(double x_p, double y_p, vector<Point2d> path); //현재 위치와 가장 가까운 한 점을 찾은 후 이 점으로 부터 n개의 좌표를 저장
	vector<Point2d> getWaypoint(double x_p, double y_p, double heading, vector<Point2d> forPASIV_path); // PASIV 주행을 위한 좌표계 변환
	vector<Point2d> WaySimul_straight();
	vector<Point2d> WaySimul_turn();


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
