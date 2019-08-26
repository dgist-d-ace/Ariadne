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

	Mat cirGray, cirGray2, temp, buffer;
	uint Theta, Theta2;

	for (int i = 0; i < checkTheta.size(); i++) {
		cirGray = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1);
		Theta = 90 + checkTheta.at(i);
		Point2d center(cenX, cenY);
		Point2d step1(cenX + onestep * cos(CV_PI*Theta / 180), cenY - onestep * sin(CV_PI*Theta / 180));
		//Point2d step15(cenX + onestep*1.5 * cos(CV_PI*Theta / 180), cenY - onestep*1.5 * sin(CV_PI*Theta / 180));
		Point2d step05(cenX + onestep * cos(CV_PI*Theta / 180) / 2, cenY - onestep * sin(CV_PI*Theta / 180) / 2);
		circle(cirGray, step1, (onestep*0.8), Scalar::all(40), -1, CV_AA, 0);
		for (int j = 0; j < checkTheta2.size(); j++)
		{
			temp = cirGray.clone();
			cirGray2 = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1);
			Theta2 = 90 + checkTheta2.at(j);
			Point2d step2(step1.x + onestep * cos(CV_PI*Theta2 / 180), step1.y - onestep * sin(CV_PI*Theta2 / 180));
			circle(cirGray2, step1, (onestep*0.4), Scalar::all(40), -1, CV_AA, 0);
			circle(temp, step2, (onestep*0.6), Scalar::all(20), -1, CV_AA, 0);
			circle(cirGray2, center, (onestep*0.8), Scalar::all(40), -1, CV_AA, 0);

			//circle(cirGray2, Point2d(cenX, cenY), (onestep*0.8), Scalar::all(15), -1, CV_AA, 0);
			cirGray2 += temp;
			buffer = cirGray2.clone();
			checkImgs.push_back(buffer);
			//imshow("CHECK", buffer);
			//waitKey(10);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////
	////Fill the Regions where cannot go in, because of max value of steering angle.////
	////////////////////////////////////////////////////////////////////////////////////
	double leftEndX = cenX - SICK_SCAN_ROI_X * scale;
	double rightEndX = cenX + SICK_SCAN_ROI_X * scale;
	double topEndY = cenY - SICK_SCAN_ROI_Y * scale;
	double bottomEndY = cenY + SICK_SCAN_ROI_Y * scale;
	int npt2[] = { 4 };
	//Left area where platform can not go (left 60degrees)
	Point points[1][4];
	points[0][0] = Point(cenX - carW, cenY);
	points[0][1] = Point(leftEndX, cenY);
	points[0][2] = Point(leftEndX, cenY - (SICK_SCAN_ROI_X*scale - carW) * sqrt(3) / 2);
	points[0][3] = Point(cenX - (SICK_SCAN_ROI_X*scale + carW) / 2, cenY - (SICK_SCAN_ROI_X*scale - carW) * sqrt(3) / 2);
	const Point* pnts[1] = { points[0] };

	//Right area where platform can not go (right 60degrees)
	Point points2[1][4];
	points2[0][0] = Point(cenX + carW, cenY);
	points2[0][1] = Point(rightEndX, cenY);
	points2[0][2] = Point(rightEndX, cenY - (SICK_SCAN_ROI_X*scale - carW) * sqrt(3) / 2);
	points2[0][3] = Point(cenX + (SICK_SCAN_ROI_X*scale + carW) / 2, cenY - (SICK_SCAN_ROI_X*scale - carW) * sqrt(3) / 2);
	const Point* pnts2[1] = { points2[0] };

	//Drawing
	fillPoly(outRange, pnts, npt2, 1, Scalar::all(255));
	fillPoly(outRange, pnts2, npt2, 1, Scalar::all(255));

}

