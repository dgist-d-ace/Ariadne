// This is for executing each mission

#include "MissionAct.h"
#include <iostream>
#define PI 3.14159265358979323846

using namespace std;
using namespace cv;

//READ!!!
//before acting mission, we have to ordert the BRAKE(0)
//smaller onestep is better, but we also need the long step.
//

Driving::Driving() {
	dataContainer = DataContainer::getInstance();

	int initialGPSpoint = 0;

	//////////////////////////////
	////Score Validataion Mat.////
	//////////////////////////////
	Mat cirGray, cirGray2, temp, buffer;
	uint Theta, Theta2;

	for (int i = 0; i < checkTheta.size(); i++) {
		cirGray = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1);
		Theta = 90 + checkTheta.at(i);
		Point2d center(cenX, cenY);

		Point2d step101(cenX + onestep * cos(CV_PI*Theta / 180)*1.15 / 5, cenY - onestep  * sin(CV_PI*Theta / 180) *1.15/ 5);
		Point2d step102(cenX + onestep * cos(CV_PI*Theta / 180) * 2 / 5, cenY - onestep * sin(CV_PI*Theta / 180) * 2 / 5);
		Point2d step103(cenX + onestep * cos(CV_PI*Theta / 180) * 3 / 5, cenY - onestep * sin(CV_PI*Theta / 180) * 3 / 5);
		Point2d step104(cenX + onestep * cos(CV_PI*Theta / 180) * 4 / 5, cenY - onestep * sin(CV_PI*Theta / 180) * 4 / 5);
		Point2d step1(cenX + onestep * cos(CV_PI*Theta / 180), cenY - onestep * sin(CV_PI*Theta / 180));
		//Point2d step15(cenX + onestep*1.5 * cos(CV_PI*Theta / 180), cenY - onestep*1.5 * sin(CV_PI*Theta / 180));
		Point2d step05(cenX + onestep * cos(CV_PI*Theta / 180) / 2, cenY - onestep * sin(CV_PI*Theta / 180) / 2);
		//cirGray, step05, (onestep*0.42), Scalar::all(2), -1, CV_AA, 0);
		//line(cirGray, step101, step1, Scalar::all(30), 80);
		circle(cirGray, step05, onestep*0.4,Scalar::all(3), -1, CV_AA, 0);
		circle(cirGray, step103, onestep*0.4, Scalar::all(3), -1, CV_AA, 0);
		circle(cirGray, step1, onestep*0.4, Scalar::all(3), -1, CV_AA, 0);
		for (int j = 0; j < checkTheta2.size(); j++)
		{
			temp = cirGray.clone();
			cirGray2 = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1);
			Theta2 = 90 + checkTheta2.at(j);
			Point2d step201(step1.x + onestep * cos(CV_PI*Theta2 / 180)/5, step1.y - onestep * sin(CV_PI*Theta2 / 180)/5);
			Point2d step202(step1.x + onestep * cos(CV_PI*Theta2 / 180)*2/5, step1.y - onestep * sin(CV_PI*Theta2 / 180)*2/5);
			Point2d step203(step1.x + onestep * cos(CV_PI*Theta2 / 180)*3/5, step1.y - onestep * sin(CV_PI*Theta2 / 180)*3/5);
			Point2d step204(step1.x + onestep * cos(CV_PI*Theta2 / 180) * 4 / 5, step1.y - onestep * sin(CV_PI*Theta2 / 180) * 4 / 5);
			Point2d step2(step1.x + onestep * cos(CV_PI*Theta2 / 180), step1.y - onestep * sin(CV_PI*Theta2 / 180));
			circle(cirGray2, step1, (onestep*0.2), Scalar::all(4), -1, CV_AA, 0);
			circle(cirGray2, center, (onestep*0.5), Scalar::all(2), -1, CV_AA, 0);
			circle(cirGray2, step203, onestep * 0.4, Scalar::all(2), -1, CV_AA, 0);
			circle(cirGray2, step103, onestep*0.2, Scalar::all(1), -1, CV_AA, 0);

			cirGray2 += temp;
			buffer = cirGray2.clone();
			checkImgs.push_back(buffer);
			//imshow("CHECK", buffer);
			//waitKey(100);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////
	////Fill the Regions where cannot go in, because of max value of steering angle.////
	////////////////////////////////////////////////////////////////////////////////////
	int npt2[] = { 4 };
	//Left area where platform can not go (left 60degrees)
	Point points[1][4];
	points[0][0] = Point(leftEndX, cenY);
	points[0][1] = Point(leftEndX+6, cenY);
	points[0][2] = Point(leftEndX+6,0);
	points[0][3] = Point(leftEndX, 0);
	const Point* pnts[1] = { points[0] };

	//Right area where platform can not go (right 60degrees)
	Point points2[1][4];
	points2[0][0] = Point(rightEndX, cenY);
	points2[0][1] = Point(rightEndX-6, cenY);
	points2[0][2] = Point(rightEndX-6, 0);
	points2[0][3] = Point(rightEndX, 0);
	const Point* pnts2[1] = { points2[0] };

	//Drawing
	fillPoly(outRange, pnts, npt2, 1, Scalar::all(255));
	fillPoly(outRange, pnts2, npt2, 1, Scalar::all(255));

	setPath();
}

void Driving::setPath() {
	double path_x;
	double path_y;

	//경로 설정 바꿔야함
	//
	//ifstream gpsfile("C:\\Users\\D-Ace\\Documents\\Ariadne\\Ariadne\\p_k_city_middle_2_final.txt"); 
	
	//DGIST test
	//ifstream gpsfile("C:\\Users\\D-Ace\\Documents\\Ariadne\\Ariadne\\p_dgist_test_1.txt"); 
	
	//First GPS map
	//ifstream gpsfile("C:\\Users\\D-Ace\\Documents\\Ariadne\\Ariadne\\p_k_city_middle_1_final.txt");
	
	//Second GPS map
	//ifstream gpsfile("C:\\Users\\D-Ace\\Documents\\Ariadne\\Ariadne\\p_k_city_middle_2_final.txt");

	//Final GPS map data**
	ifstream gpsfile("C:\\Users\\D-Ace\\Documents\\Ariadne\\Ariadne\\p_k_city_middle_2_final_real_final.txt");
	//k_city_rtk_10Hz_1
	
	char line[200];
	string tap;
	vector<string> vec;
	//vector<double> temp;

	if (gpsfile.is_open()) {

		while (gpsfile.getline(line, sizeof(line), '\n')) {
			stringstream str(line);

			while (getline(str, tap, ',')) {
				vec.push_back(tap);
			}

			path_x = (atof(vec[0].c_str()));
			path_y = (atof(vec[1].c_str()));
			Point2d pathPoint = Point2d(path_x, path_y);
			path.push_back(pathPoint);
			vec.clear();
		}
	}
}

//set the "presentGPSpoint" which mean the mission strat point.
void Driving::getGPSinitial(double x_p, double y_p, vector<Point2d> path) {
	int min = 0;
	double temp = 10000000;
	double ref;
	//cout << "getGPSinitial" << endl;

	for (int i = 0; i < path.size(); i++) {
		ref = pow(pow(x_p - path[i].x, 2) + pow(y_p - path[i].y, 2), 0.5);

		if (ref <= temp) {
			min = i;
			temp = ref;
		}
	}
	
	initialGPSpoint = min;
	presentGPSpoint = min;

	cout << "initial: " << initialGPSpoint << endl;
}

//output: vector includes number of "numGPS" waypoints
//refresh the "presentGPSpoint" as the closest point to me.
vector<Point2d> Driving::forPASIV_path(double x_p, double y_p, vector<Point2d> path) {
	//cout << "in forPASIV_path" << endl;
	int min = 0;
	int smin = 0;
	double temp = 10000000;

	for (int i = 0; i < path.size(); i++) {
		double ref = pow(pow(x_p - path[i].x, 2) + pow(y_p - path[i].y, 2), 0.5);
		if (ref <= temp) {
			smin = min; 
			min = i;
			temp = ref;
		}
	}
/*
	if (abs(min - prevIdx) > numGPSMAP) {
		min = prevIdx;
		cout << "index is too far\n";
	}*/
	prevIdx = min;
	/////////////////////need to debug////////////////////
	// 신호등 봐야 하는 교차로 : 2: 좌 , 3 : 직, 4 : 직, 5 : 좌, 6 : 좌, 8 : 직, 9 : 직 번째
	for (int i = 2; i < 10; i++) {
		if( i == 2 || i == 5|| i == 6 ){
			//좌회전
			if (  crossIdx[i][0] - 30 <= min && min <= crossIdx[i][0] -15) {
				cout << "present cross is  " << i << "  th cross" << endl;
				int trafficID = dataContainer->getValue_yolo_trafficID();
				if (trafficID == RED || trafficID == RED_YELLOW || trafficID == YELLOW || trafficID == GREEN){
					dataContainer->setValue_UtoP_SPEED(0);

					dataContainer->setValue_UtoP_BRAKE(50);
					Sleep(300);
					dataContainer->setValue_UtoP_BRAKE(100);
					Sleep(300);
					dataContainer->setValue_UtoP_BRAKE(150);
					Sleep(300);
					dataContainer->setValue_UtoP_BRAKE(200);

					while (trafficID == RED || trafficID == RED_YELLOW || trafficID == YELLOW || trafficID == GREEN || trafficID == 6) {
						trafficID = dataContainer->getValue_yolo_trafficID();
						Sleep(50);
					}
					dataContainer->setValue_UtoP_BRAKE(1);
					
				}
				break;
			}
		}
		else if (i == 3 || i == 4 || i == 8 || i == 9){
			//직진
			if ( crossIdx[i][0] - 30 <= min && min <= crossIdx[i][0]-15) {
				int trafficID = dataContainer->getValue_yolo_trafficID();
				if (trafficID == RED || trafficID == RED_YELLOW || trafficID == RED_LEFT || trafficID == YELLOW ) {
					dataContainer->setValue_UtoP_SPEED(0);

					dataContainer->setValue_UtoP_BRAKE(50);
					Sleep(300);
					dataContainer->setValue_UtoP_BRAKE(100);
					Sleep(300);
					dataContainer->setValue_UtoP_BRAKE(150);
					Sleep(300);
					dataContainer->setValue_UtoP_BRAKE(200);

					while (trafficID == RED || trafficID == RED_YELLOW || trafficID == RED_LEFT || trafficID == YELLOW || trafficID == 6) {
						trafficID = dataContainer->getValue_yolo_trafficID();
						Sleep(50);
					}
					dataContainer->setValue_UtoP_BRAKE(1);
				}
				break;
			}
		}
		
	}
	//if (sqrt(pow(x_p - path[presentGPSpoint].x, 2) + pow(y_p - path[presentGPSpoint].y, 2) > 10 )) {
	//	min = presentGPSpoint + 5;
	//}

	vector<Point2d> result;
	for (int i = min; i < std::min(min + numGPSMAP, (int)path.size()); i++) {
		result.push_back(path[i]);
	}
	initialGPSpoint = min;
	presentGPSpoint = min;
	cout << "initial!!!!!!!!!!!!!!!!!!!:   " << initialGPSpoint << endl;
	return result;
}

