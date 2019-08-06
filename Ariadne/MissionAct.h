
#include "DataContainer.h"
#include <iostream>

#define steeringKP 0.75
#define speedKP		1.5

#define ALWAYS 1
#define LEFT 2
#define RIGHT 3
#define BACKWARD 4
#define STOP 6
#define ON 7


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
	SOCKET server;
	SOCKET client;

public slots:
	void Basic();

	void setData_speed(int speed);
	void setData_steering(int steering);
	void Intersection();
	void SendToArduino(int);

signals:
	void arduino(int);
};

