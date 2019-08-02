
#include "DataContainer.h"
#include <iostream>

class Driving : public QObject
{
	Q_OBJECT
protected:

public:
	DataContainer *dataContainer;
	Driving();
    void GoTo(double, double, double);
    double rad2deg(double);
    double deg2rad(double);

	cv::Mat imgPath;

public slots:
	void Basic();

	void Mission1();

};

