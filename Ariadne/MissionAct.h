
#include "DataContainer.h"
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

	cv::Mat imgPath;
	vector<uint> score[169]; //include the scores at [90,85, 95, 80, 100, 75, 105, 70, 110, 65, 115, 60, 120]degrees
	vector<int> theta = { 0, -5, 5, -10, 10, -15, 15, -20, 20, -25, 25, -30, 30 }; //The steering angle candidates
	//vector<int> theta = { 0, -5, 5, -10, 10, -15, 15 };
public slots:
	void Basic();

	void setData_speed(int speed);			
	void setData_steering(int steering);
	
	void Mission1();

};

