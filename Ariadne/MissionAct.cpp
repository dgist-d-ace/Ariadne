// This is for executing each mission

#include "MissionAct.h"
#include <iostream>
#define PI 3.14159265358979323846

using namespace cv;
Driving::Driving() {
	dataContainer = DataContainer::getInstance();
}

// input: destination coordinate x, y(m), heading value of destination(radian)
// output: steer Value
int Driving::GoTo(double x, double y, double theta)
{
    /// x unit : meter, relative x coordinate from now to destination
    /// y unit : meter, relative y coordinate from now to destination
    /// theta unit: radian, the target heading degree when the destination coordinates are reached.
    double turningR = abs(x + y / tan(-(theta))); /// Turning Radius
    if (turningR < 0.8)
    {
        if (x > 0) { return 2000; }
        else { return -2000; }
    }
    double steerRad;
    if (x > 0) { steerRad = atan(pow(0.8, 2) / sqrt(pow(turningR, 2) - 0.64)); }
    else { steerRad = -atan(pow(0.8, 2) / sqrt(pow(turningR, 2) - 0.64)); } /// Steer Radian

    double steerDegree = rad2deg(steerRad); /// convert Radian to Degree

    if (-27 < steerDegree && steerDegree < 27) {
        return steerDegree * 71;
        /// Because of the steer value set, can't be over than 2000 : it means the maximum steer degree is 28.16
    }
    else
    {
        if (steerDegree > 0) { return 2000; } ///set steer maximum value
        else { return -2000; }
    }

}


