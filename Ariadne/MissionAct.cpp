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

double rad2deg(double radian) { return radian * 180 / PI; }
double deg2rad(double degree) { return degree * PI / 180; }



void Driving::Basic() {

	//
	// To do : Implement Basic Driving Algorithm
	//
	dataContainer->setValue_UtoP_AorM(1);

//Plan B: MAKING VORNOI FEILD & VOTING SYSTEM.
	while (1)
	{
		imgPath = cv::Mat::zeros(768, 1366, CV_8UC3);
		vector<Point2d> vecXY = dataContainer->getValue_lidar_VecXY();
		vector<Point2d> vecXYDraw;


		double cenX = imgPath.cols * 0.5, cenY = imgPath.rows * 0.9;
		double scale = cenY / (SICK_SCAN_ROI_Y + 200); //â ũ�⸦ ���� ������ ����(�ر� ��Ʈ�� : 0.35)

		//Car size in map
		double carW = CAR_WEITH * scale;
		double carH = CAR_HEIGH * scale;

		double leftEndX = cenX - SICK_SCAN_ROI_X * scale;
		double rightEndX = cenX + SICK_SCAN_ROI_X * scale;
		double topEndY = cenY - SICK_SCAN_ROI_Y * scale;
		double bottomEndY = cenY + SICK_SCAN_ROI_Y * scale;

		double platEndY = cenY - 50;
		Point2d center(cenX, cenY), platEnd(cenX, platEndY);
		Point2d leftTopEnd(leftEndX, topEndY), rightBottomEnd(rightEndX, cenY);

		//rectangle(img, leftTopEnd, rightBottomEnd, CV_RGB(150, 150, 150), 2, CV_AA, 0);
		//circle(img, center, 5, CV_RGB(255, 255, 255), -1); //���� LiDAR ��ġ

		for (int i = 0; i < vecXY.size(); ++i) { //������ ����
			double xyDrawX = center.x + vecXY[i].x * scale;
			double xyDrawY = center.y - vecXY[i].y * scale;

			Point2d xyDraw(xyDrawX, xyDrawY);
			vecXYDraw.push_back(xyDraw);
		}

		for (int i = 0; i < vecXYDraw.size() - 1; ++i) { //��ü�� �����ϴ� �� ����
			double dist = sqrt(pow(vecXYDraw[i].x - vecXYDraw[i + 1].x, 2) + pow(vecXYDraw[i].y - vecXYDraw[i + 1].y, 2));

			if (dist <= SICK_SCAN_DIST_OBJECT * scale) {
				if (vecXYDraw[i].x < rightEndX && vecXYDraw[i].x > leftEndX && vecXYDraw[i].y > topEndY) {
					line(imgPath, vecXYDraw[i], vecXYDraw[i + 1], CV_RGB(0, 255, 0), 2);
				}
			}
		}


		/// %%%%%%%%%%%%%%%%%%%%%%%%%%MINHO's native code for steering angle.%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		//////////////////////////////////////////////////////////////////////////////
		//////////Fill the Regions where cannot go in, because of obstacles.//////////
		//////////////////////////////////////////////////////////////////////////////

		vector<vector<double> > objDataSet = dataContainer->getValue_lidar_Data().back();
		Point polypts[1][4];

		for (int i = 0; i < objDataSet.size(); i++)
		{

			double cirCenX = center.x + objDataSet[i][0] * scale;
			double cirCenY = center.y - objDataSet[i][1] * scale;
			double cirCenR = objDataSet[i][2] * scale;
			Point2d cirCen(cirCenX, cirCenY); //��ü�� ��Ÿ���� �� �׸���

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
				y4 = bottomEndY - 0.0001;
			}
			if (y2 > cenY) {
				x3 = x3 * (-1);
				y3 = bottomEndY - 0.0001;
			}
			polypts[0][0] = Point(x1, y1);
			polypts[0][1] = Point(x2, y2);
			polypts[0][2] = Point(x3, y3);
			polypts[0][3] = Point(x4, y4);

			const Point* ppt[1] = { polypts[0] };
			int npt[] = { 4 };

			fillPoly(imgPath, ppt, npt, 1, CV_RGB(200, 200, 0));
			circle(imgPath, cirCen, cirCenR, CV_RGB(255, 0, 0), -1, CV_AA);
		}
		//////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////
		///fill the Regions where cannot go in, because of max value of steering angle.///
		//////////////////////////////////////////////////////////////////////////////////

		//left blind area
		Point points[1][3];
		points[0][0] = Point(center.x - carW, center.y);
		points[0][1] = Point(cenX - SICK_SCAN_ROI_X, center.y);
		points[0][2] = Point(cenX - SICK_SCAN_ROI_X, center.y - (SICK_SCAN_ROI_X - carW)*sqrt(3)); //���߿� �ݰ濡 ���缭 ���������� �׸���

		const Point* pnts[1] = { points[0] };
		int npt2[] = { 3 };

		fillPoly(imgPath, pnts, npt2, 1, CV_RGB(150, 0, 0));

		//right blind area
		Point points2[1][3];
		points2[0][0] = Point(center.x + carW, center.y);
		points2[0][1] = Point(cenX + SICK_SCAN_ROI_X, center.y);
		points2[0][2] = Point(cenX + SICK_SCAN_ROI_X, center.y - (SICK_SCAN_ROI_X - carW) * sqrt(3)); //���߿� �ݰ濡 ���缭 ���������� �׸���

		const Point* pnts2[1] = { points2[0] };

		fillPoly(imgPath, pnts2, npt2, 1, CV_RGB(150, 0, 0));
		//////////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////////
		////////////////////Fill the Regions which mean out of ROI////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		//rectangle(img, Point2d(0, 0), Point2d(img.cols, topEndY), CV_RGB(255, 255, 255), -1, CV_AA, 0);
		//rectangle(img, Point2d(0, 0), Point2d(leftEndX, img.rows), CV_RGB(255, 255, 255), -1, CV_AA, 0);
		//rectangle(img, Point2d(img.cols, img.rows), Point2d(0, cenY), CV_RGB(255, 255, 255), -1, CV_AA, 0);
		//rectangle(img, Point2d(img.cols, img.rows), Point2d(rightEndX, 0), CV_RGB(255, 255, 255), -1, CV_AA, 0);
		//////////////////////////////////////////////////////////////////////////////////

		cv::cvtColor(imgPath, imgPath, CV_BGR2GRAY);
		threshold(imgPath, imgPath, 1, 10, THRESH_BINARY_INV);

		//MAKING VORNOI FIELD
		int kerSize;
		for (int i = 1; i < 5; i++)
		{
			kerSize = 30 * i;
			Mat kernel = Mat::ones(kerSize, kerSize, CV_8UC1);


			Mat stepVot = Mat::zeros(imgPath.cols, imgPath.rows, CV_8UC1);
			cv::morphologyEx(imgPath, stepVot, MORPH_ERODE, kernel);
			imgPath += stepVot;
		}

		//VOTING PART:
		//REGION OF WORKABLE ANGLE: 60 ~ 120, with interval=5 degrees
		vector<uint> score[13]; //include the scores at [90,85, 95, 80, 100, 75, 105, 70, 110, 65, 115, 60, 120]degrees
		uchar onestep = 500 * scale; //mean how much car move go in 0.5s. / 12km/hour -> 3m/second
		vector<int> theta = { 0, -5, 5, -10, 10, -15, 15, -20, 20, -25, 25, -30, 30 }; //The steering angle candidates
		Mat cirGray;
		Mat scresult;
		uint scoretheta;
		uint sum;

		//predict first step
		for (int i = 0; i < theta.size(); i++)
		{

			cirGray = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1);
			scoretheta = 90 + theta.at(i);
			Point2d scoreCir(cenX + onestep * cos(CV_PI*scoretheta / 180), cenY - onestep * sin(CV_PI*scoretheta / 180));

			circle(cirGray, scoreCir, carW / 4, CV_RGB(1, 1, 1), -1, CV_AA, 0);
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
		int goTheta1 = theta.at(scoreMax1); //���� �������� ������ +, �������� - �� ���Ⱒ ����, gotheta�� ���� ���Ⱒ��
		//cout << scoreMax << "th value" << goTheta1 << endl;

		Point2d stepFirst(cenX + onestep * cos(CV_PI*(90 + goTheta1) / 180), cenY - (onestep*sin(CV_PI*(90 + goTheta1) / 180)));
		cv::arrowedLine(imgPath, center, stepFirst, CV_RGB(255, 255, 255), 5);

		//Predict second step-----------------------------------------------------------�̿ϼ�
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


		//DRAW ARROWLINES of LiDAR, and surrounding objects.
		//DRAW the vectors meaning the movement of objects.

		/*
		arrowedLine(img, center, platEnd, CV_RGB(150, 150, 150), 2); //���̴� ��ü ����

		vector<Point2d> vecData = finVecData;
		vector<bool> boolData = finBoolData;

		for (int i = 0; i < vecData.size(); i++) {
			double arrowCenX = center.x + objDataSet[i][0] * scale;
			double arrowCenY = center.y - objDataSet[i][1] * scale;

			double arrowPointX = arrowCenX + vecData[i].x;
			double arrowPointY = arrowCenY - vecData[i].y;

			Point2d arrowCen(arrowCenX, arrowCenY); //ȭ��ǥ ���� ��ǥ
			Point2d arrowPoint(arrowPointX, arrowPointY); //ȭ��ǥ �Ӹ� ��ǥ

			arrowedLine(img, arrowCen, arrowPoint, CV_RGB(0, 255, 255), 2, 8, 0, 0.3); //��ü�� ����

			if (boolData[i] == true) {
				putText(img, "True", arrowCen, FONT_HERSHEY_SIMPLEX, 1, CV_RGB(255, 255, 255));
			}
			else if (boolData[i] == false) {
				putText(img, "False", arrowCen, FONT_HERSHEY_SIMPLEX, 1, CV_RGB(255, 255, 255));
			}
		}*/
		if (goTheta2 > 26)
		{
			goTheta2 = 26;
		}
		else if (goTheta2 < (-26))
		{
			goTheta2 = -26;
		}

		double gotox = (stepSecond.x -imgPath.cols/2)/1000;
		double gotoy = (imgPath.rows *0.9 -stepSecond.y)/1000 ;


		dataContainer->setValue_UtoP_STEER(GoTo(gotox,gotoy,goTheta2));
		dataContainer->setValue_UtoP_SPEED(18);

		printf("x1 = %f x= %f y=%f theta=%d", stepSecond.x, gotox, gotoy, goTheta2);
		cv::imshow("DrawLiDARData", imgPath);


		int key = cv::waitKey(1);

		if (key == 27) {
			break;
		}
	}


}

void Driving::Mission1() {
	//
	// To do : Implement Basic Driving Algorithm
	//

}

/// functions which switch radian and degree
double Driving::rad2deg(double radian) { return radian * 180 / PI; }
double Driving::deg2rad(double degree) { return degree * PI / 180; }