vector<Point2d> Driving::getWaypoint(double x_p, double y_p, double heading, vector<Point2d>forPASIV_path) {
	double theta = - heading;

	//cout << "getWayPoint" << endl;
	vector<Point2d> gpsWayPoint;
	double imgPX;
	double imgPY;
	if (forPASIV_path.size() == 0) {
	}
	else {
		for (int i = 0; i < forPASIV_path.size(); i++) {
			imgPX = ((forPASIV_path[i].x - x_p) * cos(theta) + (forPASIV_path[i].y - y_p) * sin(theta) + 3.0)*200.0 / 3.0;
			imgPY = ((forPASIV_path[i].x - x_p) * sin(theta) - (forPASIV_path[i].y - y_p) * cos(theta) + 6.0)*200.0 / 3.0;
			Point2d temp = Point2d(imgPX, imgPY);
			//Point2d temp = Point2d(200.0 / 3.0 * ((forPASIV_path[i].x - x_p) * cos(theta) + (forPASIV_path[i].y - y_p ) * sin(theta) + 3.0), 200.0 / 3.0 * ((forPASIV_path[i].x - x_p) * sin(theta) - (forPASIV_path[i].y - y_p ) * cos(theta) + 6.0 ));
			//Point2d temp = Point2d(200 / 3 * (forPASIV_path[i].x - x_p + 3), -200 / 3 * (forPASIV_path[i].y - y_p + 6));
			//vector<double> temp{ 200/3*(forPASIV_path[i].x - x_p + 3), -200/3* (forPASIV_path[i].y - y_p + 6)};
			gpsWayPoint.push_back(temp);
			//temp.clear();
		}
	}
	return gpsWayPoint;
}

//Input: lane data from scnn
//output: 400x400 Mat image
//Make the scored lane map 
//Tuning Point:
//**vector about steering angle(or checking angle)
//**onestep
//**do or dont fill the area meaning out of steering angle range
//**kernal size for obstacle,  size of lane.
//**value of lane, obstacle
//**speed decision.(ratio)
//Manual tuning: scoreStep & itvLane
Mat Driving::getLaneData(int scorestep)
{
	//the form of existLanes: vector<int> like [ll, l, r, rr], (0: None, 1: White, 2: Blue, 3: Yellow)
	//the form of lanes: vector<vector<Point2i>> [[the points of ll],[the points of l],[the points of r],[the points of rr]]
	Mat bufferImg = Mat::zeros(800, 800, CV_8UC1);
	bufferImg = Scalar::all(scorestep*(10 + 3));//130
	vector<int> idLane = dataContainer->getValue_scnn_existLanes();
	vector<vector<Point2i>> Lanes = dataContainer->getValue_scnn_lanes();
	Mat bufferImgR = Mat::zeros(800, 800, CV_8UC1);
	Mat bufferImgL = Mat::zeros(800, 800, CV_8UC1);
	//cout << idLane[0] << idLane[1] << idLane[2] << idLane[3] << endl;
	if (idLane[0] == 0 && idLane[1] == 0 && idLane[2] == 0 && idLane[3] == 0) {
		//cout<< "there is no lane!!!!!" <<endl;
	}
	else {
		Point2i endUp;
		Point2i endDown;

		//check the l, ll lanes
		for (int i = 1; i > -1; i--) {
			if (idLane[i] == 0) {}
			else {
				endUp = Point(0, 0);
				endDown = Point(0, bufferImg.rows);
				vector<Point2i> lineContour1, lineContour2, lineContour3, lineContour4, lineContour5, lineContour6, lineContour7, lineContour8, lineContour9;
				lineContour1.push_back(endDown);
				//lineContour2.push_back(endDown);
				//lineContour3.push_back(endDown);
				//lineContour4.push_back(endDown);
				//lineContour5.push_back(endDown);
				//lineContour6.push_back(endDown);
				//lineContour7.push_back(endDown);
				//lineContour8.push_back(endDown);
				//lineContour9.push_back(endDown);
				for (int j = 0; j < Lanes.at(i).size(); j++)
				{
					lineContour1.push_back(Lanes.at(i).at(j));
					//lineContour2.push_back(Point2i(Lanes.at(i).at(j).x + itvLane, Lanes.at(i).at(j).y));
					//lineContour3.push_back(Point2i(Lanes.at(i).at(j).x + itvLane * 2, Lanes.at(i).at(j).y));
					//lineContour4.push_back(Point2i(Lanes.at(i).at(j).x + itvLane * 3, Lanes.at(i).at(j).y));
					//lineContour5.push_back(Point2i(Lanes.at(i).at(j).x + itvLane * 4, Lanes.at(i).at(j).y));
					//lineContour6.push_back(Point2i(Lanes.at(i).at(j).x + itvLane * 5, Lanes.at(i).at(j).y));
					//lineContour7.push_back(Point2i(Lanes.at(i).at(j).x + itvLane * 6, Lanes.at(i).at(j).y));
					//lineContour8.push_back(Point2i(Lanes.at(i).at(j).x + itvLane * 7, Lanes.at(i).at(j).y));
					//lineContour9.push_back(Point2i(Lanes.at(i).at(j).x + itvLane * 8, Lanes.at(i).at(j).y));

				}
				lineContour1.push_back(endUp);
				//lineContour2.push_back(endUp);
				//lineContour3.push_back(endUp);
				//lineContour4.push_back(endUp);
				//lineContour5.push_back(endUp);
				//lineContour6.push_back(endUp);
				//lineContour7.push_back(endUp);
				//lineContour8.push_back(endUp);
				//lineContour9.push_back(endUp);

				const Point2i *pts1 = (const Point2i*)Mat(lineContour1).data;
				//const Point2i *pts2 = (const Point2i*)Mat(lineContour2).data;
				//const Point2i *pts3 = (const Point2i*)Mat(lineContour3).data;
				//const Point2i *pts4 = (const Point2i*)Mat(lineContour4).data;
				//const Point2i *pts5 = (const Point2i*)Mat(lineContour5).data;
				//const Point2i *pts6 = (const Point2i*)Mat(lineContour6).data;
				//const Point2i *pts7 = (const Point2i*)Mat(lineContour7).data;
				//const Point2i *pts8 = (const Point2i*)Mat(lineContour8).data;
				//const Point2i *pts9 = (const Point2i*)Mat(lineContour9).data;

				int ptNum = Mat(lineContour1).rows;
				//fillPoly(bufferImgL, &pts9, &ptNum, 1, Scalar::all(scorestep * 2));
				//fillPoly(bufferImgL, &pts8, &ptNum, 1, Scalar::all(scorestep * 1));
				//fillPoly(bufferImgL, &pts7, &ptNum, 1, Scalar::all(scorestep * 0));
				//fillPoly(bufferImgL, &pts6, &ptNum, 1, Scalar::all(scorestep * 0));
				//fillPoly(bufferImgL, &pts5, &ptNum, 1, Scalar::all(scorestep * 2));
				//fillPoly(bufferImgL, &pts4, &ptNum, 1, Scalar::all(scorestep * 4));
				//fillPoly(bufferImgL, &pts3, &ptNum, 1, Scalar::all(scorestep * 6));
				//fillPoly(bufferImgL, &pts2, &ptNum, 1, Scalar::all(scorestep * 8));
				fillPoly(bufferImgL, &pts1, &ptNum, 1, Scalar::all(scorestep * (7 * idLane[i])));
				
				//if (idLane[2]==0)
				//{
				//	Mat bufferIv = Mat::zeros(800, 800, CV_8UC1);
				//	fillPoly(bufferIv, &pts9, &ptNum, 1, Scalar(scorestep, scorestep, scorestep));
				//	threshold(bufferIv, bufferIv, 1, scorestep * 15, THRESH_BINARY_INV);
				//	bufferImgL += bufferIv;
				//}
				break;
			}
		}

		//check the r, rr lines
		for (int i = 2; i < idLane.size(); i++) {
			if (idLane[i] == 0) {}
			else {

				endUp = Point(bufferImg.cols, 0);
				endDown = Point(bufferImg.cols, bufferImg.rows);
				vector<Point2i> lineContour1, lineContour2, lineContour3, lineContour4, lineContour5, lineContour6, lineContour7, lineContour8, lineContour9;
				lineContour1.push_back(endDown);
				//lineContour2.push_back(endDown);
				//lineContour3.push_back(endDown);
				//lineContour4.push_back(endDown);
				//lineContour5.push_back(endDown);
				//lineContour6.push_back(endDown);
				//lineContour7.push_back(endDown);
				//lineContour8.push_back(endDown);
				//lineContour9.push_back(endDown);

				for (int j = 0; j < Lanes.at(i).size(); j++)
				{
					lineContour1.push_back(Lanes.at(i).at(j));
					//lineContour2.push_back(Point2i(Lanes.at(i).at(j).x - itvLane, Lanes.at(i).at(j).y));
					//lineContour3.push_back(Point2i(Lanes.at(i).at(j).x - itvLane * 2, Lanes.at(i).at(j).y));
					//lineContour4.push_back(Point2i(Lanes.at(i).at(j).x - itvLane * 3, Lanes.at(i).at(j).y));
					//lineContour5.push_back(Point2i(Lanes.at(i).at(j).x - itvLane * 4, Lanes.at(i).at(j).y));
					//lineContour6.push_back(Point2i(Lanes.at(i).at(j).x - itvLane * 5, Lanes.at(i).at(j).y));
					//lineContour7.push_back(Point2i(Lanes.at(i).at(j).x - itvLane * 6, Lanes.at(i).at(j).y));
					//lineContour8.push_back(Point2i(Lanes.at(i).at(j).x - itvLane * 7, Lanes.at(i).at(j).y));
					//lineContour9.push_back(Point2i(Lanes.at(i).at(j).x - itvLane * 8, Lanes.at(i).at(j).y));
				}
				lineContour1.push_back(endUp);
				//lineContour2.push_back(endUp);
				//lineContour3.push_back(endUp);
				//lineContour4.push_back(endUp);
				//lineContour5.push_back(endUp);
				//lineContour6.push_back(endUp);
				//lineContour7.push_back(endUp);
				//lineContour8.push_back(endUp);
				//lineContour9.push_back(endUp);

				const Point2i *pts1 = (const Point2i*)Mat(lineContour1).data;
				//const Point2i *pts2 = (const Point2i*)Mat(lineContour2).data;
				//const Point2i *pts3 = (const Point2i*)Mat(lineContour3).data;
				//const Point2i *pts4 = (const Point2i*)Mat(lineContour4).data;
				//const Point2i *pts5 = (const Point2i*)Mat(lineContour5).data;
				//const Point2i *pts6 = (const Point2i*)Mat(lineContour6).data;
				//const Point2i *pts7 = (const Point2i*)Mat(lineContour7).data;
				//const Point2i *pts8 = (const Point2i*)Mat(lineContour8).data;
				//const Point2i *pts9 = (const Point2i*)Mat(lineContour9).data;

				int ptNum = Mat(lineContour1).rows;

				//fillPoly(bufferImgR, &pts9, &ptNum, 1, Scalar::all(scorestep * 2));
				//fillPoly(bufferImgR, &pts8, &ptNum, 1, Scalar::all(scorestep * 1));
				//fillPoly(bufferImgR, &pts7, &ptNum, 1, Scalar::all(scorestep * 0));
				//fillPoly(bufferImgR, &pts6, &ptNum, 1, Scalar::all(scorestep * 0));
				//fillPoly(bufferImgR, &pts5, &ptNum, 1, Scalar::all(scorestep * 2));
				//fillPoly(bufferImgR, &pts4, &ptNum, 1, Scalar::all(scorestep * 4));
				//fillPoly(bufferImgR, &pts3, &ptNum, 1, Scalar::all(scorestep * 6));
				//fillPoly(bufferImgR, &pts2, &ptNum, 1, Scalar::all(scorestep * 8));
				fillPoly(bufferImgR, &pts1, &ptNum, 1, Scalar::all(scorestep * (7 * idLane[i])));

				//if (idLane[1] == 0)
				//{
				//	Mat bufferIv = Mat::zeros(800, 800, CV_8UC1);
				//	fillPoly(bufferIv, &pts9, &ptNum, 1, Scalar(scorestep, scorestep, scorestep));
				//	threshold(bufferIv, bufferIv, 1, scorestep * 15, THRESH_BINARY_INV);
				//	bufferImgR += bufferIv;
				//}

				break;
			}
		}
	}

	bufferImg = bufferImgR + bufferImgL;
	//imshow("M", bufferImg);
	bufferImg = bufferImg(Range(800 - 400, 800), Range(400 - 200, 400 + 200));
	resize(bufferImg, bufferImg, Size(400, 400), 0, 0, CV_INTER_NN);	//resize the image for be same the size of locLidar data
	///resize(bufferImg, bufferImg, Size(600, 600), 0, 0, CV_INTER_NN);	//resize the image for be same the size of locLidar data
	//imshow("buffer", bufferImg);
	return bufferImg;
}