#define scoreStep 5
#define itvLane	20 //tuning with real scale ratio with Bae
//Input: lane data from scnn
//output: 600x600 Mat image
//Make the scored lane map 
//Tuning Point:
/*
**vector about steering angle(or checking angle)
**onestep
**do or dont fill the area meaning out of steering angle range
**kernal size for obstacle, inter size of lane.
**value of lane, obstacle
**speed decision.(ratio)
*/
//Manual tuning: scoreStep & itvLane
Mat Driving::getLaneData(int scorestep)
{
	//the form of existLanes: vector<int> like [ll, l, r, rr], (0: None, 1: White, 2: Blue, 3: Yellow)
	//the form of lanes: vector<vector<Point2i>> [[the points of ll],[the points of l],[the points of r],[the points of rr]]
	Mat bufferImg = Mat::zeros(800, 800, CV_8UC1);
	bufferImg = Scalar::all(scorestep*(10+3));//130
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
				lineContour2.push_back(endDown);
				lineContour3.push_back(endDown);
				lineContour4.push_back(endDown);
				lineContour5.push_back(endDown);
				lineContour6.push_back(endDown);
				lineContour7.push_back(endDown);
				lineContour8.push_back(endDown);
				lineContour9.push_back(endDown);
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
					lineContour9.push_back(Point2i(Lanes.at(i).at(j).x + itvLane * 7, Lanes.at(i).at(j).y));

				}
				lineContour1.push_back(endUp);
				lineContour2.push_back(endUp);
				lineContour3.push_back(endUp);
				lineContour4.push_back(endUp);
				lineContour5.push_back(endUp);
				lineContour6.push_back(endUp);
				lineContour7.push_back(endUp);
				lineContour8.push_back(endUp);
				lineContour9.push_back(endUp);

				const Point2i *pts1 = (const Point2i*)Mat(lineContour1).data;
				const Point2i *pts2 = (const Point2i*)Mat(lineContour2).data;
				const Point2i *pts3 = (const Point2i*)Mat(lineContour3).data;
				const Point2i *pts4 = (const Point2i*)Mat(lineContour4).data;
				const Point2i *pts5 = (const Point2i*)Mat(lineContour5).data;
				const Point2i *pts6 = (const Point2i*)Mat(lineContour6).data;
				const Point2i *pts7 = (const Point2i*)Mat(lineContour7).data;
				const Point2i *pts8 = (const Point2i*)Mat(lineContour8).data;
				const Point2i *pts9 = (const Point2i*)Mat(lineContour9).data;

				int ptNum = Mat(lineContour1).rows;
				fillPoly(bufferImgL, &pts9, &ptNum, 1, Scalar(scorestep * 3, scorestep * 3, scorestep * 3));
				fillPoly(bufferImgL, &pts8, &ptNum, 1, Scalar(scorestep *2, scorestep *2, scorestep *2));
				fillPoly(bufferImgL, &pts7, &ptNum, 1, Scalar(scorestep*1, scorestep *1, scorestep *1));
				fillPoly(bufferImgL, &pts6, &ptNum, 1, Scalar(scorestep * 0, scorestep * 0, scorestep * 0));
				fillPoly(bufferImgL, &pts5, &ptNum, 1, Scalar(scorestep *2, scorestep *2, scorestep *2));
				fillPoly(bufferImgL, &pts4, &ptNum, 1, Scalar(scorestep * 4, scorestep * 4, scorestep * 4));
				fillPoly(bufferImgL, &pts3, &ptNum, 1, Scalar(scorestep * 6, scorestep * 6, scorestep * 6));
				fillPoly(bufferImgL, &pts2, &ptNum, 1, Scalar(scorestep * 8, scorestep * 8, scorestep * 8));
				fillPoly(bufferImgL, &pts1, &ptNum, 1, Scalar(scorestep * (10 + idLane[i]), scorestep *( 10 + idLane[i]), scorestep *( 10 + idLane[i])));
				
				Mat bufferIv = Mat::zeros(800, 800, CV_8UC1);
				fillPoly(bufferIv, &pts9, &ptNum, 1, Scalar(scorestep, scorestep, scorestep));
				threshold(bufferIv, bufferIv, 1, scorestep*4, THRESH_BINARY_INV);
				bufferImgL += bufferIv;
				break;
			}
		}
		//check the r, rr lines
		for (int i = 2; i < idLane.size(); i++) {
			if (idLane[i] == 0) {}
			else {
				int id = idLane[i];
				if (idLane[3] == 0){id = 3;}
				else { id = idLane[i]; }

				endUp = Point(bufferImg.cols, 0);
				endDown = Point(bufferImg.cols, bufferImg.rows);
				vector<Point2i> lineContour1, lineContour2, lineContour3, lineContour4, lineContour5, lineContour6, lineContour7, lineContour8, lineContour9;
				lineContour1.push_back(endDown);
				lineContour2.push_back(endDown);
				lineContour3.push_back(endDown);
				lineContour4.push_back(endDown);
				lineContour5.push_back(endDown);
				lineContour6.push_back(endDown);
				lineContour7.push_back(endDown);
				lineContour8.push_back(endDown);
				lineContour9.push_back(endDown);

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
					lineContour9.push_back(Point2i(Lanes.at(i).at(j).x - itvLane * 7, Lanes.at(i).at(j).y));
				}
				lineContour1.push_back(endUp);
				lineContour2.push_back(endUp);
				lineContour3.push_back(endUp);
				lineContour4.push_back(endUp);
				lineContour5.push_back(endUp);
				lineContour6.push_back(endUp);
				lineContour7.push_back(endUp);
				lineContour8.push_back(endUp);
				lineContour9.push_back(endUp);

				const Point2i *pts1 = (const Point2i*)Mat(lineContour1).data;
				const Point2i *pts2 = (const Point2i*)Mat(lineContour2).data;
				const Point2i *pts3 = (const Point2i*)Mat(lineContour3).data;
				const Point2i *pts4 = (const Point2i*)Mat(lineContour4).data;
				const Point2i *pts5 = (const Point2i*)Mat(lineContour5).data;
				const Point2i *pts6 = (const Point2i*)Mat(lineContour6).data;
				const Point2i *pts7 = (const Point2i*)Mat(lineContour7).data;
				const Point2i *pts8 = (const Point2i*)Mat(lineContour8).data;
				const Point2i *pts9 = (const Point2i*)Mat(lineContour9).data;

				int ptNum = Mat(lineContour1).rows;

				fillPoly(bufferImgR, &pts9, &ptNum, 1, Scalar(scorestep * 3, scorestep * 3, scorestep * 3));
				fillPoly(bufferImgR, &pts8, &ptNum, 1, Scalar(scorestep *2, scorestep *2, scorestep *2));
				fillPoly(bufferImgR, &pts7, &ptNum, 1, Scalar(scorestep * 1, scorestep * 1, scorestep * 1));
				fillPoly(bufferImgR, &pts6, &ptNum, 1, Scalar(scorestep * 0, scorestep * 0, scorestep * 0));
				fillPoly(bufferImgR, &pts5, &ptNum, 1, Scalar(scorestep *2, scorestep *2, scorestep *2));
				fillPoly(bufferImgR, &pts4, &ptNum, 1, Scalar(scorestep * 4, scorestep * 4, scorestep * 4));
				fillPoly(bufferImgR, &pts3, &ptNum, 1, Scalar(scorestep * 6, scorestep * 6, scorestep * 6));
				fillPoly(bufferImgR, &pts2, &ptNum, 1, Scalar(scorestep * 8, scorestep * 8, scorestep * 8));
				fillPoly(bufferImgR, &pts1, &ptNum, 1, Scalar(scorestep * (10 + id), scorestep *(10 + id), scorestep *(10 + id)));
				
				Mat bufferIv = Mat::zeros(800, 800, CV_8UC1);
				fillPoly(bufferIv, &pts9, &ptNum, 1, Scalar(scorestep, scorestep, scorestep));
				threshold(bufferIv, bufferIv, 1, scorestep*4, THRESH_BINARY_INV);

				bufferImgR += bufferIv;

				break;
			}
		}
	}
	//imshow("R", bufferImgR);
	//imshow("L", bufferImgL);

	bufferImg = bufferImgR + bufferImgL;
	//imshow("M", bufferImg);
	bufferImg = bufferImg(Range(800 - 400, 800), Range(400 - 200, 400 + 200));
	resize(bufferImg, bufferImg, Size(600, 600), 0, 0, CV_INTER_NN);	//resize the image for be same the size of locLidar data
	//imshow("buffer", bufferImg);
	return bufferImg;
}