void Driving::Basic() {
	// To do : Implement Basic Driving Algorithm
	//
	dataContainer->setValue_UtoP_AorM(1);


//Plan B: Auto-driving with Score Map Rule (ASMR)
	//Please name this algorithm
	while (1)
	{
		clock_t start, end;
		start = clock();

		//imgPath = cv::Mat::zeros(900, 900, CV_8UC3); //50fps
		imgPath = cv::Mat::zeros(600, 600, CV_8UC3); //100fps
		//imgPath = cv::Mat::zeros(1200, 1200, CV_8UC3); //20fps
		//imgPath = cv::Mat::zeros(768, 1366, CV_8UC3);
		vector<Point2d> vecXY = dataContainer->getValue_lidar_VecXY();
		vector<Point2d> vecXYDraw;

		double cenX = imgPath.cols * 0.5, cenY = imgPath.rows *0.99; //the location of LiDAR in the map.
		double scale = cenY / (SICK_SCAN_ROI_Y+50);				  //obj_Data => imgPath

		//Car size in map
		double carW = CAR_WEITH * scale;
		double carH = CAR_HEIGH * scale;

		double leftEndX = cenX - SICK_SCAN_ROI_X * scale;
		double rightEndX = cenX + SICK_SCAN_ROI_X * scale;
		double topEndY = cenY - SICK_SCAN_ROI_Y * scale;
		double bottomEndY = cenY + SICK_SCAN_ROI_Y * scale;

		//Objects in XY coordinate -> in imgPath
		Point2d center(cenX, cenY);

			////////////////////////////////////////////////////////////////////////////////////
			////Fill the Regions where cannot go in, because of max value of steering angle.////
			////////////////////////////////////////////////////////////////////////////////////
		
		//Left area where platform can not go
		Point points[1][3];
		points[0][0] = Point(center.x - carW, center.y);
		points[0][1] = Point(cenX - SICK_SCAN_ROI_X, center.y);
		points[0][2] = Point(cenX - SICK_SCAN_ROI_X, center.y - (SICK_SCAN_ROI_X - carW)*sqrt(3)); //���߿� �ݰ濡 ���缭 ���������� �׸���

		const Point* pnts[1] = { points[0] };
		int npt2[] = { 3 };

		fillPoly(imgPath, pnts, npt2, 1, CV_RGB(150, 0, 0));

		//Right area where platform can not go
		Point points2[1][3];
		points2[0][0] = Point(center.x + carW, center.y);
		points2[0][1] = Point(cenX + SICK_SCAN_ROI_X, center.y);
		points2[0][2] = Point(cenX + SICK_SCAN_ROI_X, center.y - (SICK_SCAN_ROI_X - carW) * sqrt(3)); //���߿� �ݰ濡 ���缭 ���������� �׸���

		const Point* pnts2[1] = { points2[0] };

		fillPoly(imgPath, pnts2, npt2, 1, CV_RGB(150, 0, 0));
		

			//////////////////////////////////////////////
			////Fill the Regions which mean out of ROI////
			//////////////////////////////////////////////

		//rectangle(imgPath, Point2d(0, 0), Point2d(imgPath.cols, topEndY), CV_RGB(255, 255, 255), -1, CV_AA, 0);
		//rectangle(imgPath, Point2d(0, 0), Point2d(leftEndX, imgPath.rows), CV_RGB(255, 255, 255), -1, CV_AA, 0);
		//rectangle(imgPath, Point2d(imgPath.cols, imgPath.rows), Point2d(0, cenY), CV_RGB(255, 255, 255), -1, CV_AA, 0);
		//rectangle(imgPath, Point2d(imgPath.cols, imgPath.rows), Point2d(rightEndX, 0), CV_RGB(255, 255, 255), -1, CV_AA, 0);
		Mat scoreMap = imgPath;
		//imshow("1", imgPath);

			//////////////////////////////////////////////////////////////////
			////Fill the Regions where cannot go in, because of obstacles.////
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
					line(imgPath, vecXYDraw[i], vecXYDraw[i + 1], CV_RGB(0, 255, 0), 2);
				}
			}
		}

		vector<vector<double> > objDataSet = dataContainer->getValue_lidar_Data().back();
		Point polypts[1][4];
		double cirCenX, cirCenY, cirCenR;
		double cenDist, touchDist, theta_s, theta_l;
		int polyX1, polyY1, polyX2, polyY2, polyX3, polyY3, polyX4, polyY4;
		for (int i = 0; i < objDataSet.size(); i++){
			cirCenX = center.x + objDataSet[i][0] * scale;
			cirCenY = center.y - objDataSet[i][1] * scale;
			cirCenR = objDataSet[i][2] * scale;
			Point2d cirCen(cirCenX, cirCenY); //center of objs.

			//circle(img, cirCen, objDataSet[i][2] * scale, CV_RGB(255, 0, 0), -1, CV_AA);

			cenDist = sqrt(pow((cirCen.x - cenX), 2) + pow((cirCen.y - cenY), 2));
			touchDist = sqrt(pow((cirCen.x - cenX), 2) + pow((cirCen.y - cenY), 2) - pow(cirCenR, 2));
			theta_s = asin(cirCenR / cenDist);
			theta_l = atan(objDataSet[i][1] / objDataSet[i][0]);

			if (theta_l < 0){
				theta_l += CV_PI;
			}

			polyX1 = (int)(touchDist*cos(theta_l - theta_s) + cenX), polyY1 = (int)(cenY - touchDist * sin(theta_l - theta_s));
			polyX2 = (int)(touchDist*cos(theta_l + theta_s) + cenX), polyY2 = (int)(cenY - touchDist * sin(theta_l + theta_s));
			polyX3 = (polyX2 - cenX)*(cenY - topEndY) / (cenY - polyY2) + cenX, polyY3 = topEndY;
			polyX4 = (polyX1 - cenX)*(cenY - topEndY) / (cenY - polyY1) + cenX, polyY4 = topEndY;

			if (polyY1 > cenY) {
				polyX4 = polyX4 * (-1);
				polyY4 = bottomEndY - 0.0001;
			}
			if (polyY2 > cenY) {
				polyX3 = polyX3 * (-1);
				polyY3 = bottomEndY - 0.0001;
			}
			polypts[0][0] = Point(polyX1, polyY1);
			polypts[0][1] = Point(polyX2, polyY2);
			polypts[0][2] = Point(polyX3, polyY3);
			polypts[0][3] = Point(polyX4, polyY4);

			const Point* ppt[1] = { polypts[0] };
			int npt[] = { 4 };

			fillPoly(imgPath, ppt, npt, 1, CV_RGB(200, 200, 0));
			circle(imgPath, cirCen, cirCenR, CV_RGB(255, 0, 0), -1, CV_AA);
		}

			////////////////////////////////////
			////Make the image to Grayscale.////
			////////////////////////////////////

		cv::cvtColor(scoreMap, scoreMap, CV_BGR2GRAY);
		threshold(scoreMap, scoreMap, 1, 10, THRESH_BINARY_INV);
		
		cv::cvtColor(imgPath, imgPath, CV_BGR2GRAY);
		threshold(imgPath, imgPath, 1, 10, THRESH_BINARY_INV);

		//MAKING VORNOI FIELD
		int kerSize;
		for (int i = 1; i < 4; i++)
		{
			kerSize = 20 * i;
			Mat kernel = Mat::ones(kerSize, kerSize, CV_8UC1);


			Mat stepVot = Mat::zeros(imgPath.cols, imgPath.rows, CV_8UC1);
			cv::morphologyEx(imgPath, stepVot, MORPH_ERODE, kernel);
			imgPath += stepVot;
		}
		//uchar *asd = imgPath.data;
		
		//cout << (uint)asd[imgPath.cols*(imgPath.rows/2)+imgPath.cols/2] << endl;
		//cout << (uint)asd[imgPath.cols*(1) + 1] << endl;

			//////////////////////////////////////////////////////////////////////////////
			////Determine the desired Steering Angle in Score System with Vornoi Field////
			//////////////////////////////////////////////////////////////////////////////

		//REGION OF WORKABLE ANGLE: 60 ~ 120, with interval=5 degrees
		uchar onestep = carH*scale; //mean how much car move go in 0.5s. / 12km/hour -> 3m/second
		Mat cirGray;
		Mat cirGray2;
		Mat scresult;
		uint scoreTheta;
		uint scoreTheta2;
		uint sum;
		for (int i = 0; i < theta.size(); i++)
		{
			cirGray = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1);
			scoreTheta = 90 + theta.at(i);
			Point2d step1(cenX + onestep * cos(CV_PI*scoreTheta / 180), cenY - onestep * sin(CV_PI*scoreTheta / 180));
			circle(cirGray, step1, carH / 2, CV_RGB(50, 50, 50), -1, CV_AA, 0);
			for (int j = 0; j < theta.size(); j++)
			{
				cirGray2 = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1);
				scoreTheta2 = 90 + theta.at(j);
				Point2d step2(step1.x + onestep * cos(CV_PI*scoreTheta2 / 180), step1.y -onestep* sin(CV_PI*scoreTheta2 / 180));
				circle(cirGray2, step2, carH / 2, CV_RGB(50,50,50), -1, CV_AA, 0);
				cirGray2 += cirGray;
				//imshow("check Circle img", cirGray2);
				sum = 0;
				scresult = cirGray2.mul(imgPath);
				uchar *sumData = scresult.data;
				int sheight = scresult.rows;
				int swidth = scresult.cols;
				for (int n = 0; n < sheight; n++){
					for (int m = 0; m < swidth; m++){
						sum += sumData[m*sheight + n];}}
				score->push_back(sum);
				//waitKey(1);
				//Sleep(100);
			}
		}

		uint scoreMax = distance(score->begin(), max_element(score->begin(), score->end()));
		int goTheta1 = theta.at(scoreMax / theta.size());
		int goTheta2 = theta.at(scoreMax % theta.size());
		Point2d stepFirst(cenX + onestep * cos(CV_PI*(90 + goTheta1) / 180), cenY - (onestep*sin(CV_PI*(90 + goTheta1) / 180)));
		Point2d stepSecond(stepFirst.x + onestep * cos(CV_PI*(90 + goTheta2) / 180), stepFirst.y - (onestep*sin(CV_PI*(90 + goTheta2) / 180)));
		cv::arrowedLine(imgPath, center, stepFirst, CV_RGB(255, 255, 255), 5);
		cv::arrowedLine(imgPath, stepFirst, stepSecond, CV_RGB(255, 255, 255), 5);
		
		uint desired_steering = goTheta1 * steerRatio + goTheta2 *(1-steerRatio);
		Point2d pntF(cenX + onestep*2 * cos(CV_PI*(90 + desired_steering) / 180), cenY - (onestep*2*sin(CV_PI*(90 + desired_steering) / 180)));
		cv::arrowedLine(imgPath, center, pntF, CV_RGB(150, 150, 150), 5);


		//scoring 1st and 2nd step seperately
		/* 
		//predict first step
		for (int i = 0; i < theta.size(); i++)
		{

			cirGray = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1);
			scoretheta = 90 + theta.at(i);
			Point2d scoreCir(cenX + onestep * cos(CV_PI*scoretheta / 180), cenY - onestep * sin(CV_PI*scoretheta / 180));

			circle(cirGray, scoreCir, carW / 2, CV_RGB(1, 1, 1), -1, CV_AA, 0);
			//imshow("test", cirGray);
			//Mat cirGray;
			//cvtColor(imgCircle, cirGray, CV_RGB2GRAY);
			scresult = cirGray.mul(imgPath);
			sum = 0;
			uchar *sumData = scresult.data;
			int sheight = scresult.rows;
			int swidth = scresult.cols;
			for (int j = 0; j < sheight; j++)
			{
				for (int m = 0; m < swidth; m++)
				{
					//sum += scresult.at<uchar>(j,m);
					sum += sumData[m*sheight + j];
				}
			}
			score->push_back(sum);
			//waitKey(1);
		}

		uint scoreMax1 = distance(score->begin(), max_element(score->begin(), score->end()));
		int goTheta1 = theta.at(scoreMax1);
		Point2d stepFirst(cenX + onestep * cos(CV_PI*(90 + goTheta1) / 180), cenY - (onestep*sin(CV_PI*(90 + goTheta1) / 180)));
		cv::arrowedLine(imgPath, center, stepFirst, CV_RGB(255, 255, 255), 5);


		//Predict second step
		vector<uint> score2[13];
		for (int i = 0; i < theta.size(); i++)
		{
			cirGray = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1);
			scoretheta = 90 + theta.at(i);
			Point2d scoreCir2(stepFirst.x + onestep * cos(CV_PI*scoretheta / 180), stepFirst.y - onestep * sin(CV_PI*scoretheta / 180));

			circle(cirGray, scoreCir2, carW / 4, CV_RGB(1, 1, 1), -1, CV_AA, 0);
			//imshow("test", cirGray);
			//Mat cirGray;
			//cvtColor(imgCircle, cirGray, CV_RGB2GRAY);
			scresult = cirGray.mul(imgPath);
			sum = 0;
			uchar *sumData = scresult.data;
			int sheight = scresult.rows;
			int swidth = scresult.cols;
			for (int j = 0; j < sheight; j++)
			{
				for (int m = 0; m < swidth; m++)
				{
					//sum += scresult.at<uchar>(j,m);
					sum += sumData[m*sheight + j];
				}
			}
			score2->push_back(sum);
			//waitKey(1);
		}
		uint scoreMax2 = distance(score2->begin(), max_element(score2->begin(), score2->end()));
		int goTheta2 = theta.at(scoreMax2);
		Point2d stepSecond(stepFirst.x + onestep * cos(CV_PI*(90 + goTheta2) / 180), stepFirst.y - (onestep*sin(CV_PI*(90 + goTheta2) / 180)));
		cv::arrowedLine(imgPath, stepFirst, stepSecond, CV_RGB(255, 255, 255), 5);
		*/

			/////////////////////////////////////////////////////////////////////
			////Determine the desired Speed in Score System with Vornoi Field////
			/////////////////////////////////////////////////////////////////////
		
		//Add the line data in the scoreMap and img Path.
		
		uint scoreofMap; //total sum of scoreMap
		uint scoreofPath;//total sum of imgPath
		uint speedHigh;
		uint speedLow;
		uint desired_speed;

		//compare the scoreofMap and scoreofPath


			//////////////////////////////////////////////////
			////Final Control the steering angle and speed////
			//////////////////////////////////////////////////
		setData_steering(desired_steering);
		//setData_speed(desired_speed);
		dataContainer->setValue_UtoP_SPEED(30);
		cv::imshow("LaneMap", scoreMap);
		cv::imshow("DrawLiDARData", imgPath);

		end = clock();
		cout << "time: " << (double)(end - start) / 1000 << "sec" << endl ;

		int key = cv::waitKey(1);

		if (key == 27) {
			break;
		}
	}
}

void Driving::setData_speed(int desired_speed)
{
	int present_speed = dataContainer->getValue_PtoU_SPEED(); //0~200
	desired_speed *= 10;

	desired_speed = (desired_speed - present_speed) *speedKP + present_speed;

	if (desired_speed > 200) {
		desired_speed = 200;
	}
	else if (desired_speed < 0) {
		desired_speed = 0;
	}
	dataContainer->setValue_UtoP_SPEED(desired_speed);
}

void Driving::setData_steering(int desired_steering)
{
	int present_steering = dataContainer->getValue_PtoU_STEER(); //-2000~2000
	desired_steering *= -71;

	desired_steering = (desired_steering - present_steering) *steeringKP + present_steering;

	if (desired_steering > 2000) {
		desired_steering = 2000;
	}
	else if (desired_steering < -2000) {
		desired_steering = -2000;
	}                                                                                                    
	dataContainer->setValue_UtoP_STEER(desired_steering);
}


void Driving::Mission1() {
	//
	// To do : Implement Basic Driving Algorithm
	//

}

/// functions which switch radian and degree
double Driving::rad2deg(double radian) { return radian * 180 / PI; }
double Driving::deg2rad(double degree) { return degree * PI / 180; }
