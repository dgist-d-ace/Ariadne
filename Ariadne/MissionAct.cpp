// This is for executing each mission

#include "MissionAct.h"
#include <iostream>
#define PI 3.14159265358979323846

using namespace std;
using namespace cv;

Driving::Driving() {
	dataContainer = DataContainer::getInstance();
}

//using the data from scnn
//output: 600x600 Mat image
//out of white lane: x*1, out of blue lane: x*2, out of yellow line: x*3
Mat Driving::getLaneData(int scorestep)
{
	//the form of existLanes: vector<int> like [ll, l, r, rr], (0: None, 1: White, 2: Blue, 3: Yellow)
	//the form of lanes: vector<vector<Point2i>> [[the points of ll],[the points of l],[the points of r],[the points of rr]]
	Mat bufferImg = Mat::zeros(800, 800, CV_8UC1);
	vector<int> idLane = dataContainer->getValue_scnn_existLanes();
	vector<vector<Point2i>> Lanes = dataContainer->getValue_scnn_lanes();
	Mat bufferImgR = Mat::zeros(800, 800, CV_8UC1);
	Mat bufferImgL = Mat::zeros(800, 800, CV_8UC1);
	if (idLane[0] == 0 && idLane[1] == 0 && idLane[2] == 0 && idLane[3] == 0) {
		//cout<< "there is no lane!!!!!" <<endl;
	}
	else {
		Point2i endUp;
		Point2i endDown;
		int itvLane = 20;
		//check the l, ll lanes
		for (int i = 1; i > -1; i--) {
			if (idLane[i] == 0) {}
			else {
				endUp = Point(0, 0);
				endDown = Point(0, bufferImg.rows);
				vector<Point2i> lineContour1, lineContour2, lineContour3, lineContour4, lineContour5, lineContour6, lineContour7, lineContour8;
				lineContour1.push_back(endDown);
				lineContour2.push_back(endDown);
				lineContour3.push_back(endDown);
				lineContour4.push_back(endDown);
				lineContour5.push_back(endDown);
				lineContour6.push_back(endDown);
				lineContour7.push_back(endDown);
				lineContour8.push_back(endDown);
				for (int j = 0; j < Lanes.at(i).size(); j++)
				{
					lineContour1.push_back(Lanes.at(i).at(j));
					lineContour2.push_back(Point2i(Lanes.at(i).at(j).x + itvLane, Lanes.at(i).at(j).y));
					lineContour3.push_back(Point2i(Lanes.at(i).at(j).x + itvLane * 2, Lanes.at(i).at(j).y));
					lineContour4.push_back(Point2i(Lanes.at(i).at(j).x + itvLane * 3, Lanes.at(i).at(j).y));
					lineContour5.push_back(Point2i(Lanes.at(i).at(j).x + itvLane * 4, Lanes.at(i).at(j).y));
					lineContour6.push_back(Point2i(Lanes.at(i).at(j).x + itvLane * 5, Lanes.at(i).at(j).y));
					lineContour7.push_back(Point2i(Lanes.at(i).at(j).x + itvLane * 6, Lanes.at(i).at(j).y));
					lineContour8.push_back(Point2i(Lanes.at(i).at(j).x + itvLane * 7, Lanes.at(i).at(j).y));
				}
				lineContour1.push_back(endUp);
				lineContour2.push_back(endUp);
				lineContour3.push_back(endUp);
				lineContour4.push_back(endUp);
				lineContour5.push_back(endUp);
				lineContour6.push_back(endUp);
				lineContour7.push_back(endUp);
				lineContour8.push_back(endUp);

				const Point2i *pts1 = (const Point2i*)Mat(lineContour1).data;
				const Point2i *pts2 = (const Point2i*)Mat(lineContour2).data;
				const Point2i *pts3 = (const Point2i*)Mat(lineContour3).data;
				const Point2i *pts4 = (const Point2i*)Mat(lineContour4).data;
				const Point2i *pts5 = (const Point2i*)Mat(lineContour5).data;
				const Point2i *pts6 = (const Point2i*)Mat(lineContour6).data;
				const Point2i *pts7 = (const Point2i*)Mat(lineContour7).data;
				const Point2i *pts8 = (const Point2i*)Mat(lineContour8).data;

				int ptNum = Mat(lineContour1).rows;

				fillPoly(bufferImgL, &pts8, &ptNum, 1, Scalar(scorestep * 2, scorestep * 2, scorestep * 2));
				fillPoly(bufferImgL, &pts7, &ptNum, 1, Scalar(scorestep * 1, scorestep * 1, scorestep * 1));
				fillPoly(bufferImgL, &pts6, &ptNum, 1, Scalar(scorestep * 0, scorestep * 0, scorestep * 0));
				fillPoly(bufferImgL, &pts5, &ptNum, 1, Scalar(scorestep * 1, scorestep * 1, scorestep * 1));
				fillPoly(bufferImgL, &pts4, &ptNum, 1, Scalar(scorestep * 2, scorestep * 2, scorestep * 2));
				fillPoly(bufferImgL, &pts3, &ptNum, 1, Scalar(scorestep * 3, scorestep * 3, scorestep * 3));
				fillPoly(bufferImgL, &pts2, &ptNum, 1, Scalar(scorestep * 4, scorestep * 4, scorestep * 4));
				fillPoly(bufferImgL, &pts1, &ptNum, 1, Scalar(scorestep*5, scorestep*5, scorestep*5));
				break;
			}
		}
		//check the r, rr lines
		for (int i = 2; i < idLane.size(); i++) {
			if (idLane[i] == 0) {}
			else {
				endUp = Point(bufferImg.cols, 0);
				endDown = Point(bufferImg.cols, bufferImg.rows);
				vector<Point2i> lineContour1, lineContour2, lineContour3, lineContour4, lineContour5, lineContour6, lineContour7, lineContour8;
				lineContour1.push_back(endDown);
				lineContour2.push_back(endDown);
				lineContour3.push_back(endDown);
				lineContour4.push_back(endDown);
				lineContour5.push_back(endDown);
				lineContour6.push_back(endDown);
				lineContour7.push_back(endDown);
				lineContour8.push_back(endDown);
				for (int j = 0; j < Lanes.at(i).size(); j++)
				{

					lineContour1.push_back(Lanes.at(i).at(j));
					lineContour2.push_back(Point2i(Lanes.at(i).at(j).x - itvLane, Lanes.at(i).at(j).y));
					lineContour3.push_back(Point2i(Lanes.at(i).at(j).x - itvLane * 2, Lanes.at(i).at(j).y));
					lineContour4.push_back(Point2i(Lanes.at(i).at(j).x - itvLane * 3, Lanes.at(i).at(j).y));
					lineContour5.push_back(Point2i(Lanes.at(i).at(j).x - itvLane * 4, Lanes.at(i).at(j).y));
					lineContour6.push_back(Point2i(Lanes.at(i).at(j).x - itvLane * 5, Lanes.at(i).at(j).y));
					lineContour7.push_back(Point2i(Lanes.at(i).at(j).x - itvLane * 6, Lanes.at(i).at(j).y));
					lineContour8.push_back(Point2i(Lanes.at(i).at(j).x - itvLane * 7, Lanes.at(i).at(j).y));
				}
				lineContour1.push_back(endUp);
				lineContour2.push_back(endUp);
				lineContour3.push_back(endUp);
				lineContour4.push_back(endUp);
				lineContour5.push_back(endUp);
				lineContour6.push_back(endUp);
				lineContour7.push_back(endUp);
				lineContour8.push_back(endUp);

				const Point2i *pts1 = (const Point2i*)Mat(lineContour1).data;
				const Point2i *pts2 = (const Point2i*)Mat(lineContour2).data;
				const Point2i *pts3 = (const Point2i*)Mat(lineContour3).data;
				const Point2i *pts4 = (const Point2i*)Mat(lineContour4).data;
				const Point2i *pts5 = (const Point2i*)Mat(lineContour5).data;
				const Point2i *pts6 = (const Point2i*)Mat(lineContour6).data;
				const Point2i *pts7 = (const Point2i*)Mat(lineContour7).data;
				const Point2i *pts8 = (const Point2i*)Mat(lineContour8).data;

				int ptNum = Mat(lineContour1).rows;

				fillPoly(bufferImgR, &pts8, &ptNum, 1, Scalar(scorestep * 2, scorestep * 2, scorestep * 2));
				fillPoly(bufferImgR, &pts7, &ptNum, 1, Scalar(scorestep * 1, scorestep * 1, scorestep * 1));
				fillPoly(bufferImgR, &pts6, &ptNum, 1, Scalar(scorestep * 0, scorestep * 0, scorestep * 0));
				fillPoly(bufferImgR, &pts5, &ptNum, 1, Scalar(scorestep * 1, scorestep * 1, scorestep * 1));
				fillPoly(bufferImgR, &pts4, &ptNum, 1, Scalar(scorestep * 2, scorestep * 2, scorestep * 2));
				fillPoly(bufferImgR, &pts3, &ptNum, 1, Scalar(scorestep * 3, scorestep * 3, scorestep * 3));
				fillPoly(bufferImgR, &pts2, &ptNum, 1, Scalar(scorestep * 4, scorestep * 4, scorestep * 4));
				fillPoly(bufferImgR, &pts1, &ptNum, 1, Scalar(scorestep*5, scorestep*5, scorestep*5));
				break;
			}
		}
		//crop the image for sync with the lidar data.
		//imshow("R", bufferImgR);
		//imshow("L", bufferImgL);


	}
	bufferImg = bufferImgR + bufferImgL;
	//imshow("total lane image", bufferImg);
	bufferImg = bufferImg(Range(800 - 400, 800), Range(400 - 200, 400 + 200));
	//threshold(bufferImg, bufferImg, 2, 10, THRESH_BINARY);
	//imshow("CROP", bufferImg);
	resize(bufferImg, bufferImg, Size(600, 600), 0, 0, CV_INTER_NN);	//resize the image for be same the size of lidar data
	//imshow("buffer", bufferImg);
	return bufferImg;

}