void Driving::getGpsData(int scorestep)
{

}

void Driving::PASIVcontrol(double desired_speed, double desired_steering, double desired_brake)
{
	//get the values from platform
	/*double pre_speed, pre_steer, pre_brake;
	pre_speed = dataContainer->getValue_PtoU_SPEED()/10;
	pre_steer = dataContainer->getValue_PtoU_STEER()/10;
	pre_brake = dataContainer->getValue_PtoU_BRAKE()/10;*/
	
	if (desired_speed > 0) { desired_brake = 0; }
	if (desired_brake > 0) { desired_speed = 0; }
	
	//function
	cout << "desired_speed = " << desired_speed << endl;
	cout << "desired_steer = " << desired_steering << endl;
	cout << "desired_brake = " << desired_brake << endl;


	//Return
	dataContainer->setValue_UtoP_SPEED(desired_speed);
	dataContainer->setValue_UtoP_STEER(desired_steering);
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
		imgPath = cv::Mat::zeros(600, 600, CV_8UC1);				//path made with lanes and objs
		scoreMap = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1); //lane map without objs

		vector<Point2d> vecXY = dataContainer->getValue_lidar_VecXY();
		vector<Point2d> vecXYDraw;

		//ROI AREA in the maps
		double leftEndX = cenX - SICK_SCAN_ROI_X * scale;
		double rightEndX = cenX + SICK_SCAN_ROI_X * scale;
		double topEndY = cenY - SICK_SCAN_ROI_Y * scale;
		double bottomEndY = cenY + SICK_SCAN_ROI_Y * scale;

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
		for (int i = 0; i < objDataSet.size(); i++) {
			cirCenX = locLidar.x + objDataSet[i][0] * scale;
			cirCenY = locLidar.y - objDataSet[i][1] * scale;
			cirCenR = objDataSet[i][2] * scale;
			Point2d cirCen(cirCenX, cirCenY); //locLidar of objs.
			cenDist = sqrt(pow((cirCen.x - cenX), 2) + pow((cirCen.y - cenY), 2));
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
			fillPoly(imgPath, ppt, npt, 1, CV_RGB(200, 200, 200));
			circle(imgPath, cirCen, cirCenR, CV_RGB(255, 255, 255), -1, CV_AA);
		}

		///////////////////////////////////////			
		////Make the Score Implemented map.////
		///////////////////////////////////////		//BIG change 0823
		//cv::cvtColor(imgPath, imgPath, CV_BGR2GRAY);
		threshold(imgPath, imgPath, 1, 20, THRESH_BINARY_INV);
		threshold(scoreMap, scoreMap, 1, 20, THRESH_BINARY_INV);

		Mat _window = Mat::ones(15, 15, CV_8UC1);
		morphologyEx(imgPath, imgPath, MORPH_ERODE, _window);
		//morphologyEx(scoreMap, scoreMap, MORPH_ERODE, _window);

		//SCORE IMPLEMENTED VORONOI FIELD
		int kerSize;
		Mat kernel;
		Mat stepVot = Mat::zeros(imgPath.cols, imgPath.rows, CV_8UC1);
		Mat stepVot2 = Mat::zeros(imgPath.cols, imgPath.rows, CV_8UC1);
		for (int i = 1; i < 4; i++)
		{
			kerSize = 25 * i;
			kernel = Mat::ones(kerSize, kerSize, CV_8UC1);
			morphologyEx(scoreMap, stepVot2, MORPH_ERODE, kernel);
			morphologyEx(imgPath, stepVot, MORPH_ERODE, kernel);
			imgPath += stepVot;
			scoreMap += stepVot2;
		}
		//cout << imgPath.col(300) << endl;
		//scoreMap = Scalar::all(160);

		////Apply the lane data to the locLidar data
		Mat laneImg = getLaneData(scoreStep);
		scoreMap -= laneImg;
		imgPath -= laneImg;

		////Apply the out of range of steering angle
		scoreMap -= outRange;
		imgPath -= outRange;

		//////////////////////////////////////////////////////////////////////////////
		////Determine the desired Steering Angle in Score System with Vornoi Field////
		//////////////////////////////////////////////////////////////////////////////
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
		uint scoreMin = distance(score.begin(), min_element(score.begin(), score.end()));
		int goTheta1 = checkTheta.at(scoreMax / checkTheta2.size());
		int goTheta2 = checkTheta2.at(scoreMax % checkTheta2.size());
		int dangerTheta = checkTheta.at(scoreMin / checkTheta2.size());

		Point2d stepFirst(cenX + onestep * cos(CV_PI*(90 + goTheta1) / 180), cenY - (onestep*sin(CV_PI*(90 + goTheta1) / 180)));
		Point2d stepSecond(stepFirst.x + onestep * cos(CV_PI*(90 + goTheta2) / 180), stepFirst.y - (onestep*sin(CV_PI*(90 + goTheta2) / 180)));
		Point2d dangerPoint(cenX + onestep * cos(CV_PI*(90 + dangerTheta) / 180), cenY - (onestep*sin(CV_PI*(90 + dangerTheta) / 180)));

		double desired_steering = goTheta1 * steerRatio + goTheta2 * (1 - steerRatio);
		Point2d pntF(cenX + onestep * 1.5 * cos(CV_PI*(90 + desired_steering) / 180), cenY - onestep * 1.5*sin(CV_PI*(90 + desired_steering) / 180));

		arrowedLine(imgPath, locLidar, stepFirst,Scalar::all(50), 5);
		arrowedLine(imgPath, stepFirst, stepSecond, Scalar::all(50), 5);

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

		double desired_speed = speedHigh * scoreofPath / scoreofMap;
		//circle(imgPath, dangerPoint,20,Scalar::all(255),  -1, CV_AA, 0 );
			///////////////////////
			////Extra Condition////
			///////////////////////
		double desired_brake;
		if (objdist.size() == 0) {
			//There is no obstacles.
			desired_brake=0;
			cv::arrowedLine(imgPath, locLidar, pntF, Scalar::all(200), 2);
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
				cout << "STOP!!!!!!!TOO CLOSE!!!!" << endl;
			}
			else if (desired_speed < speedLow) {
				//limit the speed
				desired_brake = 0;
				desired_speed = speedLow; cout << "LOW SCORE!!" << endl;
				cv::arrowedLine(imgPath, locLidar, pntF, Scalar::all(200), 2);
			}
			else {
				desired_brake = 0;
				desired_speed = desired_speed;
				cv::arrowedLine(imgPath, locLidar, pntF, Scalar::all(200), 2);
			}
		}
		//cout << "ordered speed: " << desired_speed << endl;
		//imshow("Map", scoreMap);
		imshow("Path", imgPath);
		QImage image1 = QImage((uchar*)imgPath.data, imgPath.cols, imgPath.rows, imgPath.step, QImage::Format_RGB888);
		dataContainer->setValue_ui_pathmap(image1);
		//////////////////////////////////////////////////
		////Final Control the steering angle and speed////
		//////////////////////////////////////////////////
		PASIVcontrol(desired_speed, desired_steering, desired_brake);

		end = clock();
		cout << "lidar time: " << (double)(end - start) / 1000 << "sec" << endl ;
		int key = cv::waitKey(1);
		if (key == 27) {
			break;
		}
	}
}