Mat Driving::getGpsData(int scorestep)
{
	//cout << "getting the data from gps." << endl;
	int mission = dataContainer->getValue_yolo_missionID();
	int stepNum = 4;
	vector<Point2d> wayPoints;
	double gps_X = dataContainer->getValue_gps_latitude();
	double gps_Y = dataContainer->getValue_gps_longitude();
	double gps_H = dataContainer->getValue_gps_heading();

	wayPoints = getWaypoint(gps_X, gps_Y, gps_H, forPASIV_path(gps_X, gps_Y, path));

	Mat gpsMap = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1);
	gpsMap = Scalar::all(scorestep*stepNum);//100

	//Function
	Mat buffer = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1);
	Point2d getPoint;
	for (int i = 1; i < stepNum+1; i++){
		buffer = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1);
		for (int j = 0; j < wayPoints.size(); j++){
			getPoint = Point2d(wayPoints.at(j).x , wayPoints.at(j).y);

			//cout << i << "th Map - " << j << "th point: " << "wayPoint x-> " << wayPoints.at(j).x << "y-> " << wayPoints.at(j).y << endl;
			
			circle(buffer, getPoint, itvGPS * (stepNum+1-i), Scalar::all(scorestep), -1, CV_AA, 0);
		}
		gpsMap -= buffer;
		//cout << "========================================================\n\n" << endl;
	}
	//gpsMap: GPS points: 0 -> 20 -> 40 -> 60 -> 80(background)
	//imshow("GPSmap", gpsMap);
	return gpsMap;
}

void Driving::PASIVcontrol(Mat imgPath, double desired_speed, double steer1, double steer2, double desired_brake)
{
	//function
	double present_steer = (double)dataContainer->getValue_PtoU_STEER()/(-71.0);
	double present_speed = (double)dataContainer->getValue_PtoU_SPEED()/(10.0);
	double present_brake = (double)dataContainer->getValue_PtoU_BRAKE();

	double steer_ratio;
	double steerKp;
	double bustNkid = (dataContainer->getValue_speed_ratio_bust())*(dataContainer->getValue_speed_ratio_kid());
	
	//SPEED control with steering angle
	steer_ratio = (1.0 - abs(steer2) / 85.0) * (1.0 - abs(steer1) / 85.0);
	//cout << "steer_ratio: " << steer_ratio << endl;
	desired_speed *= steer_ratio;
	desired_speed = desired_speed * bustNkid;

	//SPEED control with comparing the present_speed;(decide the brake and speed)
	double speed_ratio = (double)present_speed / desired_speed; //0.925 * 0.84 = 0.777 ~ 1.0
	if (desired_speed == 0){
		desired_brake = 200;
		desired_speed = 0;
	} 
	else if (speed_ratio < 0.85) {
		desired_brake = 0;
		desired_speed += (desired_speed - present_speed)*0.5;
	}
	else if (speed_ratio > 1.15) {
		desired_brake = 40;
		desired_speed = desired_speed;
	}
	else {
		desired_brake = 0;
		desired_speed = desired_speed;
	}

	if (desired_speed < speedLow) desired_speed = speedLow;
	else if (desired_speed > speedHigh) desired_speed = speedHigh;

	//Decide the Kp of steering and control steering angle
	//minimum speed = 5-> 1.0, 15-> 0.3
	double desired_steering;
	if (objflag == 0){
		steerKp = 1.00 - 0.05*desired_speed;
		desired_steering = steer1 * steerKp;
		//cout << "NOOOO near object" << endl;
	}
	else if (objflag > 0) {
		steerKp = 1;
		desired_steering = steer1 * steerKp;
		desired_steering *= 1.1;
		desired_speed *= 0.5;
		//cout << "YESSSSSSSSSSSSSS NEAR obj" << endl;
	}
	//cout << "KP: " << steerKp << " desired_speed: " << desired_speed << endl;
	
	
	/*if (abs(steer1) > 10 && abs(steer2) >= 4 ) {
		desired_steering *= 1.2;
	}
	else if (abs(steer1) > 15) {
		desired_steering*=1.3 ;
	}*/

	Point2d locLidar(cenX, cenY);
	Point2d pntF(cenX + onestep * 2.0* (desired_speed/speedHigh) * cos(CV_PI*(90 + desired_steering) / 180), cenY - onestep * 2.0* (desired_speed / speedHigh)*sin(CV_PI*(90 + desired_steering) / 180));
	Point2d pntF_(cenX + onestep * 2.0* (desired_speed / speedHigh) * cos(CV_PI*(90 + present_steer) / 180), cenY - onestep * 2.0* (desired_speed / speedHigh)*sin(CV_PI*(90 + present_steer) / 180));

	arrowedLine(imgPath, locLidar, pntF, Scalar::all(200), 3);
	arrowedLine(imgPath, locLidar, pntF_, Scalar::all(100), 3);

	if (desired_brake>150) { 
		desired_steering = 0; 
	}

	//cout << "desired_speed = " << (double)desired_speed << "   present_speed: "<< (double)present_speed<< endl;
	//cout <<"desired_steer = " << (double)desired_steering << "   present_steer: " << (double)present_steer << endl;
	//cout << "desired_brake = " << (double)desired_brake << "   present_brake: " << (double)present_brake << endl;
	
	//Return
	//dataContainer->setValue_UtoP_SPEED(desired_speed*10.0);
	dataContainer->setValue_UtoP_SPEED(desired_speed*10.0);
	dataContainer->setValue_UtoP_STEER(desired_steering*(-71.0)); // [deg]
	dataContainer->setValue_UtoP_BRAKE(desired_brake);

}

//PASIV (Pointcloud-based Auto-driving with Score Implemented Voronoi field)
void Driving::Basic(int missionId) {
	cout << "PASIV driving" << endl;
	dataContainer->setValue_UtoP_AorM(1);
	clock_t start, end;

	while (1) {
		start = clock();
		///////////////////////////////////////////
		////Break the PASIV for another mission////
		///////////////////////////////////////////
		int mission = dataContainer->getValue_yolo_missionID();
		if (mission != missionId)
		{
			cout << "called in PASIV but wrong mission ID : " << mission << endl;
			break;
		}
		///////////////////////////////////////////
		imgPath = cv::Mat::zeros(400, 400, CV_8UC1);				//path made with lanes and objs
		scoreMap = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1); //lane map without objs

		vector<Point2d> vecXY = dataContainer->getValue_lidar_VecXY();
		vector<Point2d> vecXYDraw;

		//Localization of LiDAR in the ROI
		Point2d locLidar(cenX, cenY);
		
		//////////////////////////////////////////////////////////////////
		////Because Of Obstacles, Fill the Regions where cannot go in.////
		//////////////////////////////////////////////////////////////////
		for (int i = 0; i < vecXY.size(); ++i) {
			double xyDrawX = locLidar.x + vecXY[i].x * scale;
			double xyDrawY = locLidar.y - vecXY[i].y * scale;

			Point2d xyDraw(xyDrawX, xyDrawY);
			vecXYDraw.push_back(xyDraw);
		}
		for (int i = 0; i < vecXYDraw.size() - 1; ++i) {
			double dist = sqrt(pow(vecXYDraw[i].x - vecXYDraw[i + 1].x, 2) + pow(vecXYDraw[i].y - vecXYDraw[i + 1].y, 2));

			if (dist <= SICK_SCAN_DIST_OBJECT * scale) {
				if (vecXYDraw[i].x < rightEndX && vecXYDraw[i].x > leftEndX && vecXYDraw[i].y > topEndY) {
					line(imgPath, vecXYDraw[i], vecXYDraw[i + 1], CV_RGB(255, 255, 255), 2);
				}
			}
		}

		vector<vector<double> > objDataSet = dataContainer->getValue_lidar_Data().back();
		Point polypts[1][4];
		double cirCenX, cirCenY, cirCenR;
		double cenDist, touchDist, theta_s, theta_l;
		vector<double>objdist;
		int polyX1, polyY1, polyX2, polyY2, polyX3, polyY3, polyX4, polyY4;
		objflag = 0;

		for (int i = 0; i < objDataSet.size(); i++) {
			cirCenX = locLidar.x + objDataSet[i][0] * scale;
			cirCenY = locLidar.y - objDataSet[i][1] * scale;
			cirCenR = objDataSet[i][2] * scale;
			Point2d cirCen(cirCenX, cirCenY); //locLidar of objs.
			cenDist = sqrt(pow((cirCen.x - cenX), 2) + pow((cirCen.y - cenY), 2));
			
			//distance < 1.2m or (x: -0.8~0.8m and y: 2m)
			if (cenDist < 1200 * scale || (cirCenX > (cenX - 800 * scale) && cirCenX < (cenX + 800 * scale) && cirCenY >(cenY-2000 * scale))) {
				objflag++;
			}

			objdist.push_back(cenDist - cirCenR);
			touchDist = sqrt(pow((cirCen.x - cenX), 2) + pow((cirCen.y - cenY), 2) - pow(cirCenR, 2));
			theta_s = asin(cirCenR / cenDist);
			theta_l = atan(objDataSet[i][1] / objDataSet[i][0]);
			if (theta_l < 0) {
				theta_l += CV_PI;
			}

			polyX1 = (int)(touchDist*cos(theta_l - theta_s) + cenX), polyY1 = (int)(cenY - touchDist * sin(theta_l - theta_s));
			polyX2 = (int)(touchDist*cos(theta_l + theta_s) + cenX), polyY2 = (int)(cenY - touchDist * sin(theta_l + theta_s));
			polyX3 = (polyX2 - cenX)*(cenY - topEndY) / (cenY - polyY2) + cenX, polyY3 = topEndY;
			polyX4 = (polyX1 - cenX)*(cenY - topEndY) / (cenY - polyY1) + cenX, polyY4 = topEndY;
			if (polyY1 >= cenY) {
				polyX4 = rightEndX;
				polyY4 = bottomEndY;
			}
			if (polyY2 >= cenY) {
				polyX3 = leftEndX;
				polyY3 = bottomEndY;
			}
			polypts[0][0] = Point(polyX1, polyY1);
			polypts[0][1] = Point(polyX2, polyY2);
			polypts[0][2] = Point(polyX3, polyY3);
			polypts[0][3] = Point(polyX4, polyY4);
			const Point* ppt[1] = { polypts[0] };
			int npt[] = { 4 };
			fillPoly(imgPath, ppt, npt, 1, CV_RGB(200, 200, 200));
			circle(imgPath, cirCen, cirCenR, CV_RGB(255, 255, 255), -1, CV_AA);
		}

		///////////////////////////////////////			
		////Make the Score Implemented map.////
		///////////////////////////////////////		//BIG change 0823
		threshold(imgPath, imgPath, 1, 25, THRESH_BINARY_INV);
		threshold(scoreMap, scoreMap, 1, 25, THRESH_BINARY_INV);

		Mat _window = Mat::ones(15, 15, CV_8UC1);
		morphologyEx(imgPath, imgPath, MORPH_ERODE, _window);

		//SCORE IMPLEMENTED VORONOI FIELD
		int kerSize;
		Mat kernel;
		Mat stepVot = Mat::zeros(imgPath.cols, imgPath.rows, CV_8UC1);
		Mat stepVot2 = Mat::zeros(imgPath.cols, imgPath.rows, CV_8UC1);
		for (int i = 1; i < 4; i++)
		{
			kerSize = 17 * i;
			kernel = Mat::ones(kerSize, kerSize, CV_8UC1);
			morphologyEx(scoreMap, stepVot2, MORPH_ERODE, kernel);
			morphologyEx(imgPath, stepVot, MORPH_ERODE, kernel);
			imgPath += stepVot;
			scoreMap += stepVot2;
		}

		//scoreMap: 200 , Max value of imgPath: 200 
	
		////Apply the lane data to the locLidar data
		Mat laneImg = getLaneData(LanescoreStep);
		scoreMap -= laneImg;
		imgPath -= laneImg;

		/////////////////////////////////////////////////////////////////////
		////Determine the desired Speed in Score System with Vornoi Field////
		/////////////////////////////////////////////////////////////////////
		//Add the line data in the scoreMap and img Path.
		double scoreofMap = 0; //total sum of scoreMap
		double scoreofPath = 0;//total sum of imgPath
		uchar *map = scoreMap.data;
		uchar *path = imgPath.data;
		int mapH = imgPath.rows;
		int mapW = imgPath.cols;
		for (int i = 0; i < mapH; i++) {
			for (int j = 0; j < mapW; j++) {
				scoreofMap += map[i*mapW + j];
				scoreofPath += path[i*mapW + j];
			}
		}
		double scoreRatio = scoreofPath / scoreofMap;
		double desired_speed;
		desired_speed = speedHigh * scoreRatio;

		//////////////////////////////////////////////////////////////////////////////
		////Determine the desired Steering Angle in Score System with Vornoi Field////
		//////////////////////////////////////////////////////////////////////////////
		uint scoresize = checkTheta.size()*checkTheta2.size();
		vector<double> score;
		score.reserve(scoresize);
		Mat scresult;
		double sum;
		for (int i = 0; i < checkImgs.size(); i++) {
			scresult = checkImgs[i].mul(imgPath);
			uchar *sumData = scresult.data;
			int scHeight = scresult.rows;
			int scWidth = scresult.cols;
			sum = 0;
			for (int h = 0; h < scHeight; h++) {
				for (int w = 0; w < scWidth; w++) {
					sum += sumData[w*scHeight + h];
				}
			}
			score.push_back(sum);
		}

		uint scoreMax = distance(score.begin(), max_element(score.begin(), score.end()));
		uint scoreMin = distance(score.begin(), min_element(score.begin(), score.end()));
		int goTheta1 = checkTheta.at(scoreMax / checkTheta2.size());
		int goTheta2 = checkTheta2.at(scoreMax % checkTheta2.size());
		int dangerTheta = checkTheta.at(scoreMin / checkTheta2.size());

		Point2d stepFirst(cenX + onestep * cos(CV_PI*(90 + goTheta1) / 180), cenY - (onestep*sin(CV_PI*(90 + goTheta1) / 180)));
		Point2d stepSecond(stepFirst.x + onestep * 3.0 / 5.0 * cos(CV_PI*(90 + goTheta2) / 180), stepFirst.y - (onestep*3.0 / 5.0*sin(CV_PI*(90 + goTheta2) / 180)));

		arrowedLine(imgPath, locLidar, stepFirst, Scalar::all(50), 5);
		arrowedLine(imgPath, stepFirst, stepSecond, Scalar::all(50), 5);

			///////////////////////
			////Extra Condition////
			///////////////////////
		
		double desired_brake;
		if (objdist.size() == 0) {
			desired_brake = 0;
		}
		else {
			//stop condition
			//if there is any object within 600mm, STOP
			//if the score is lower then threshold
			double objClose = objdist.at(distance(objdist.begin(), min_element(objdist.begin(), objdist.end())));

			if (objClose < 550 * scale) {
				//similary to Emergency Stop
				desired_speed = 0;
				desired_brake = 200;
				//cout << "STOP!!!!!!!TOO CLOSE!!!!" << endl;
			}
			else if (desired_speed < speedLow) {
				//limit the speed
				desired_brake = 0;
				//desired_speed = speedLow; cout << "LOW SCORE!!" << endl;
			}
			else {
				desired_brake = 0;
				desired_speed = desired_speed;
			}
		}
		
		PASIVcontrol(imgPath, desired_speed, goTheta1, goTheta2, desired_brake);
		QImage image1 = QImage((uchar*)imgPath.data, imgPath.cols, imgPath.rows, imgPath.step, QImage::Format_Grayscale8);
		dataContainer->setValue_ui_pathmap(image1);

		end = clock();
		cout << "lidar time: " << (double)(end - start) / 1000 << "sec" << endl;
		int key = cv::waitKey(1);
		if (key == 27) {
			break;
		}
	}
}

