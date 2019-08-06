
#include "DataContainer.h"
#include <iostream>

#define steeringKP 0.75
#define speedKP		1.5
#define steerRatio  0.8

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

public slots:
	void Basic();

	void setData_speed(int speed);			
	void setData_steering(int steering);
	
	void Mission1();

};