//PASIV with GPS
void Driving::BasicGPS(int missionId) {
	cout << "PASIV driving" << endl;
	dataContainer->setValue_UtoP_AorM(1);

	clock_t start, end;
	dataContainer->setValue_UtoP_BRAKE(0);

	while (1) {
		///////////////////////////////////////////
		////Break the PASIV for another mission////
		///////////////////////////////////////////
		int mission = dataContainer->getValue_yolo_missionID();
		if (mission != missionId)
		{
			cout << "called in PASIV but wrong mission ID : " << mission << endl;
			break;
		}
		//////////////////////////////////////////////////////////////////////////////////////
		start = clock();
		imgPath = cv::Mat::zeros(600, 600, CV_8UC3);
		vector<Point2d> vecXY = dataContainer->getValue_lidar_VecXY();
		vector<Point2d> vecXYDraw;

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
		points[0][2] = Point(leftEndX, center.y - (SICK_SCAN_ROI_X*scale - carW)/sqrt(3)/2);
		points[0][3] = Point(cenX - (SICK_SCAN_ROI_X*scale + carW) / 2, center.y - (SICK_SCAN_ROI_X*scale - carW)/sqrt(3)/2);
		const Point* pnts[1] = { points[0] };
		int npt2[] = { 4 };
		//Right area where platform can not go (right 60degrees)
		Point points2[1][4];
		points2[0][0] = Point(center.x + carW, center.y);
		points2[0][1] = Point(rightEndX, center.y);
		points2[0][2] = Point(rightEndX, center.y - (SICK_SCAN_ROI_X*scale - carW)/sqrt(3)/2);
		points2[0][3] = Point(cenX + (SICK_SCAN_ROI_X*scale + carW) / 2, center.y - (SICK_SCAN_ROI_X*scale - carW)/sqrt(3)/2);
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
		for (int i = 0; i < objDataSet.size(); i++) {
			cirCenX = center.x + objDataSet[i][0] * scale;
			cirCenY = center.y - objDataSet[i][1] * scale;
			cirCenR = objDataSet[i][2] * scale;
			Point2d cirCen(cirCenX, cirCenY); //locLidar of objs.
			cenDist = sqrt(pow((cirCen.x - cenX), 2) + pow((cirCen.y - cenY), 2));
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
		threshold(imgPath, imgPath, 1, 30, THRESH_BINARY_INV);
		threshold(scoreMap, scoreMap, 1, 30, THRESH_BINARY_INV);

		//SCORE IMPLEMENTED VORONOI FIELD
		int kerSize;
		Mat kernel;
		Mat stepVot = Mat::zeros(imgPath.cols, imgPath.rows, CV_8UC1);
		Mat stepVot2 = Mat::zeros(imgPath.cols, imgPath.rows, CV_8UC1);
		for (int i = 1; i < 4; i++)
		{
			kerSize = 20 * i;
			kernel = Mat::ones(kerSize, kerSize, CV_8UC1);
			morphologyEx(scoreMap, stepVot2, MORPH_ERODE, kernel);
			morphologyEx(imgPath, stepVot, MORPH_ERODE, kernel);
			imgPath += stepVot;
			scoreMap += stepVot2;
		}

		////Apply the lane data to the locLidar data
		Mat laneImg = getLaneData(scoreStep);
		scoreMap -= laneImg;
		imgPath -= laneImg;
		//apply gps

		if (mission == INTER_READY || mission == INTER_LEFT || mission == INTER_RIGHT || mission == INTER_STRAIGHT || mission == INTER_STOP)
		{
			//Intersection -> using GPS points
			//getGpsData();
			cout << "called in PASIV but wrong mission ID : " << mission << endl;
		}



		//////////////////////////////////////////////////////////////////////////////
		////Determine the desired Steering Angle in Score System with Vornoi Field////
		//////////////////////////////////////////////////////////////////////////////
		//REGION OF WORKABLE ANGLE: 60 ~ 120, with interval=5 degrees
		vector<uint> score[171];
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
			score->push_back(sum);
		}
		uint scoreMax = distance(score->begin(), max_element(score->begin(), score->end()));
		int goTheta1 = checkTheta.at(scoreMax / checkTheta2.size());
		int goTheta2 = checkTheta2.at(scoreMax % checkTheta2.size());

		Point2d stepFirst(cenX + onestep * cos(CV_PI*(90 + goTheta1) / 180), cenY - (onestep*sin(CV_PI*(90 + goTheta1) / 180)));
		Point2d stepSecond(stepFirst.x + onestep * cos(CV_PI*(90 + goTheta2) / 180), stepFirst.y - (onestep*sin(CV_PI*(90 + goTheta2) / 180)));

		double desired_steering = goTheta1 * steerRatio + goTheta2 * (1 - steerRatio);
		Point2d pntF(cenX + onestep * 1.5 * cos(CV_PI*(90 + desired_steering) / 180), cenY - onestep * 1.5*sin(CV_PI*(90 + desired_steering) / 180));

		arrowedLine(imgPath, center, stepFirst, CV_RGB(50, 50, 50), 5);
		arrowedLine(imgPath, stepFirst, stepSecond, CV_RGB(50, 50, 50), 5);

		/////////////////////////////////////////////////////////////////////
		////Determine the desired Speed in Score System with Vornoi Field////
		/////////////////////////////////////////////////////////////////////
		//Add the line data in the scoreMap and img Path.
		double scoreofMap = 0; //total sum of scoreMap
		double scoreofPath = 0;//total sum of imgPath
		double desired_speed;

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
		desired_speed = speedHigh * scoreofPath / scoreofMap;
		//cout << "ratio: " << ((double)scoreofPath / scoreofMap) << endl;

			///////////////////////
			////Extra Condition////
			///////////////////////
		if (objdist.size() == 0) {
			//There is no obstacles.
			dataContainer->setValue_UtoP_BRAKE(0);
			cv::arrowedLine(imgPath, center, pntF, CV_RGB(200, 200, 200), 2);
		}
		else {
			//stop condition
			//if there is any object within 600mm, STOP
			//if the score is lower then threshold
			double objClose = objdist.at(distance(objdist.begin(), min_element(objdist.begin(), objdist.end())));
			if (objClose < 600 * scale) {
				//similary to Emergency Stop
				desired_speed = 0;
				dataContainer->setValue_UtoP_BRAKE(100);
				cout << "STOP!!!!!!!TOO CLOSE!!!!" << endl;
			}
			else if (desired_speed < speedLow) {
				//limit the speed
				dataContainer->setValue_UtoP_BRAKE(0);
				desired_speed = speedLow; cout << "Fucking LOW SCORE!!" << endl;
				cv::arrowedLine(imgPath, center, pntF, CV_RGB(200, 200, 200), 2);
			}
			else {
				desired_speed = desired_speed;
				dataContainer->setValue_UtoP_BRAKE(0);
				cv::arrowedLine(imgPath, center, pntF, CV_RGB(200, 200, 200), 2);
			}
		}

		//imshow("Map", scoreMap);
		imshow("Path", imgPath);

		QImage image1 = QImage((uchar*)imgPath.data, imgPath.cols, imgPath.rows, imgPath.step, QImage::Format_RGB888);
		dataContainer->setValue_ui_pathmap(image1);

		//////////////////////////////////////////////////
		////Final Control the steering angle and speed////
		//////////////////////////////////////////////////
		dataContainer->setValue_UtoP_STEER(desired_steering);
		dataContainer->setValue_UtoP_SPEED(desired_speed);

		end = clock();
		/// cout << "locLidar time: " << (double)(end - start) / 1000 << "sec" << endl ;
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

	//미션이 끝났을 시, yolo에서 다른 mission trigger를 주지 않으면 basic으로 넘어감
	if(dataContainer->getValue_yolo_missionID() == PARKING )
		dataContainer->setValue_yolo_missionID(BASIC);
}

//Mission No.2: Intersection Ready
void Driving::MissionIntReady() {
	cout << "mission 2" << endl;

	//
	//mission code
	//
	BasicGPS(INTER_READY);

	//미션이 끝났을 시, yolo에서 다른 mission trigger를 주지 않으면 basic으로 넘어감
	if (dataContainer->getValue_yolo_missionID()== INTER_READY)
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
	if (dataContainer->getValue_yolo_missionID()== INTER_LEFT)
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
	if (dataContainer->getValue_yolo_missionID()== INTER_RIGHT)
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
	if (dataContainer->getValue_yolo_missionID()== INTER_STRAIGHT)
		dataContainer->setValue_yolo_missionID(BASIC);
}

//Mission No.6: Intersection->Intersection Stop
void Driving::MissionIntStop() {
	cout << "misison 6" << endl;

	//
	//mission code
	//

	dataContainer->setValue_yolo_speed_ratio(0);

	dataContainer->setValue_UtoP_BRAKE(50);
	Sleep(100);
	dataContainer->setValue_UtoP_BRAKE(100);
	Sleep(100);
	dataContainer->setValue_UtoP_BRAKE(150);
	Sleep(100);
	dataContainer->setValue_UtoP_BRAKE(200);

	///emit(greenRight(false)); /// 이렇게 하지 말고 욜로쪽에서 하나 더 만드는게 나을듯
	cout << "Red" << endl;
	
	while (dataContainer->getValue_yolo_missionID() == INTER_STOP)

	dataContainer->setValue_UtoP_BRAKE(0); /// 브레이크 해제
	dataContainer->setValue_yolo_speed_ratio(1); /// 신호를 받기 전으로 원상복귀		
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

	uint objflag = 0;
	Mat LaneMap = Mat::zeros(imgPath.cols, imgPath.rows, CV_8UC1);
	dataContainer->setValue_UtoP_BRAKE(0);
	while (1) {
		//dynamic obstacle 미션 중 yolo에서 다른 mission trigger가 들어오면 그 mission으로 넘어감
		if (dataContainer->getValue_yolo_missionID() != DYNAMIC_OBSTACLE)
		{
			cout << "mission 8 was called but ended long mission ID : " << dataContainer->getValue_yolo_missionID() << endl;
			break;
		}			

		imgPath = cv::Mat::zeros(600, 600, CV_8UC1);
		vector<Point2d> vecXY = dataContainer->getValue_lidar_VecXY();
		vector<Point2d> vecXYDraw;
		double cenX = imgPath.cols * 0.5, cenY = imgPath.rows *0.99; //the location of LiDAR in the map.
		double scale = cenY / (SICK_SCAN_ROI_Y + 50);				  //obj_Data => imgPath
		//ROI AREA
		double leftEndX = cenX - SICK_SCAN_ROI_X * scale;
		double rightEndX = cenX + SICK_SCAN_ROI_X * scale;
		double topEndY = cenY - SICK_SCAN_ROI_Y * scale;
		double bottomEndY = cenY + SICK_SCAN_ROI_Y * scale;
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
			if (cirCenY < (center.y - 10) && (abs(cenDist-cirCenR) >0.001)) { 
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
			fillPoly(imgPath, ppt, npt, 1, CV_RGB(200, 200, 200));
			circle(imgPath, cirCen, cirCenR, CV_RGB(255, 255, 255), -1, CV_AA);
		}
		//cv::cvtColor(imgPath, imgPath, CV_BGR2GRAY);
		threshold(imgPath, imgPath, 1, 160, THRESH_BINARY_INV);
		LaneMap = Scalar::all(160);
		Mat imgLane = getLaneData(scoreStep);
		imgPath -= imgLane;
		LaneMap -= imgLane;

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

		Point2d stepFirst(cenX + onestep * cos(CV_PI*(90 + goTheta1) / 180), cenY - (onestep*sin(CV_PI*(90 + goTheta1) / 180)));
		Point2d stepSecond(stepFirst.x + onestep * cos(CV_PI*(90 + goTheta2) / 180), stepFirst.y - (onestep*sin(CV_PI*(90 + goTheta2) / 180)));

		double desired_steering = goTheta1 * steerRatio + goTheta2 * (1 - steerRatio);
		Point2d pntF(cenX + onestep * 1.5 * cos(CV_PI*(90 + desired_steering) / 180), cenY - onestep * 1.5*sin(CV_PI*(90 + desired_steering) / 180));

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
		desired_speed = DynamicMaxSpeed *scorePath /scoreLaneMap;

		double thDist = stopDist * scale;
		circle(imgPath, center, thDist, Scalar::all(255));

		double objClose = -1;
		if (objdist.size() == 0){
			cv::arrowedLine(imgPath, center, pntF, CV_RGB(255, 255, 255), 2);
		}
		else{
			objClose = objdist.at(distance(objdist.begin(), min_element(objdist.begin(), objdist.end())));
			if (objClose >0 && objClose < thDist){
				desired_brake = 200;
				desired_speed = 0;
				objflag = 1;
				cout << "STOP!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			}
			else{
				cv::arrowedLine(imgPath, center, pntF, CV_RGB(255, 255, 255), 2);
			}
		}
		//////////////////////////////////////////////////
		////Final Control the steering angle and speed////
		//////////////////////////////////////////////////
		//imshow("Map", LaneMap);
		imshow("Path", imgPath);
		PASIVcontrol(desired_speed, desired_steering, desired_brake);
		QImage image1 = QImage((uchar*)imgPath.data, imgPath.cols, imgPath.rows, imgPath.step, QImage::Format_RGB888);
		dataContainer->setValue_ui_pathmap(image1);

		///////////////////////////////////////////////////////
		////Trigger for ending the Dynamic Obstacle Mission////
		///////////////////////////////////////////////////////
		if (objflag == 1){
			if (objClose > thDist || objdist.size()==0) {
				objflag = 0;
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

//// functions which switch radian and degree
double Driving::rad2deg(double radian) { return radian * 180 / PI; }
double Driving::deg2rad(double degree) { return degree * PI / 180; }

void Driving::autoMode() {
	//emit(send2View(7));
	while (1) {
		cout << "automode function called" << endl;
		cout << dataContainer->getValue_yolo_missionID() << endl;
		if (dataContainer->getValue_yolo_missionID() == PARKING) { emit(currentMission(PARKING));MissionParking(); }
		else if (dataContainer->getValue_yolo_missionID() == INTER_LEFT) { emit(currentMission(INTER_LEFT));emit(send2View(2)); MissionIntLeft(); }
		else if (dataContainer->getValue_yolo_missionID() == INTER_RIGHT) { emit(currentMission(INTER_RIGHT));emit(send2View(3)); MissionIntRight(); }
		else if (dataContainer->getValue_yolo_missionID() == INTER_STRAIGHT) { emit(currentMission(INTER_STRAIGHT));MissionIntStraight(); }
		else if (dataContainer->getValue_yolo_missionID() == STATIC_OBSTACLE) { emit(currentMission(STATIC_OBSTACLE));MissionStaticObs(); }
		else if (dataContainer->getValue_yolo_missionID() == DYNAMIC_OBSTACLE) { emit(currentMission(DYNAMIC_OBSTACLE));MissionDynamicObs(); }
		else if (dataContainer->getValue_yolo_missionID() == INTER_STOP) { emit(currentMission(INTER_STOP));MissionIntStop(); }
		else { emit(currentMission(BASIC));emit(send2View(1)); Basic(BASIC); }
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

	///////////////////////
	////Mission Control////
	///////////////////////

MissionUpdate::MissionUpdate() {
	dataContainer = DataContainer::getInstance();
}

void MissionUpdate::MissionIDUpdate() {

	while (true)
	{
		/*

	/// dataContainer의 yolo map을 받아서 mission number를 업데이트 하는 함수
	//map 대신 array나 datacontainer에 각 미션 위험도를 int type으로 만들어서 바꾸자
	int mission = 0;
	map<string, int> temp = dataContainer->getValue_yolo_missions();

	/*
	일정 거리 이하인 미션 실행
	static
	dynamic
	intersectionStop
	intersectionStraight
	intersectionLeft
	intersectionRight
	parking
	*/

	/*
	if (temp.find("parking")->second == 3) {
		dataContainer->setValue_yolo_missionID(PARKING);
	}
	else if (temp.find("intersectionLeft")->second == 3) {
		dataContainer->setValue_yolo_missionID(INTER_LEFT);
	}
	else if (temp.find("intersectionRight")->second == 3) {
		dataContainer->setValue_yolo_missionID(INTER_RIGHT);
	}
	else if (temp.find("intersectionStraight")->second == 3) {
		dataContainer->setValue_yolo_missionID(INTER_STRAIGHT);
	}
	else if (temp.find("staticObstacle")->second == 3) {
		dataContainer->setValue_yolo_missionID(STATIC_OBSTACLE);
	}
	else if (temp.find("dynamicObstacle")->second == 3) {
		dataContainer->setValue_yolo_missionID(DYNAMIC_OBSTACLE);
	}
	else if (temp.find("intersectionStop")->second == 3) {
		dataContainer->setValue_yolo_missionID(INTER_STOP);
	}

	*/
	/// kidSafe나 bust는 mission number에 병렬적으로 들어올 수 있으므로 이에 따라 속도 비율을 조정한다.
	/*
	if (temp.find("kidSafe")->second == 3) { dataContainer->setValue_yolo_speed_ratio(0.9); }
	else { dataContainer->setValue_yolo_speed_ratio(1); } /// 지나간 후에는 다시 원상복귀한다. 이 때 원상복귀할때까지 시간 조절이 필요할 수 있음.

	if (temp.find("bust")->second == 3) { dataContainer->setValue_yolo_speed_ratio(0.9); }
	else { dataContainer->setValue_yolo_speed_ratio(1); }

	*/

		/// 신호등에 따라 갈지 말지를 결정하는 함수
		/// ISSUE: BRAKE 정도가 PASIV와 충돌할 가능성은? PASIV에서 주행 명령을 내리면 다시 가버릴 수 있음.
		
		
	}
}