//VOSS algorithm (VOronoi Score System)
void Driving::Basic() {
	//cout << "FUCK COME IN BASIC" << endl;
	dataContainer->setValue_UtoP_AorM(1);
	clock_t start, end;
	while (1)
	{
		//cout << "FUCK COME IN while" << endl;
		start = clock();
		imgPath = cv::Mat::zeros(600, 600, CV_8UC3); 
		vector<Point2d> vecXY = dataContainer->getValue_lidar_VecXY();
		vector<Point2d> vecXYDraw;
		double cenX = imgPath.cols * 0.5, cenY = imgPath.rows *0.99; //the location of LiDAR in the map.
		double scale = cenY / (SICK_SCAN_ROI_Y+50);				  //obj_Data => imgPath
		//Car size in map
		double carW = CAR_WEITH * scale;
		double carH = CAR_HEIGH * scale;
		//ROI AREA
		double leftEndX = cenX - SICK_SCAN_ROI_X * scale;
		double rightEndX = cenX + SICK_SCAN_ROI_X * scale;
		double topEndY = cenY - SICK_SCAN_ROI_Y * scale;
		double bottomEndY = cenY + SICK_SCAN_ROI_Y * scale;
		//Localization of LiDAR in the ROI
		Point2d center(cenX, cenY);

			////////////////////////////////////////////////////////////////////////////////////
			////Fill the Regions where cannot go in, because of max value of steering angle.////
			////////////////////////////////////////////////////////////////////////////////////
		//Left area where platform can not go (left 60degrees)
		Point points[1][4];
		points[0][0] = Point(cenX - carW, center.y);
		points[0][1] = Point(leftEndX, center.y);
		points[0][2] = Point(leftEndX, center.y - (SICK_SCAN_ROI_X*scale - carW)*sqrt(3)/2);
		points[0][3] = Point(cenX - (SICK_SCAN_ROI_X*scale + carW)/2, center.y - (SICK_SCAN_ROI_X*scale - carW)*sqrt(3)/2);
		const Point* pnts[1] = { points[0] };
		int npt2[] = { 4 };
		//Right area where platform can not go (right 60degrees)
		Point points2[1][4];
		points2[0][0] = Point(center.x + carW, center.y);
		points2[0][1] = Point(rightEndX, center.y);
		points2[0][2] = Point(rightEndX, center.y - (SICK_SCAN_ROI_X*scale - carW) * sqrt(3) / 2);
		points2[0][3] = Point(cenX + (SICK_SCAN_ROI_X*scale + carW) / 2, center.y - (SICK_SCAN_ROI_X*scale - carW)*sqrt(3) / 2);
		const Point* pnts2[1] = { points2[0] };
		//Drawing
		fillPoly(imgPath, pnts, npt2, 1, CV_RGB(150, 0, 0));
		fillPoly(imgPath, pnts2, npt2, 1, CV_RGB(150, 0, 0));
		cv::cvtColor(imgPath, scoreMap, CV_BGR2GRAY);

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
					line(imgPath, vecXYDraw[i], vecXYDraw[i + 1], CV_RGB(0, 255, 0), 2);
				}
			}
		}

		vector<vector<double> > objDataSet = dataContainer->getValue_lidar_Data().back();
		Point polypts[1][4];
		double cirCenX, cirCenY, cirCenR;
		double cenDist, touchDist, theta_s, theta_l;
		vector<double>objdist;
		int polyX1, polyY1, polyX2, polyY2, polyX3, polyY3, polyX4, polyY4;
		for (int i = 0; i < objDataSet.size(); i++){
			cirCenX = center.x + objDataSet[i][0] * scale;
			cirCenY = center.y - objDataSet[i][1] * scale;
			cirCenR = objDataSet[i][2] * scale;
			Point2d cirCen(cirCenX, cirCenY); //center of objs.
			cenDist = sqrt(pow((cirCen.x - cenX), 2) + pow((cirCen.y - cenY), 2));
			objdist.push_back(cenDist-cirCenR);
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
			////Make the image to Score map.////
			////////////////////////////////////

		cv::cvtColor(imgPath, imgPath, CV_BGR2GRAY);
		threshold(imgPath, imgPath, 1, 10, THRESH_BINARY_INV);
		threshold(scoreMap, scoreMap, 1, 10, THRESH_BINARY_INV);

		//MAKING VORNOI FIELD
		int kerSize;
		Mat kernel; 
		Mat stepVot = Mat::zeros(imgPath.cols, imgPath.rows, CV_8UC1);
		Mat stepVot2 = Mat::zeros(imgPath.cols, imgPath.rows, CV_8UC1);
		for (int i = 1; i < 4; i++)
		{
			kerSize = 15 * i;
			kernel = Mat::ones(kerSize, kerSize, CV_8UC1);
			morphologyEx(scoreMap, stepVot2, MORPH_ERODE, kernel);
			morphologyEx(imgPath, stepVot, MORPH_ERODE, kernel);
			imgPath += stepVot;
			scoreMap += stepVot2;
		}

		//Apply the lane data to the lidar data
		Mat buf = Mat::zeros(600, 600, CV_8UC1);
		Mat laneImg = getLaneData(10);
		imshow("LaneMap", laneImg);
		scoreMap -= laneImg;
		imgPath -= laneImg;

			//////////////////////////////////////////////////////////////////////////////
			////Determine the desired Steering Angle in Score System with Vornoi Field////
			//////////////////////////////////////////////////////////////////////////////

		//REGION OF WORKABLE ANGLE: 60 ~ 120, with interval=5 degrees
		vector<uint> score[169]; //include the scores at [90,85, 95, 80, 100, 75, 105, 70, 110, 65, 115, 60, 120]degrees
		vector<int> theta = { 0, -5, 5, -10, 10, -15, 15, -20, 20, -25, 25, -30, 30 }; //The steering angle candidates
		uchar onestep = (CAR_HEIGH) * scale; 
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
			circle(cirGray, step1, (onestep/ 2 )- 5, CV_RGB(10, 10, 10), -1, CV_AA, 0);
			for (int j = 0; j < theta.size(); j++)
			{
				cirGray2 = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1);
				scoreTheta2 = 90 + theta.at(j);
				Point2d step2(step1.x + onestep * cos(CV_PI*scoreTheta2 / 180), step1.y - onestep * sin(CV_PI*scoreTheta2 / 180));
				circle(cirGray2, step2, (onestep / 2) -5, CV_RGB(10,10,10), -1, CV_AA, 0);
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
		
		int desired_steering = goTheta1 * steerRatio + goTheta2 *(1-steerRatio);
		Point2d pntF(cenX + onestep*1.5 * cos(CV_PI*(90 + desired_steering) / 180), cenY - (onestep*1.5*sin(CV_PI*(90 + desired_steering) / 180)));

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
		cv::arrowedLine(imgPath, center, stepFirst, CV_RGB(50, 50, 50), 5);
		cv::arrowedLine(imgPath, stepFirst, stepSecond, CV_RGB(50, 50, 50), 5);

			/////////////////////////////////////////////////////////////////////
			////Determine the desired Speed in Score System with Vornoi Field////
			/////////////////////////////////////////////////////////////////////
		//Add the line data in the scoreMap and img Path.
		uint scoreofMap=0; //total sum of scoreMap
		uint scoreofPath=0;//total sum of imgPath
		uint desired_speed;

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
		//compare the scoreofMap and scoreofPath
		desired_speed = (uint)((double)speedHigh * scoreofPath / scoreofMap);
		//cout << "ratio: " << ((double)scoreofPath / scoreofMap) << endl;

			/////////////////////////////
			////!!!!!!EMERGENCY!!!!!!////
			/////////////////////////////
		if (objdist.size() == 0) {
			cout << "do nothing" << endl;
			//cv::arrowedLine(imgPath, center, pntF, CV_RGB(255, 255, 25), 5);
		}
		else {
			//stop condition
			//if there is any object within 600mm, STOP
			//if the score is lower then threshold
			double objClose = objdist.at(distance(objdist.begin(), min_element(objdist.begin(), objdist.end())));
			if (objClose < 600*scale) { 
				desired_speed = 0; 
				dataContainer->setValue_UtoP_BRAKE(100);
				cout << "STOP!!!!!!!TOO CLOSE!!!!" << endl;
			}
			else if (desired_speed < speedLow) {
				desired_speed = speedLow; cout << "Fucking LOW SCORE!!" << endl;
				cv::arrowedLine(imgPath, center, pntF, CV_RGB(200, 200, 200), 2);
			}
			else {
				desired_speed = desired_speed;
				cv::arrowedLine(imgPath, center, pntF, CV_RGB(200, 200, 200), 2);
			}
		}

		/*cout << "desired_speed = " << desired_speed << endl;
		cout << "desired_steer = " << desired_steering << endl;*/
		imshow("Map", scoreMap);
		imshow("Path", imgPath);

			//////////////////////////////////////////////////
			////Final Control the steering angle and speed////
			//////////////////////////////////////////////////
		//dataContainer->setValue_UtoP_STEER(desired_steering);
		//dataContainer->setValue_UtoP_SPEED(desired_speed);
		
		end = clock();
		cout << "lidar time: " << (double)(end - start) / 1000 << "sec" << endl ;
		int key = cv::waitKey(1);

		if (key == 27) {
			break;
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
	Point2d center(cenX, cenY), platEnd(cenX, platEndY);
	Point2d leftTopEnd(leftEndX, topEndY), leftBottomEnd(leftEndX, cenY);
	Point2d rightTopEnd(rightEndX, topEndY), rightBottomEnd(rightEndX, cenY);
	//center = platEnd;
	//////////////////////////////////////////////////////////////////////////////
	//fill the Regions where cannot go in, because of obstacles.
	for (int i = 0; i < vecXY.size(); ++i) { //스케일 조정
		double xyDrawX = center.x + vecXY[i].x * scale;
		double xyDrawY = center.y - vecXY[i].y * scale;

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

		double cirCenX = center.x + objDataSet[i][0] * scale;
		double cirCenY = center.y - objDataSet[i][1] * scale;
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

	//arrowedLine(img, center, platEnd, CV_RGB(0, 150, 150), 2); //라이다 본체 벡터
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

	imshow("lidar", img);

	dataContainer->setValue_lidar_ImgData(img);
	*/
}

void Driving::LOS() {
	aster = new Planner;
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

void Driving::Mission1() {
	//
	// To do : Implement Basic Driving Algorithm
	//

}

void Driving::Mission2() {

}

void Driving::Mission3() {


}

void Driving::Mission4() {

}

void Driving::Mission5() {}

/// functions which switch radian and degree
double Driving::rad2deg(double radian) { return radian * 180 / PI; }
double Driving::deg2rad(double degree) { return degree * PI / 180; }
