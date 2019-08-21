
#include "DataContainer.h"
#include "Planner.hpp"
#include <iostream>

#define steerRatio  1.0
#define speedHigh	12.0
#define speedLow	4.0

#define stopDist 2000 //for Dynamic Obstacle mission
#define DynamicMissionSpeed 10
class Driving : public QObject
{
	Q_OBJECT
protected:

public:
	DataContainer *dataContainer;
	Driving();
    double rad2deg(double);
    double deg2rad(double);
	Mat getLaneData(int scorestep);
	void getGpsData(int scorestep);
	Mat imgPath = Mat::zeros(600, 600, CV_8UC1);
	Mat scoreMap;
	vector<Mat> checkImgs;
	//vector<int> checkTheta = { 0, -2,2, -4, 4, -6, 6, -8, 8, -10,10, -12,12, -14, 14, -16,16,-18,18, -20, 20, -22,22,-24,24,-26, 26 };//The steering angle candidates
	//vector<int> checkTheta = { 0, -5,5,-10,10,-15,15,-20,20,-25,25,-30,30 };//The steering angle candidates
	vector<int> checkTheta = { 0, -3,3,-6,6,-9,9,-12,12,-15,15,-18,18,-21,21,-24,24,-27,27 };
	vector<int> checkTheta2 = { 0,-6,6,-12,12,-18,18,-24,24 };


	Planner *aster;

public slots:
	void DrawData();
	void LOS();

	void Basic();

	void Mission1(); // 주차
    void Mission2(); 
    void Mission3();
    void Mission4();
    void Mission5(); // 정적장애물
	void Mission6(); // 동적장애물

};

class SpeedControl : public QObject
{
	Q_OBJECT
protected:

public:
	DataContainer *dataContainer;
	SpeedControl();

public slots:
	void ControlbySituation();
};