//PASIV with GPS
void Driving::BasicGPS(int missionId) {
	cout << "PASIV driving with GPS" << endl;
	dataContainer->setValue_UtoP_AorM(1);
	clock_t start, end;

	//GPS initialization
	getGPSinitial(dataContainer->getValue_gps_latitude(), dataContainer->getValue_gps_longitude(), path);
	while (1) {
		start = clock();
		///////////////////////////////////////////
		////Break the PASIV for another mission////
		///////////////////////////////////////////
		/*if (initialGPSpoint + numGPS <= presentGPSpoint)
		{
			initialGPSpoint = presentGPSpoint;
			cout << "GPs end" << endl;
			break;
		}*/
		int mission = dataContainer->getValue_yolo_missionID();
		if (mission == PARKING) {
			break;
		}
		if (mission == 0)
		{
			cout << "called in PASIV but wrong mission ID : " << mission << endl;
			break;
		}
		
		///////////////////////////////////////////
		imgPath = cv::Mat::zeros(400, 400, CV_8UC1);				//path made with lanes and objs
		scoreMap = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1); //lane map without objs

		vector<Point2d> vecXY = dataContainer->getValue_lidar_VecXY();
		vector<Point2d> vecXYDraw;

		//Localization of LiDAR in the ROI
		Point2d locLidar(cenX, cenY);

		//////////////////////////////////////////////////////////////////
		////Because Of Obstacles, Fill the Regions where cannot go in.////
		//////////////////////////////////////////////////////////////////
		for (int i = 0; i < vecXY.size(); ++i) {
			double xyDrawX = locLidar.x + vecXY[i].x * scale;
			double xyDrawY = locLidar.y - vecXY[i].y * scale;

			Point2d xyDraw(xyDrawX, xyDrawY);
			vecXYDraw.push_back(xyDraw);
		}
		for (int i = 0; i < vecXYDraw.size() - 1; ++i) {
			double dist = sqrt(pow(vecXYDraw[i].x - vecXYDraw[i + 1].x, 2) + pow(vecXYDraw[i].y - vecXYDraw[i + 1].y, 2));

			if (dist <= SICK_SCAN_DIST_OBJECT * scale) {
				if (vecXYDraw[i].x < rightEndX && vecXYDraw[i].x > leftEndX && vecXYDraw[i].y > topEndY) {
					line(imgPath, vecXYDraw[i], vecXYDraw[i + 1], CV_RGB(255, 255, 255), 2);
				}
			}
		}

		vector<vector<double> > objDataSet = dataContainer->getValue_lidar_Data().back();
		Point polypts[1][4];
		double cirCenX, cirCenY, cirCenR;
		double cenDist, touchDist, theta_s, theta_l;
		vector<double>objdist;
		int polyX1, polyY1, polyX2, polyY2, polyX3, polyY3, polyX4, polyY4;
		objflag = 0;

		for (int i = 0; i < objDataSet.size(); i++) {
			cirCenX = locLidar.x + objDataSet[i][0] * scale;
			cirCenY = locLidar.y - objDataSet[i][1] * scale;
			cirCenR = objDataSet[i][2] * scale;
			Point2d cirCen(cirCenX, cirCenY); //locLidar of objs.
			cenDist = sqrt(pow((cirCen.x - cenX), 2) + pow((cirCen.y - cenY), 2));

			//distance < 1.2m or (x: -0.8~0.8m and y: 2m)
			if (cenDist < 1200 * scale || (cirCenX > (cenX - 800 * scale) && cirCenX < (cenX + 800 * scale) && cirCenY >(cenY - 2500 * scale))) {
				objflag++;
			}

			objdist.push_back(cenDist - cirCenR);
			touchDist = sqrt(pow((cirCen.x - cenX), 2) + pow((cirCen.y - cenY), 2) - pow(cirCenR, 2));
			theta_s = asin(cirCenR / cenDist);
			theta_l = atan(objDataSet[i][1] / objDataSet[i][0]);
			if (theta_l < 0) {
				theta_l += CV_PI;
			}

			polyX1 = (int)(touchDist*cos(theta_l - theta_s) + cenX), polyY1 = (int)(cenY - touchDist * sin(theta_l - theta_s));
			polyX2 = (int)(touchDist*cos(theta_l + theta_s) + cenX), polyY2 = (int)(cenY - touchDist * sin(theta_l + theta_s));
			polyX3 = (polyX2 - cenX)*(cenY - topEndY) / (cenY - polyY2) + cenX, polyY3 = topEndY;
			polyX4 = (polyX1 - cenX)*(cenY - topEndY) / (cenY - polyY1) + cenX, polyY4 = topEndY;
			if (polyY1 >= cenY) {
				polyX4 = rightEndX;
				polyY4 = bottomEndY;
			}
			if (polyY2 >= cenY) {
				polyX3 = leftEndX;
				polyY3 = bottomEndY;
			}
			polypts[0][0] = Point(polyX1, polyY1);
			polypts[0][1] = Point(polyX2, polyY2);
			polypts[0][2] = Point(polyX3, polyY3);
			polypts[0][3] = Point(polyX4, polyY4);
			const Point* ppt[1] = { polypts[0] };
			int npt[] = { 4 };
			fillPoly(imgPath, ppt, npt, 1, CV_RGB(200, 200, 200));
			circle(imgPath, cirCen, cirCenR, CV_RGB(255, 255, 255), -1, CV_AA);
		}

		///////////////////////////////////////			
		////Make the Score Implemented map.////
		///////////////////////////////////////		//BIG change 0823
		threshold(imgPath, imgPath, 1, 25, THRESH_BINARY_INV);
		threshold(scoreMap, scoreMap, 1, 25, THRESH_BINARY_INV);

		Mat _window = Mat::ones(15, 15, CV_8UC1);
		morphologyEx(imgPath, imgPath, MORPH_ERODE, _window);

		//SCORE IMPLEMENTED VORONOI FIELD
		int kerSize;
		Mat kernel;
		Mat stepVot = Mat::zeros(imgPath.cols, imgPath.rows, CV_8UC1);
		Mat stepVot2 = Mat::zeros(imgPath.cols, imgPath.rows, CV_8UC1);
		for (int i = 1; i < 4; i++)
		{
			kerSize = 17 * i;
			kernel = Mat::ones(kerSize, kerSize, CV_8UC1);
			morphologyEx(scoreMap, stepVot2, MORPH_ERODE, kernel);
			morphologyEx(imgPath, stepVot, MORPH_ERODE, kernel);
			imgPath += stepVot;
			scoreMap += stepVot2;
		}

		//////Apply the lane data to the locLidar data
		Mat laneImg = getLaneData(LanescoreStep);
		scoreMap -= laneImg;
		imgPath -= laneImg;

		//Apply GPS data.
		Mat gpsMap = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1);
		gpsMap = getGpsData(GPSscoreStep);
		scoreMap -= gpsMap;
		imgPath -= gpsMap;

		//////Apply the out of range of steering angle
		//scoreMap -= outRange;
		//imgPath -= outRange;

		/////////////////////////////////////////////////////////////////////
		////Determine the desired Speed in Score System with Vornoi Field////
		/////////////////////////////////////////////////////////////////////
		//Add the line data in the scoreMap and img Path.
		double scoreofMap = 0; //total sum of scoreMap
		double scoreofPath = 0;//total sum of imgPath
		uchar *map = scoreMap.data;
		uchar *path = imgPath.data;
		int mapH = imgPath.rows;
		int mapW = imgPath.cols;
		for (int i = 0; i < mapH; i++) {
			for (int j = 0; j < mapW; j++) {
				scoreofMap += map[i*mapW + j];
				scoreofPath += path[i*mapW + j];
			}
		}
		double scoreRatio = scoreofPath / scoreofMap;
		double desired_speed;
		desired_speed = speedHigh * scoreRatio;

		//////////////////////////////////////////////////////////////////////////////
		////Determine the desired Steering Angle in Score System with Vornoi Field////
		//////////////////////////////////////////////////////////////////////////////
		uint scoresize = checkTheta.size()*checkTheta2.size();
		vector<double> score;
		score.reserve(scoresize);
		Mat scresult;
		double sum;
		for (int i = 0; i < checkImgs.size(); i++) {
			scresult = checkImgs[i].mul(imgPath);
			uchar *sumData = scresult.data;
			int scHeight = scresult.rows;
			int scWidth = scresult.cols;
			sum = 0;
			for (int h = 0; h < scHeight; h++) {
				for (int w = 0; w < scWidth; w++) {
					sum += sumData[w*scHeight + h];
				}
			}
			score.push_back(sum);
		}

		uint scoreMax = distance(score.begin(), max_element(score.begin(), score.end()));
		uint scoreMin = distance(score.begin(), min_element(score.begin(), score.end()));
		int goTheta1 = checkTheta.at(scoreMax / checkTheta2.size());
		int goTheta2 = checkTheta2.at(scoreMax % checkTheta2.size());
		int dangerTheta = checkTheta.at(scoreMin / checkTheta2.size());

		Point2d stepFirst(cenX + onestep * cos(CV_PI*(90 + goTheta1) / 180), cenY - (onestep*sin(CV_PI*(90 + goTheta1) / 180)));
		Point2d stepSecond(stepFirst.x + onestep * 3.0 / 5.0 * cos(CV_PI*(90 + goTheta2) / 180), stepFirst.y - (onestep*3.0 / 5.0*sin(CV_PI*(90 + goTheta2) / 180)));

		arrowedLine(imgPath, locLidar, stepFirst, Scalar::all(50), 5);
		arrowedLine(imgPath, stepFirst, stepSecond, Scalar::all(50), 5);

		///////////////////////
		////Extra Condition////
		///////////////////////

		double desired_brake;
		if (objdist.size() == 0) {
			desired_brake = 0;
		}
		else {
			//stop condition
			//if there is any object within 600mm, STOP
			//if the score is lower then threshold
			double objClose = objdist.at(distance(objdist.begin(), min_element(objdist.begin(), objdist.end())));

			if (objClose < 550 * scale) {
				//similary to Emergency Stop
				desired_speed = 0;
				desired_brake = 200;
				//cout << "STOP!!!!!!!TOO CLOSE!!!!" << endl;
			}
			else if (desired_speed < speedLow) {
				//limit the speed
				desired_brake = 0;
				//desired_speed = speedLow; cout << "LOW SCORE!!" << endl;
			}
			else {
				desired_brake = 0;
				desired_speed = desired_speed;
			}
		}

		//imshow("Map", scoreMap);
		PASIVcontrol(imgPath, desired_speed, goTheta1, goTheta2, desired_brake);
		//imshow("Path", imgPath);
		QImage image1 = QImage((uchar*)imgPath.data, imgPath.cols, imgPath.rows, imgPath.step, QImage::Format_Grayscale8);
		dataContainer->setValue_ui_pathmap(image1);

		end = clock();
		//cout << "lidar time: " << (double)(end - start) / 1000 << "sec" << endl;
		int key = cv::waitKey(1);
		if (key == 27) {
			break;
		}
	}
}

