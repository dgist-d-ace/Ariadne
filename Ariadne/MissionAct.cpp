// This is for executing each mission

#include "MissionAct.h"
#include <iostream>
#define PI 3.14159265358979323846

Driving::Driving() {
	dataContainer = DataContainer::getInstance();
}

void Driving::GoTo(double x, double y, double theta)
{
    /// x unit : meter, relative x coordinate from now to destination
    /// y unit : meter, relative y coordinate from now to destination
    /// theta unit: degree, the target heading degree when the destination coordinates are reached.

    double turningR = x + y * tan(-(theta)); /// Turning Radius
    double steerRad;
    if (x > 0) { steerRad = atan(2 / sqrt(pow(turningR, 2) - 1)); }
    else { steerRad = -atan(2 / sqrt(pow(turningR, 2) - 1)); } /// Steer Radian
    std::cout << "turningRadius: " << turningR << std::endl;
    std::cout << "steerRadian: " << steerRad << std::endl;
    double steerDegree = rad2deg(steerRad); /// convert Radian to Degree

    std::cout << "steerDeg: " << steerDegree << std::endl;

    if (-27 < steerDegree < 27) {
        dataContainer->setValue_UtoP_STEER(steerDegree * 71);
        /// Because of the steer value set, can't be over than 2000 : it means the maximum steer degree is 28.16
        /// after time ms, the steer value return to 0.
        std::cout << "exit" << std::endl;
    }
    else { std::cout << "steerDegree is incorrect" << std::endl; } /// TODO: 여기서 maximum x y heading radian 구하기
}


void Driving::Basic() {

	//
	// To do : Implement Basic Driving Algorithm
	//



	std::cout << "drigving start\n";

   
}

void Driving::Mission1() {
	//
	// To do : Implement Basic Driving Algorithm
	//

}

/// functions which switch radian and degree
double Driving::rad2deg(double radian) { return radian * 180 / PI; }
double Driving::deg2rad(double degree) { return degree * PI / 180; }