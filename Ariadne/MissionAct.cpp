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
	
	return 1;
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

//VOSS algorithm (VOronoi Score System)
void Driving::Basic() {
	
	dataContainer->setValue_UtoP_AorM(1);
	clock_t start, end;
	while (1)
	{
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
		Point points[1][3];
		points[0][0] = Point(center.x - carW, center.y);
		points[0][1] = Point(cenX - SICK_SCAN_ROI_X, center.y);
		points[0][2] = Point(cenX - SICK_SCAN_ROI_X, center.y - (SICK_SCAN_ROI_X - carW)*sqrt(3)); 
		const Point* pnts[1] = { points[0] };
		int npt2[] = { 3 };
		//Right area where platform can not go (right 60degrees)
		Point points2[1][3];
		points2[0][0] = Point(center.x + carW, center.y);
		points2[0][1] = Point(cenX + SICK_SCAN_ROI_X, center.y);
		points2[0][2] = Point(cenX + SICK_SCAN_ROI_X, center.y - (SICK_SCAN_ROI_X - carW) * sqrt(3));
		const Point* pnts2[1] = { points2[0] };
		//Drawing
		fillPoly(imgPath, pnts, npt2, 1, CV_RGB(150, 0, 0));
		fillPoly(imgPath, pnts2, npt2, 1, CV_RGB(150, 0, 0));
	
		cv::cvtColor(imgPath, scoreMap, CV_BGR2GRAY);
		threshold(scoreMap, scoreMap, 1, 10, THRESH_BINARY_INV);


			//////////////////////////////////////////////
			////Fill the Regions which mean out of ROI////
			//////////////////////////////////////////////
		//rectangle(imgPath, Point2d(0, 0), Point2d(imgPath.cols, topEndY), CV_RGB(255, 255, 255), -1, CV_AA, 0);
		//rectangle(imgPath, Point2d(0, 0), Point2d(leftEndX, imgPath.rows), CV_RGB(255, 255, 255), -1, CV_AA, 0);
		//rectangle(imgPath, Point2d(imgPath.cols, imgPath.rows), Point2d(0, cenY), CV_RGB(255, 255, 255), -1, CV_AA, 0);
		//rectangle(imgPath, Point2d(imgPath.cols, imgPath.rows), Point2d(rightEndX, 0), CV_RGB(255, 255, 255), -1, CV_AA, 0);

		//rectangle(scoreMap, Point2d(0, 0), Point2d(imgPath.cols, topEndY), CV_RGB(255, 255, 255), -1, CV_AA, 0);
		//rectangle(scoreMap, Point2d(0, 0), Point2d(leftEndX, imgPath.rows), CV_RGB(255, 255, 255), -1, CV_AA, 0);
		//rectangle(scoreMap, Point2d(imgPath.cols, imgPath.rows), Point2d(0, cenY), CV_RGB(255, 255, 255), -1, CV_AA, 0);
		//rectangle(scoreMap, Point2d(imgPath.cols, imgPath.rows), Point2d(rightEndX, 0), CV_RGB(255, 255, 255), -1, CV_AA, 0);
		//imshow("1", imgPath);

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

			//circle(img, cirCen, objDataSet[i][2] * scale, CV_RGB(255, 0, 0), -1, CV_AA);

			cenDist = sqrt(pow((cirCen.x - cenX), 2) + pow((cirCen.y - cenY), 2));
			objdist.push_back(cenDist);
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

		cv::cvtColor(imgPath, imgPath, CV_BGR2GRAY);
		threshold(imgPath, imgPath, 1, 10, THRESH_BINARY_INV);

		//MAKING VORNOI FIELD
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
		//uchar *asd = imgPath.data;
		
		//cout << (uint)asd[imgPath.cols*(imgPath.rows/2)+imgPath.cols/2] << endl;
		//cout << (uint)asd[imgPath.cols*(1) + 1] << endl;
		//imshow("Map", scoreMap);

			//////////////////////////////////////////////////////////////////////////////
			////Determine the desired Steering Angle in Score System with Vornoi Field////
			//////////////////////////////////////////////////////////////////////////////

		//REGION OF WORKABLE ANGLE: 60 ~ 120, with interval=5 degrees
		vector<uint> score[169]; //include the scores at [90,85, 95, 80, 100, 75, 105, 70, 110, 65, 115, 60, 120]degrees
		vector<int> theta = { 0, -5, 5, -10, 10, -15, 15, -20, 20, -25, 25, -30, 30 }; //The steering angle candidates
		uchar onestep = (1210) * scale; //carH; //mean how much car move go in 0.5s. / 12km/hour -> 3m/second
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
			circle(cirGray, step1, carW / 2, CV_RGB(10, 10, 10), -1, CV_AA, 0);
			for (int j = 0; j < theta.size(); j++)
			{
				cirGray2 = Mat::zeros(imgPath.rows, imgPath.cols, CV_8UC1);
				scoreTheta2 = 90 + theta.at(j);
				Point2d step2(step1.x + onestep * cos(CV_PI*scoreTheta2 / 180), step1.y -onestep* sin(CV_PI*scoreTheta2 / 180));
				circle(cirGray2, step2, carW / 2, CV_RGB(10,10,10), -1, CV_AA, 0);
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

			/////////////////////////////////////////////////////////////////////
			////Determine the desired Speed in Score System with Vornoi Field////
			/////////////////////////////////////////////////////////////////////
		
		//Add the line data in the scoreMap and img Path.
		uint scoreofMap=0; //total sum of scoreMap
		uint scoreofPath=0;//total sum of imgPath
		uint speedHigh = 8;
		uint speedLow = 3;
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
			continue;
		}
		else {
			double objClose = objdist.at(distance(objdist.begin(), min_element(objdist.begin(), objdist.end())));
			if (desired_speed < speedLow) {
				desired_speed = speedLow; cout << "Fucking LOW SCORE!!" << endl;
				cv::arrowedLine(imgPath, center, pntF, CV_RGB(250, 250, 250), 5);
				cv::arrowedLine(imgPath, center, stepFirst, CV_RGB(100, 100, 100), 3);
				cv::arrowedLine(imgPath, stepFirst, stepSecond, CV_RGB(100, 100, 100), 3);
			
			}
			else if (objClose < 750 * scale) { 
				desired_speed = speedLow; cout << "TOO CLOSE!!!!" << endl;
				cv::arrowedLine(imgPath, center, pntF, CV_RGB(250, 250, 250), 5);
				cv::arrowedLine(imgPath, center, stepFirst, CV_RGB(100, 100, 100), 3);
				cv::arrowedLine(imgPath, stepFirst, stepSecond, CV_RGB(100, 100, 100), 3);
			
			}
			else {
				cv::arrowedLine(imgPath, center, pntF, CV_RGB(250, 250, 250), 5);
				cv::arrowedLine(imgPath, center, stepFirst, CV_RGB(100, 100, 100), 3);
				cv::arrowedLine(imgPath, stepFirst, stepSecond, CV_RGB(100, 100, 100), 3);
			}

			cout << "desired_speed = " << desired_speed << endl;
			cout << "desired_steer = " << desired_steering << endl;
			imshow("LaneMap", scoreMap);
			imshow("DrawLiDARData", imgPath);
		}



			//////////////////////////////////////////////////
			////Final Control the steering angle and speed////
			//////////////////////////////////////////////////
		//setData_steering(desired_steering);
		//setData_speed(desired_speed);
		//dataContainer->setValue_UtoP_SPEED(30);
	
		//end = clock();
		//cout << "time: " << (double)(end - start) / 1000 << "sec" << endl ;


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