//Mission No.1: Parking
void Driving::MissionParking() {
	cout << "parking start" << endl;

	//
	//	mission code
	//
	dataContainer->setValue_UtoP_SPEED(0);
	dataContainer->setValue_UtoP_BRAKE(200);
	Sleep(1000);
	dataContainer->setValue_UtoP_BRAKE(0);

	if (ParkingMission()) {}
	else cout << "Parking Fail" << endl;

	//미션이 끝났을 시, yolo에서 다른 mission trigger를 주지 않으면 basic으로 넘어감
	if (dataContainer->getValue_yolo_missionID() == PARKING)
		dataContainer->setValue_yolo_missionID(BASIC);
}

//Mission No.2: Intersection Ready
void Driving::MissionBusLane() {
	cout << "mission 2" << endl;

	//
	//mission code
	//
	BasicGPS(BUSLANE);

	//미션이 끝났을 시, yolo에서 다른 mission trigger를 주지 않으면 basic으로 넘어감
	if (dataContainer->getValue_yolo_missionID() == BUSLANE)
		dataContainer->setValue_yolo_missionID(BASIC);
}

//Mission No.3: Intersection->Turn Left

void Driving::MissionIntLeft() {
	cout << "mission 3" << endl;
	//
	//mission code
	//
	BasicGPS(INTER_LEFT);
	//미션이 끝났을 시, yolo에서 다른 mission trigger를 주지 않으면 basic으로 넘어감
	if (dataContainer->getValue_yolo_missionID() == INTER_LEFT)
		dataContainer->setValue_yolo_missionID(BASIC);
}

//Mission No.4: Intersection->Turn Right
void Driving::MissionIntRight() {
	cout << "mission 4" << endl;
	//
	//mission code
	//
	BasicGPS(INTER_RIGHT);

	//미션이 끝났을 시, yolo에서 다른 mission trigger를 주지 않으면 basic으로 넘어감
	if (dataContainer->getValue_yolo_missionID() == INTER_RIGHT)
		dataContainer->setValue_yolo_missionID(BASIC);
}

//Mission No.5: Intersection->Go Straight
void Driving::MissionIntStraight() {
	cout << "mission 5" << endl;
	//
	//mission code
	//
	BasicGPS(INTER_STRAIGHT);

	//미션이 끝났을 시, yolo에서 다른 mission trigger를 주지 않으면 basic으로 넘어감
	if (dataContainer->getValue_yolo_missionID() == INTER_STRAIGHT)
		dataContainer->setValue_yolo_missionID(BASIC);
}

//Mission No.6: Intersection->Intersection Stop
void Driving::MissionIntStop() {
	cout << "misison 6" << endl;

	//
	//mission code
	//

	dataContainer->setValue_UtoP_SPEED(0);

	dataContainer->setValue_UtoP_BRAKE(50);
	Sleep(300);
	dataContainer->setValue_UtoP_BRAKE(100);
	Sleep(300);
	dataContainer->setValue_UtoP_BRAKE(150);
	Sleep(300);
	dataContainer->setValue_UtoP_BRAKE(200);

	///emit(greenRight(false)); /// 이렇게 하지 말고 욜로쪽에서 하나 더 만드는게 나을듯
	cout << "Red" << endl;

	while (dataContainer->getValue_yolo_missionID() == INTER_STOP)

	dataContainer->setValue_UtoP_BRAKE(0); /// 브레이크 해제
}

//Mission No.7: Static Obstacle Mission
void Driving::MissionStaticObs() {
	//
	// 정적 장애물 미션
	//
	//Use only PASIV

	cout << "basic start" << endl;
	Basic(STATIC_OBSTACLE);
	//미션이 끝났을 시, yolo에서 다른 mission trigger를 주지 않으면 basic으로 넘어감
	if (dataContainer->getValue_yolo_missionID() == STATIC_OBSTACLE)
		dataContainer->setValue_yolo_missionID(BASIC);
}

