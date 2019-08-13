
#include "DataContainer.h"
#include "Planner.hpp"
#include <iostream>

#define steeringKP 0.75
#define speedKP		1.5
#define steerRatio  1.0

class Driving : public QObject
{
	Q_OBJECT
protected:

public:
	DataContainer *dataContainer;
	Driving();
    int GoTo(double, double, double);
    double rad2deg(double);
    double deg2rad(double);

	Mat imgPath;
	Mat scoreMap;
	//vector<int> theta = { 0, -5, 5, -10, 10, -15, 15 };

	Planner *aster;

public slots:
	void DrawData();
	void LOS();
	void Basic();
	Mat getLaneData();

	void Mission1();
    void Mission2();
    void Mission3();
    void Mission4();
    void Mission5();

};

