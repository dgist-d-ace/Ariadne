
#include "DataContainer.h"
#include <iostream>
// header for GPS
#include <string> 
#include <sstream>
#include <fstream>
#include <algorithm>

//Tuning point in getLanedata
#define LanescoreStep 5
///#define itvLane	20 //600*600
#define itvLane	25 //400*400

//Tuning point in getGpsData
#define GPSscoreStep 30
///#define itvGPS 30 //600*600
#define itvGPS 20 //400*400
#define numGPS 300
#define numGPSMAP 40

#define steerRatio  1.0
#define speedHigh	10.0
#define speedLow	5.0

//Tuning points in Dynamic Mission
#define stopDist 1200.0 //for Dynamic Obstacle mission
#define DynamicMaxSpeed 12.0

//Tuning points for GPS
#define rate (1/60)
#define K 0.7
#define desired_road_length 3
#define parking_speed  3
#define park_y 4
#define park_x 2
#define circle_path 4.5
#define speed_error 19
#define dis_error_rate 0.9
#define brake 100

#define deg_2_rad (CV_PI / 180)

//Camera define setting
#define CAMERA_X 1280
#define CAMERA_Y 720
#define CAMERA_H 1.0
#define CAMERA_ANGLE 78.0
#define CAMERA_TILT 20.0
#define CAMERA_TOP_ANGLE 15.0
#define CAMERA_SIDE_ANGLE 24.0
#define CAMERA_AREA_MIN 50000
#define CAMERA_AREA_MAX 500000

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
	uchar onestep = (3000)* scale;

	//ROI AREA in the maps
	double leftEndX = cenX - SICK_SCAN_ROI_X * scale;
	double rightEndX = cenX + SICK_SCAN_ROI_X * scale;
	double topEndY = cenY - SICK_SCAN_ROI_Y * scale;
	double bottomEndY = cenY;
	//double bottomEndY = cenY + SICK_SCAN_ROI_Y * scale;

	vector<Mat> checkImgs;
	vector<int> checkTheta = { 0, -1,1-2, 2, -4, 4, -6, 6, -8, 8, -11, 11, -14, 14, -17, 17, -20, 20, -24, 24, -28, 28,-32, 32 }; //25
	vector<int> checkTheta2 = { 0, -3,3,     -6,6,     -9,9,     -12,12  }; //9

	//////////////////////////////////////////////
	//////////////////////////////////////////////

	////////////////////   PARKING   //////////////////////////

	Mat FilteredImage(Mat &img_camera);
	Mat FindLotLines(Mat &img_edge);
	vector<vector<Point2f> > FindLotContours(Mat &img_camera, Mat &img_edge);
	Point2f SetLotCenter(vector<vector<Point2f> > &contours);
	vector<float> PosFromCamera(Point2f &center);
	vector<float> FindParkingLot(Mat &img_camera);
	void controlSpeed(int speed);
	void brakeTime(double second);
	void controlENC(int gear, int speed, double dist, int steer = 0);

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
	void practice(double parkDis);
	int ParkingMission();
	int busNum = 0;
	int parkingNum = 0;
	int objflag = 0;

signals:
	void send2View(int id);
	void currentMission(int id);
	void exitMission(int id);

public slots:
	void DrawData();
	void LOS();

	void autoMode();

	void Basic(int missionId);
	void BasicGPS(int missionId);
	void MissionParking(); // 주차
	void MissionBusLane(); // intersection ready
	void MissionIntLeft(); // intersectin left
	void MissionIntRight(); // intersection right
	void MissionIntStraight(); // intersection straight
	void MissionIntStop(); // intersection stop
	void MissionStaticObs(); // static obstacle
	void MissionDynamicObs(); // dynamic obstacle
};