//Mission No.8: Dynamic Obstacle Mission
void Driving::MissionDynamicObs() {
	//
	dataContainer->setValue_UtoP_AorM(1);
	cout << "Dynamic Obstacle Mission" << endl;
	// 동적 장애물 미션
	// PASIV for Dynamic Obstacle Mission

	uint dynamicflag = 0;
	Mat LaneMap = Mat::zeros(imgPath.cols, imgPath.rows, CV_8UC1);
	dataContainer->setValue_UtoP_BRAKE(0);
	while (1) {
		//dynamic obstacle 미션 중 yolo에서 다른 mission trigger가 들어오면 그 mission으로 넘어감
		/*
		if (dataContainer->getValue_yolo_missionID() != DYNAMIC_OBSTACLE)
		{
			cout << "mission 8 was called but ended long mission ID : " << dataContainer->getValue_yolo_missionID() << endl;
			break;
		}
		*/

		imgPath = cv::Mat::zeros(400, 400, CV_8UC1);
		vector<Point2d> vecXY = dataContainer->getValue_lidar_VecXY();
		vector<Point2d> vecXYDraw;

		//Localization of LiDAR in the ROI
		Point2d center(cenX, cenY);

		//////////////////////////////////////////////////////////////////
		////Because Of Obstacles, Fill the Regions where cannot go in.////
		//////////////////////////////////////////////////////////////////
		for (int i = 0; i < vecXY.size(); ++i) {
			double xyDrawX = center.x + vecXY[i].x * scale;
			double xyDrawY = center.y - vecXY[i].y * scale;

			Point2d xyDraw(xyDrawX, xyDrawY);
			vecXYDraw.push_back(xyDraw);
		}
		for (int i = 0; i < vecXYDraw.size() - 1; ++i) {
			double dist = sqrt(pow(vecXYDraw[i].x - vecXYDraw[i + 1].x, 2) + pow(vecXYDraw[i].y - vecXYDraw[i + 1].y, 2));

			if (dist <= SICK_SCAN_DIST_OBJECT * scale) {
				if (vecXYDraw[i].x < rightEndX && vecXYDraw[i].x > leftEndX && vecXYDraw[i].y > topEndY) {
					line(imgPath, vecXYDraw[i], vecXYDraw[i + 1], CV_RGB(255, 255, 255), 2);
				}
			}
		}

		vector<vector<double>> objDataSet = dataContainer->getValue_lidar_Data().back();
		Point polypts[1][4];

		double cirCenX, cirCenY, cirCenR;
		double cenDist, touchDist, theta_s, theta_l;;
		vector<double>objdist;
		int polyX1, polyY1, polyX2, polyY2, polyX3, polyY3, polyX4, polyY4;

		for (int i = 0; i < objDataSet.size(); i++) {
			cirCenX = center.x + objDataSet[i][0] * scale;
			cirCenY = center.y - objDataSet[i][1] * scale;
			cirCenR = objDataSet[i][2] * scale;
			Point2d cirCen(cirCenX, cirCenY); //locLidar of objs.
			cenDist = sqrt(pow((cirCen.x - cenX), 2) + pow((cirCen.y - cenY), 2));
			if (cirCenY < (center.y - 10) && (abs(cenDist - cirCenR) > 0.001)) {
				objdist.push_back(cenDist - cirCenR);
			}

			touchDist = sqrt(pow((cirCen.x - cenX), 2) + pow((cirCen.y - cenY), 2) - pow(cirCenR, 2));
			theta_s = asin(cirCenR / cenDist);
			theta_l = atan(objDataSet[i][1] / objDataSet[i][0]);
			if (theta_l < 0) {
				theta_l += CV_PI;
			}
			polyX1 = (int)(touchDist*cos(theta_l - theta_s) + cenX), polyY1 = (int)(cenY - touchDist * sin(theta_l - theta_s));
			polyX2 = (int)(touchDist*cos(theta_l + theta_s) + cenX), polyY2 = (int)(cenY - touchDist * sin(theta_l + theta_s));
			polyX3 = (polyX2 - cenX)*(cenY - topEndY) / (cenY - polyY2) + cenX, polyY3 = topEndY;
			polyX4 = (polyX1 - cenX)*(cenY - topEndY) / (cenY - polyY1) + cenX, polyY4 = topEndY;
			if (polyY1 >= cenY) {
				polyX4 = rightEndX;
				polyY4 = bottomEndY;
			}
			if (polyY2 >= cenY) {
				polyX3 = leftEndX;
				polyY3 = bottomEndY;
			}
			polypts[0][0] = Point(polyX1, polyY1);
			polypts[0][1] = Point(polyX2, polyY2);
			polypts[0][2] = Point(polyX3, polyY3);
			polypts[0][3] = Point(polyX4, polyY4);
			const Point* ppt[1] = { polypts[0] };
			int npt[] = { 4 };
			fillPoly(imgPath, ppt, npt, 1, CV_RGB(200, 200, 200));
			circle(imgPath, cirCen, cirCenR, CV_RGB(255, 255, 255), -1, CV_AA);
		}
		//cv::cvtColor(imgPath, imgPath, CV_BGR2GRAY);
		threshold(imgPath, imgPath, 1, 160, THRESH_BINARY_INV);
		LaneMap = Scalar::all(160);
		Mat imgLane = getLaneData(LanescoreStep);
		imgPath -= imgLane;
		LaneMap -= imgLane;

		//////////////////////////////////////////////////////////////////////////////
		////Determine the desired Steering Angle in Score System with Vornoi Field////
		//////////////////////////////////////////////////////////////////////////////
		//REGION OF WORKABLE ANGLE: 60 ~ 120, with interval=5 degrees
		uint scoresize = checkTheta.size()*checkTheta2.size();
		vector<double> score;
		score.reserve(scoresize);
		Mat scresult;
		double sum;
		for (int i = 0; i < checkImgs.size(); i++) {
			scresult = checkImgs[i].mul(imgPath);
			uchar *sumData = scresult.data;
			int scHeight = scresult.rows;
			int scWidth = scresult.cols;
			sum = 0;
			for (int h = 0; h < scHeight; h++) {
				for (int w = 0; w < scWidth; w++) {
					sum += sumData[w*scHeight + h];
				}
			}

			score.push_back(sum);
		}

		uint scoreMax = distance(score.begin(), max_element(score.begin(), score.end()));
		int goTheta1 = checkTheta.at(scoreMax / checkTheta2.size());
		int goTheta2 = checkTheta2.at(scoreMax % checkTheta2.size());

		Point2d stepFirst(cenX + onestep * cos(CV_PI*(90 + goTheta1) / 180), cenY - (onestep*sin(CV_PI*(90 + goTheta1) / 180)));
		Point2d stepSecond(stepFirst.x + onestep * cos(CV_PI*(90 + goTheta2) / 180), stepFirst.y - (onestep*sin(CV_PI*(90 + goTheta2) / 180)));

		arrowedLine(imgPath, center, stepFirst, CV_RGB(50, 50, 50), 5);
		arrowedLine(imgPath, stepFirst, stepSecond, CV_RGB(50, 50, 50), 5);

		/////////////////////////////////////////////////////////////////////
		////Determine the desired Speed in Score System with Vornoi Field////
		/////////////////////////////////////////////////////////////////////
		double scoreLaneMap;
		double scorePath;
		uchar *path = imgPath.data;
		uchar *lane = LaneMap.data;
		for (int h = 0; h < imgPath.rows; h++) {
			for (int w = 0; w < imgPath.cols; w++) {
				scoreLaneMap += lane[w*imgPath.rows + h];
				scorePath += path[w*imgPath.rows + h];
			}
		}

		double desired_speed, desired_brake;
		desired_speed = DynamicMaxSpeed * scorePath / scoreLaneMap;

		double thDist = stopDist * scale;
		circle(imgPath, center, thDist, Scalar::all(255));

		double objClose = -1;
		if (objdist.size() == 0) {
		}
		else {
			objClose = objdist.at(distance(objdist.begin(), min_element(objdist.begin(), objdist.end())));
			if (objClose > 0 && objClose < thDist) {
				desired_brake = 200;
				desired_speed = 0;
				dynamicflag = 1;
				cout << "STOP!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			}
			else {
			}
		}
		//////////////////////////////////////////////////
		////Final Control the steering angle and speed////
		//////////////////////////////////////////////////
		//imshow("Map", LaneMap);
		//imshow("Path", imgPath);
		PASIVcontrol(imgPath, desired_speed, goTheta1, goTheta2, desired_brake);
		QImage image1 = QImage((uchar*)imgPath.data, imgPath.cols, imgPath.rows, imgPath.step, QImage::Format_Grayscale8);
		dataContainer->setValue_ui_pathmap(image1);
		///////////////////////////////////////////////////////
		////Trigger for ending the Dynamic Obstacle Mission////
		///////////////////////////////////////////////////////
		if (dynamicflag == 1) {
			if (objClose > thDist || objdist.size() == 0) {
				dynamicflag = 0;
				dataContainer->setValue_UtoP_BRAKE(0);
				dataContainer->setValue_yolo_missionID(BASIC);
				break;
			}
		}

		///////////////////////////////////////////////////////
		int key = cv::waitKey(1);
		if (key == 27) {
			break;
		}
	}
	//미션이 끝났을 시, yolo에서 다른 mission trigger를 주지 않으면 basic으로 넘어감
	if (dataContainer->getValue_yolo_missionID() == DYNAMIC_OBSTACLE)
		dataContainer->setValue_yolo_missionID(BASIC);
}


//GPS functions
vector<Point2d>Driving::WaySimul_straight() {
	double path_x;
	double path_y;

	ifstream gpsfile("C:\\Users\\D-Ace\\Documents\\Ariadne\\Ariadne\\test1.txt");

	char line[200];
	string tap;
	vector<string> vec;
	Point2d temp;
	vector<Point2d> gpsWayPoint;

	if (gpsfile.is_open()) {
		while (gpsfile.getline(line, sizeof(line), '\n')) {

			stringstream str(line);

			while (getline(str, tap, ',')) {
				vec.push_back(tap);
			}

			path_x = (atof(vec[0].c_str()));
			path_y = (atof(vec[1].c_str()));
			temp = Point2d(path_x, path_y);
			/*temp.push_back(path_x);
			temp.push_back(path_y);*/
			gpsWayPoint.push_back(temp);

			vec.clear();
			//temp.clear();
		}
	}
	gpsfile.close();
	return gpsWayPoint;
}

vector<Point2d>Driving::WaySimul_turn() {
	double path_x;
	double path_y;

	ifstream gpsfile("C:\\Users\\D-Ace\\Documents\\Ariadne\\Ariadne\\test2.txt");

	char line[200];
	string tap;
	vector<string> vec;
	Point2d temp;
	vector<Point2d> gpsWayPoint;

	if (gpsfile.is_open()) {
		while (gpsfile.getline(line, sizeof(line), '\n')) {

			stringstream str(line);

			while (getline(str, tap, ',')) {
				vec.push_back(tap);
			}

			path_x = (atof(vec[0].c_str()));
			path_y = (atof(vec[1].c_str()));
			temp = Point2d(path_x, path_y);
			gpsWayPoint.push_back(temp);

			vec.clear();
		}
	}
	gpsfile.close();
	return gpsWayPoint;
}


//// functions which switch radian and degree
double Driving::rad2deg(double radian) { return radian * 180 / PI; }
double Driving::deg2rad(double degree) { return degree * PI / 180; }

void Driving::autoMode() {
	//emit(send2View(7));
	cout << "!!!!!!!!!!!!!!!!! parkingNum   =   " <<parkingNum << endl;
	while (1) {
		cout << dataContainer->getValue_yolo_missionID() << endl;

		if (dataContainer->getValue_yolo_missionID() == 0) { emit(currentMission(0)); Sleep(1000);	}
		else if (parkingNum == 0 && dataContainer->getValue_yolo_missionID() == PARKING) {
			emit(currentMission(PARKING));
			parkingNum++;
			MissionParking();
			emit(exitMission(PARKING));
		}
		else if (dataContainer->getValue_yolo_missionID() == BUSLANE) {
			emit(currentMission(BUSLANE));
			emit(send2View(2));
			MissionBusLane();
			emit(exitMission(BUSLANE));
		}
		else if (dataContainer->getValue_yolo_missionID() == INTER_LEFT) { 
			emit(currentMission(INTER_LEFT)); 
			emit(send2View(2)); 
			MissionIntLeft(); 
		}
		else if (dataContainer->getValue_yolo_missionID() == INTER_RIGHT) { 
			emit(currentMission(INTER_RIGHT)); 
			emit(send2View(3)); 
			MissionIntRight(); 
		}
		else if (dataContainer->getValue_yolo_missionID() == INTER_STRAIGHT) { 
			(currentMission(INTER_STRAIGHT)); 
			MissionIntStraight(); 
		}
		else if (dataContainer->getValue_yolo_missionID() == STATIC_OBSTACLE) { 
			emit(currentMission(STATIC_OBSTACLE)); 
			MissionStaticObs(); 
		}
		else if (dataContainer->getValue_yolo_missionID() == DYNAMIC_OBSTACLE) { 
			emit(currentMission(DYNAMIC_OBSTACLE)); 
			MissionDynamicObs(); 
		}
		else if (dataContainer->getValue_yolo_missionID() == INTER_STOP) { 
			emit(currentMission(INTER_STOP)); 
			MissionIntStop(); 
		}
		else { 
			emit(currentMission(BASIC)); 
			emit(send2View(1)); 
			BasicGPS(BASIC); 
		}
	}
}

void Driving::DrawData()
{
	/*
	Mat img = Mat::zeros(MAPY, MAPX, CV_8UC3);
	vector<Point2d> vecXY = dataContainer->getValue_lidar_VecXY();
	vector<Point2d> vecXYDraw;
	double cenX = img.cols * 0.5, cenY = img.rows * 0.95;
	double scale = cenY / (SICK_SCAN_ROI_Y); // 창 너비 / 실제 좌우 너비
	double leftEndX = cenX - SICK_SCAN_ROI_X * scale;
	double rightEndX = cenX + SICK_SCAN_ROI_X * scale;
	double topEndY = BOT_L;
	//double bottomEndY = cenY + SICK_SCAN_ROI_Y * scale;
	double platEndY = cenY - 50;
	Point2d locLidar(cenX, cenY), platEnd(cenX, platEndY);
	Point2d leftTopEnd(leftEndX, topEndY), leftBottomEnd(leftEndX, cenY);
	Point2d rightTopEnd(rightEndX, topEndY), rightBottomEnd(rightEndX, cenY);
	//locLidar = platEnd;
	//////////////////////////////////////////////////////////////////////////////
	//fill the Regions where cannot go in, because of obstacles.
	for (int i = 0; i < vecXY.size(); ++i) { //스케일 조정
		double xyDrawX = locLidar.x + vecXY[i].x * scale;
		double xyDrawY = locLidar.y - vecXY[i].y * scale;
		Point2d xyDraw(xyDrawX, xyDrawY);
		vecXYDraw.push_back(xyDraw);
	}
	for (int i = 0; i < vecXYDraw.size() - 1; ++i) { //물체를 구성하는 점 연결
		double dist = sqrt(pow(vecXYDraw[i].x - vecXYDraw[i + 1].x, 2) + pow(vecXYDraw[i].y - vecXYDraw[i + 1].y, 2));
		if (dist <= SICK_SCAN_DIST_OBJECT * scale) {
			if (vecXYDraw[i].x < rightEndX && vecXYDraw[i].x > leftEndX && vecXYDraw[i].y > topEndY) {
				//line(img, vecXYDraw[i], vecXYDraw[i + 1], CV_RGB(0, 255, 0), 2);
			}
		}
	}
	vector<vector<double> > objDataSet = dataContainer->getValue_lidar_Data().back();
	Point polypts[1][4];
	for (int i = 0; i < objDataSet.size(); i++) { //스케일 조정
		double cirCenX = locLidar.x + objDataSet[i][0] * scale;
		double cirCenY = locLidar.y - objDataSet[i][1] * scale;
		double cirCenR = objDataSet[i][2] * scale;
		Point2d cirCen(cirCenX, cirCenY); //물체를 나타내는 원 그리기
		//circle(img, cirCen, objDataSet[i][2] * scale, CV_RGB(255, 0, 0), -1, CV_AA);
		double cenDist = sqrt(pow((cirCen.x - cenX), 2) + pow((cirCen.y - cenY), 2));
		double touchDist = sqrt(pow((cirCen.x - cenX), 2) + pow((cirCen.y - cenY), 2) - pow(cirCenR, 2));
		double theta_s = asin(cirCenR / cenDist);
		double theta_l = atan(objDataSet[i][1] / objDataSet[i][0]);
		if (theta_l < 0)
		{
			theta_l += CV_PI;
		}
		int x1 = (int)(touchDist*cos(theta_l - theta_s) + cenX), y1 = (int)(cenY - touchDist * sin(theta_l - theta_s));
		int x2 = (int)(touchDist*cos(theta_l + theta_s) + cenX), y2 = (int)(cenY - touchDist * sin(theta_l + theta_s));
		int x3 = (x2 - cenX)*(cenY - topEndY) / (cenY - y2) + cenX, y3 = topEndY;
		int x4 = (x1 - cenX)*(cenY - topEndY) / (cenY - y1) + cenX, y4 = topEndY;
		if (y1 > cenY) {
			x4 = x4 * (-1);
			y4 = cenY - 0.0001;
		}
		if (y2 > cenY) {
			x3 = x3 * (-1);
			y3 = cenY - 0.0001;
		}
		polypts[0][0] = Point(x1, y1);
		polypts[0][1] = Point(x2, y2);
		polypts[0][2] = Point(x3, y3);
		polypts[0][3] = Point(x4, y4);
		const Point* ppt[1] = { polypts[0] };
		int npt[] = { 4 };
		//fillPoly(img, ppt, npt, 1, CV_RGB(200, 200, 0));
		circle(img, cirCen, cirCenR, CV_RGB(255, 0, 0), -1, CV_AA);
		//////////////////////////////////////////////////////////////////////////////
	}
	//arrowedLine(img, locLidar, platEnd, CV_RGB(0, 150, 150), 2); //라이다 본체 벡터
	//////////////////////////////////////////////////////////////////////////////
	//fill the Regions where cannot go in, because of max value of steering angle.
	Point points[1][3];
	points[0][0] = Point(cenX+BOT_W, cenY);
	points[0][1] = Point(cenX + SICK_SCAN_ROI_X, cenY);
	points[0][2] = Point(cenX + SICK_SCAN_ROI_X, cenY - SICK_SCAN_ROI_X * sqrt(3));
	const Point* pnts[1] = { points[0] };
	int npt2[] = { 3 };
	fillPoly(img, pnts, npt2, 1, CV_RGB(150, 0, 0));
	//ellipse(img, leftBottomEnd, Size(BOT_L/tan(BOT_M_ALPHA*PI/180), BOT_L/tan(BOT_M_ALPHA*PI/180)), 0, -90, 0, Scalar(255, 0, 0), -1); // 왼쪽 원
	Point points2[1][3];
	points2[0][0] = Point(cenX- BOT_W, cenY);
	points2[0][1] = Point(cenX - SICK_SCAN_ROI_X, cenY);
	points2[0][2] = Point(cenX - SICK_SCAN_ROI_X, cenY - SICK_SCAN_ROI_X * sqrt(3));
	const Point* pnts2[1] = { points2[0] };
	fillPoly(img, pnts2, npt2, 1, CV_RGB(150, 0, 0));
	//ellipse(img, rightBottomEnd, Size(BOT_L/tan(BOT_M_ALPHA*PI/180), BOT_L/tan(BOT_M_ALPHA*PI/180)), 0, 180, 270, Scalar(255, 0, 0), -1); //오른쪽 원
	rectangle(img, Point2d(0, 0), Point2d(img.cols, img.rows/2), CV_RGB(0, 0, 0), -1, CV_AA, 0); // 상
	//rectangle(img, Point2d(img.cols, img.rows), Point2d(0, cenY), CV_RGB(255, 255, 255), -1, CV_AA, 0); // 하
	rectangle(img, Point2d(0, 0), Point2d(leftEndX, img.rows), CV_RGB(255, 255, 255), -1, CV_AA, 0); // 좌
	rectangle(img, Point2d(img.cols, img.rows), Point2d(rightEndX, 0), CV_RGB(255, 255, 255), -1, CV_AA, 0); // 우
	//rectangle(img, Point2d(cenX + BOT_W, img.rows), Point2d(cenX - BOT_W, cenY - BOT_W / sqrt(3)), CV_RGB(0, 0, 0), -1, CV_AA, 0); // 중
	/////////////////      R    O    I    -    E    N    D       ////////////////////
	Mat route;
	cv::cvtColor(img, route, CV_BGR2GRAY);
	threshold(route, route, 1, 255, THRESH_BINARY_INV);
	img = route;
	imshow("locLidar", img);
	dataContainer->setValue_lidar_ImgData(img);
	*/
}

void Driving::LOS() {
	/*
	dataContainer->setValue_UtoP_AorM(1);
	Planner astar;
	int startX = MAPX / 2, startY = MAPY * 0.95, startTheta = 270, startSteer = 0, startSpeed = 60; // It will be multiplied by 5, so 18 is 90 degree
	int targetX = MAPX / 2, targetY = BOT_L/2, targetTheta = 270, targetSteer = 0, targetSpeed = 60;
	int isInc = 0;
	while (1) {
		DrawData();
		clock_t  begin, end;
		begin = clock();
		//Map mapLiDAR = Map(picLiDAR);
		Map mapLiDAR = Map(dataContainer->getValue_lidar_imgData());
		State start = State(startX, startY, startTheta, startSteer, startSpeed);
		State target = State(targetX, targetY, targetTheta, targetSteer, targetSpeed);
		int finalSteer = astar.plan(start, target, mapLiDAR), finalSpeed;
		if (finalSteer == 90) {
			dataContainer->setValue_UtoP_E_STOP(0);
			dataContainer->setValue_UtoP_STEER(0);
			cout << "final steer : " << finalSteer << ", final speed : " << 0 << " ang gae ggul ddi" << endl;
		}
		else {
			dataContainer->setValue_UtoP_E_STOP(0);
			dataContainer->setValue_UtoP_STEER(finalSteer * 71);
			dataContainer->setValue_UtoP_SPEED(30);
			cout << "final steer : " << finalSteer << ", final speed : " << 30 << " ang gae ggul ddi" << endl;
		}
		mapLiDAR.deleteMaps();
		end = clock();
		cout << "LOS time : " << (double)(end - begin) / 1000 << "s" << endl;
		int key = cv::waitKey(1);
		if (key == 27) {
			break;
		}
	}
	*/
}


Mat Driving::FilteredImage(Mat &img_camera) {
	Mat img_original = img_camera, img_gray, img_edge;

	//그레이 스케일 영상으로 변환 한후.  
	cvtColor(img_original, img_gray, COLOR_BGR2GRAY);

	//가우시안 블러와 캐니에지를 이용하여 에지 성분을 검출합니다.
	//GaussianBlur(img_gray, img_gray, cvSize(3, 3), 0, 0);
	Canny(img_gray, img_edge, 125, 175, 3);
	GaussianBlur(img_edge, img_edge, cvSize(5, 5), 0, 0);

	imshow("Edge", img_edge);

	return img_edge;
}

Mat Driving::FindLotLines(Mat &img_edge) {
	Mat img_lines = img_edge;

	//직선 성분을 검출합니다.
	vector<Vec2f> lines;
	HoughLines(img_lines, lines, 1, CV_PI / 180, 150);

	//검출한 직선을 영상에 그려줍니다.
	for (int i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];

		int x1, x2, y1, y2;

		int xminY = rho / sin(theta), xmaxY = -1 / tan(theta) * img_lines.cols + rho / sin(theta); // xmin = 0, xmax = cols
		int yminX = rho / cos(theta), ymaxX = -tan(theta) * img_lines.rows + rho / cos(theta); // ymin = 0, ymax = rows

		// 선분의 종류
		if (yminX >= 0 && yminX <= img_lines.cols) { // top
			x1 = yminX;
			y1 = 0;

			if (xminY >= 0 && xminY <= img_lines.rows) { // left
				x2 = 0;
				y2 = xminY;
			}
			else if (ymaxX >= 0 && ymaxX <= img_lines.cols) { // bottom
				x2 = ymaxX;
				y2 = img_lines.rows;
			}
			else if (xmaxY >= 0 && xmaxY <= img_lines.rows) { // right
				x2 = img_lines.cols;
				y2 = xmaxY;
			}
		}
		else if (xminY >= 0 && xminY <= img_lines.rows) { // left
			x1 = 0;
			y1 = xminY;

			if (ymaxX >= 0 && ymaxX <= img_lines.cols) { // bottom
				x2 = ymaxX;
				y2 = img_lines.rows;
			}
			else if (xmaxY >= 0 && xmaxY <= img_lines.rows) { // right
				x2 = img_lines.cols;
				y2 = xmaxY;
			}
		}
		else if (ymaxX >= 0 && ymaxX <= img_lines.cols) { // bottom
			x1 = ymaxX;
			y1 = img_lines.rows;

			if (xmaxY >= 0 && xmaxY <= img_lines.rows) { // right
				x2 = img_lines.cols;
				y2 = xmaxY;
			}
		}

		line(img_lines, Point(x1, y1), Point(x2, y2), Scalar(255, 0, 0), 1);
	}

	// 검출한 직선의 교점을 영상에 그려줍니다.
	for (int i = 0; i < lines.size() - 1; i++) {
		for (int j = i; j < lines.size(); j++) {
			float rho1 = lines[i][0], theta1 = lines[i][1];
			float rho2 = lines[j][0], theta2 = lines[j][1];

			int x = (rho2 / sin(theta2) - rho1 / sin(theta1)) / (1 / tan(theta2) - 1 / tan(theta1));
			int y = (rho2 / cos(theta2) - rho1 / cos(theta1)) / (tan(theta2) - tan(theta1));

			circle(img_lines, Point(x, y), 3, Scalar(0, 255, 0));
		}
	}
	QImage image1 = QImage((uchar*)img_lines.data, img_lines.cols, img_lines.rows, img_lines.step, QImage::Format_RGB888);
	dataContainer->setValue_ui_parking(image1);
	//imshow("result", img_lines);

	return img_lines;
}

vector<vector<Point2f> > Driving::FindLotContours(Mat &img_camera, Mat &img_edge) {
	Mat img_result = img_camera, img_contours = img_edge;

	//contour를 찾는다.
	vector<vector<Point> > contours;
	findContours(img_contours, contours, noArray(), RETR_LIST, CHAIN_APPROX_SIMPLE);

	//contour를 근사화한다.
	vector<vector<Point2f> > finLotContours;
	vector<Point2f> approx;

	for (size_t i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.01, true);

		double area = contourArea(Mat(approx));

		if (area > CAMERA_AREA_MIN && area < CAMERA_AREA_MAX) { //면적이 일정크기 이상이어야 한다. 50000 < 종이 주차장 < 200000
			int size = approx.size();

			if (size == 4 && isContourConvex(Mat(approx))) { //사각형 이상의 볼록다각형만 그린다.
				Point2f center(0, 0);

				for (int j = 0; j < size - 1; j++)
					line(img_result, approx[j], approx[j + 1], Scalar(0, 255, 0), 3);
				line(img_result, approx[0], approx[size - 1], Scalar(0, 255, 0), 3);

				for (int j = 0; j < size; j++) {
					circle(img_result, approx[j], 3, Scalar(0, 0, 255), -1);
					center.x += approx[j].x;
					center.y += approx[j].y;
				}

				center.x = center.x / size;
				center.y = center.y / size;
				circle(img_result, center, 3, Scalar(255, 255, 255), -1);

				approx.push_back(center);

				finLotContours.push_back(approx);
			}
		}
	}

	imshow("Result", img_result);

	return finLotContours;
}

Point2f Driving::SetLotCenter(vector<vector<Point2f> > &contours) {
	Point2f finCenter;

	vector<vector<Point2f> > parkingLots = contours;
	Point2f minFarLot(CAMERA_X, CAMERA_Y / 2);

	for (int i = 0; i < parkingLots.size(); i++) {
		if (parkingLots[i].back().x <= minFarLot.x) {
			minFarLot = parkingLots[i].back();
		}
	}

	finCenter = minFarLot;

	return finCenter;
}

vector<float> Driving::PosFromCamera(Point2f &center) {
	Point2f finCen(center.x - CAMERA_X / 2, CAMERA_Y - center.y);

	vector<float> position;

	position.push_back(CAMERA_H * tan(CAMERA_SIDE_ANGLE * deg_2_rad) * tan(finCen.x * CAMERA_ANGLE / CAMERA_X * deg_2_rad)); // x
	position.push_back(CAMERA_H * tan((finCen.y * (90 - CAMERA_TILT) / CAMERA_Y + CAMERA_SIDE_ANGLE) * deg_2_rad)); // y

	return position;
}

vector<float> Driving::FindParkingLot(Mat &img_camera) {
	// 주차장을 검출하고, 주차장의 중심을 찾아, 카메라를 원점으로 하는 중심의 좌표를 구합니다.
	Mat camera = img_camera;
	Mat edge = FilteredImage(camera);
	vector<vector<Point2f> > contours = FindLotContours(camera, edge);
	Point2f center = SetLotCenter(contours);
	vector<float> position;

	if (center.x >= (CAMERA_X * 3 / 8) && center.x <= (CAMERA_X * 5 / 8) && center.y >= (CAMERA_Y * 3 / 8) && center.y <= (CAMERA_Y * 5 / 8)) {
		cout << center.x << " " << center.y << " can find" << endl;
		position = PosFromCamera(center);
	}
	else {
		center = Point2f(0, 0);
		cout << center.x << " " << center.y << " cannot find" << endl;
		position = { 0, 0 };
	}

	cout << "Final x: " << position[0] << ", y: " << position[1] << endl;

	return position;
}

void Driving::controlSpeed(int speed) {
	cout << "Start to speed control." << endl;

	double m_speed = dataContainer->getValue_PtoU_SPEED();
	double error = abs(m_speed - speed);
	cout << "error before: " << error << endl;

	if (error <= speed_error) {					
		error = 0;
	}

	int f_speed = speed + K * error;
	cout << "Final speed: " << f_speed << ", Error after: " << error << endl;

	dataContainer->setValue_UtoP_SPEED(f_speed);
}

void Driving::brakeTime(double second) {
	cout << "Brake" << endl;

	clock_t start = clock();
	clock_t delay = second * CLOCKS_PER_SEC;
	dataContainer->setValue_UtoP_SPEED(0);
	while (clock() - start <= delay) {
		dataContainer->setValue_UtoP_BRAKE(brake);
	}

	dataContainer->setValue_UtoP_BRAKE(1);

}

void Driving::controlENC(int gear, int speed, double dist, int steer) {
	cout << "Gear " << gear << ", Speed " << (float)(speed / 10) << "km/h, Distance " << dist << endl;
	cout << "Start to controling the Encoder ." << endl;

	dataContainer->setValue_UtoP_GEAR(gear);
	dataContainer->setValue_UtoP_SPEED(speed);
	dataContainer->setValue_UtoP_STEER(steer*(-71.0));

	double b_ENC1 = dataContainer->getValue_PtoU_ENC();
	cout << "b_ENC1: " << b_ENC1 << endl;

	while (1) {
		// 속도 제어하는 코드
		//controlSpeed(speed);
		if (dataContainer->getValue_yolo_missionID() == 0) break;

		//거리 구하는 코드
		double p_ENC1 = dataContainer->getValue_PtoU_ENC();
		waitKey(100);
		double distance = abs(p_ENC1 - b_ENC1) /60.0;
		cout << "b_ENC1: " << b_ENC1 << endl << "p_ENC1 : " << p_ENC1 << endl<< "distance : "<<distance<<endl;

		if (distance >= dist) {
			cout << "moving distance : " << distance << endl;
			dataContainer->setValue_UtoP_SPEED(0);
			dataContainer->setValue_UtoP_BRAKE(200);
			Sleep(1000);
			dataContainer->setValue_UtoP_BRAKE(0);

			break;
		}
	}
}

void Driving::practice(double parkDis) { // 후진 후 좌회전
	double finDist;
	dataContainer->setValue_UtoP_SPEED(0);
	dataContainer->setValue_UtoP_BRAKE(200);
	dataContainer->setValue_UtoP_STEER(0);
	Sleep(1000);
	dataContainer->setValue_UtoP_BRAKE(0);

	if (dataContainer->getValue_yolo_missionID() == 0) return;

	finDist = 0.7 * (park_y - pow(abs(pow(parkDis, 2) - pow(park_x, 2)), 0.5));
	//finDist = 1;
	cout << "dghfhjfjhggjhghj    " << finDist << endl;
	controlENC(2, parking_speed, finDist+0.8);

	cout << "backward success" << endl;
	if (dataContainer->getValue_yolo_missionID() == 0) return;

	finDist = dis_error_rate * circle_path;
	controlENC(0, parking_speed, finDist, -17);

	cout << "turn success" << endl;
	if (dataContainer->getValue_yolo_missionID() == 0) return;

	finDist = 1.3;
	controlENC(0, parking_speed, finDist);
	if (dataContainer->getValue_yolo_missionID() == 0) return;

	cout << "straight success" << endl;

	dataContainer->setValue_UtoP_BRAKE(200);
	//brakeTime(5);
	if (dataContainer->getValue_yolo_missionID() == 0) return;

	Sleep(10000);
	dataContainer->setValue_UtoP_BRAKE(0);
	if (dataContainer->getValue_yolo_missionID() == 0) return;

	finDist = 1.3;
	controlENC(2, parking_speed, finDist);
	if (dataContainer->getValue_yolo_missionID() == 0) return;
	cout << "straight success" << endl;

	finDist = dis_error_rate * circle_path;
	controlENC(2, parking_speed, finDist, -17);
	if (dataContainer->getValue_yolo_missionID() == 0) return;

	cout << "parking the end" << endl;
}

int Driving::ParkingMission() {
	//실시간 카메라 불러오기
	VideoCapture cap(0 + CAP_DSHOW);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, CAMERA_X);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_Y);

	//저장된 동영상 불러오기
	//VideoCapture cap("Picture/Track1.mp4");

	if (!cap.isOpened()) {
		cerr << "Error to open the video.\n";
		return -1;
	}

	Mat img_camera;
	vector<float> finParkPos;

	while (1)
	{
		if (dataContainer->getValue_yolo_missionID() == 0) break;
		
		// 카메라로부터 캡쳐한 영상을 frame에 저장합니다.
		cap >> img_camera;

		if (img_camera.empty()) {
			cerr << "There is no video.\n";
			continue;
		}
		QImage image1 = QImage((uchar*)img_camera.data, img_camera.cols, img_camera.rows, img_camera.step, QImage::Format_RGB888);
		dataContainer->setValue_ui_parking(image1);
		///////////////////////////////////////
		//            //////             //
		//         ///// ☆ /////          //
		//      ///// ☆☆☆☆☆☆ /////       //
		//   ///// ☆☆☆☆☆☆☆☆☆☆☆ ///// //
		///// ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆ /////
		// 주차 전 직진 (PASIV)
		imgPath = Mat::zeros(400, 400, CV_8UC1);
		imgPath = Scalar::all(160);
		Mat gpsMap = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1);
		gpsMap = getGpsData(GPSscoreStep);
		scoreMap -= gpsMap;
		imgPath -= gpsMap;
		//////////////////////////////////////////////////////////////////////////////
		////Determine the desired Steering Angle in Score System with Vornoi Field////
		//////////////////////////////////////////////////////////////////////////////
		//REGION OF WORKABLE ANGLE: 60 ~ 120, with interval=5 degrees
		uint scoresize = checkTheta.size()*checkTheta2.size();
		vector<uint> score;
		score.reserve(scoresize);
		Mat scresult;
		uint sum;
		for (int i = 0; i < checkImgs.size(); i++) {
			scresult = checkImgs[i].mul(imgPath);
			uchar *sumData = scresult.data;
			int scHeight = scresult.rows;
			int scWidth = scresult.cols;
			sum = 0;
			for (int h = 0; h < scHeight; h++) {
				for (int w = 0; w < scWidth; w++) {
					sum += sumData[w*scHeight + h];
				}
			}

			score.push_back(sum);
		}

		uint scoreMax = distance(score.begin(), max_element(score.begin(), score.end()));
		int goTheta1 = checkTheta.at(scoreMax / checkTheta2.size());
		int goTheta2 = checkTheta2.at(scoreMax % checkTheta2.size());
		Point2d center(cenX, cenY);
		Point2d stepFirst(cenX + onestep * cos(CV_PI*(90 + goTheta1) / 180), cenY - (onestep*sin(CV_PI*(90 + goTheta1) / 180)));
		Point2d stepSecond(stepFirst.x + onestep * cos(CV_PI*(90 + goTheta2) / 180), stepFirst.y - (onestep*sin(CV_PI*(90 + goTheta2) / 180)));

		arrowedLine(imgPath, center, stepFirst, CV_RGB(50, 50, 50), 5);
		arrowedLine(imgPath, stepFirst, stepSecond, CV_RGB(50, 50, 50), 5);


		// 눈감고 10m 직진
		//controlENC(0, parking_speed, 10.0, 0);
		double distParking = 0;
		//주차장 검출
		finParkPos = FindParkingLot(img_camera);
		distParking = sqrt(pow(finParkPos[0], 2) + pow(finParkPos[1], 2));
		cout << "merong " << distParking << endl;
		//주차 시작
		if (distParking != 0) {
			cout << "The distance between camera and parking area: " << distParking << endl;
			practice(distParking);
			dataContainer->setValue_UtoP_GEAR(0);
			return 0;
		}
		else {
			PASIVcontrol(imgPath, 20, goTheta1, goTheta2, 0);
		}
		if (dataContainer->getValue_yolo_missionID() == 0)
		{
			cout << "called in PASIV but wrong mission ID : " << endl;
			break;
		}
		///// ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆ /////
		//   ///// ☆☆☆☆☆☆☆☆☆☆☆ ///// //
		//      ///// ☆☆☆☆☆☆ /////       //
		//         ///// ☆ /////          //
		//            //////             //
		///////////////////////////////////////
	}

	return 0;
}
