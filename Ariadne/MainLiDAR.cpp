#include "LastOfLiDAR.h"
#include "Planner.hpp"

State getStartState(int startX, int startY, int startTheta, int startSteer, int startSpeed) {
	//to do: read from yml file
	//start point: center of bottom, theta: always 18.
	return State(startX, startY, startTheta, startSteer, startSpeed);
}

State getTargetState(int targetX, int targetY, int targetTheta, int targetSteer, int targetSpeed) {
	//to do: read from yml file
	//target point: center of top, alpha: random
	return State(targetX, targetY, targetTheta, targetSteer, targetSpeed);
}

Map getMatMap(Mat &imgLiDAR) {
	//to do: read from yml file
	//start point: center of bottom, theta: always 18.
	return Map(imgLiDAR);
}

int LastOfLiDAR::funcLiDAR() {
	LastOfLiDAR lol;
	Planner astar;

	/**/
	if (!lol.Initialize()) {
		cout << "Connect ERROR!!!" << endl;
		return -1;
	}

	lol.StartCapture();

	//Mat picLiDAR = imread("maps/intersection1.png", 0); // exeption error: 파일 이름 잘못된거임

	// 교차로 1번 경로를 위한 시작 및 종착점 설정
	//int startX = MAPX / 2, startY = MAPY - BOT_L, startTheta = 305, startSteer = 0, startSpeed = 60; // It will be multiplied by 5, so 18 is 90 degree
	//int targetX = MAPX / 2, targetY = BOT_L, targetTheta = 270, targetSteer = 0, targetSpeed = 60;
	int startX = MAPX / 2, startY = MAPY - BOT_L, startTheta = 270, startSteer = 0 , startSpeed = 60; // It will be multiplied by 5, so 18 is 90 degree
	int targetX = MAPX / 2, targetY = BOT_L, targetTheta = 270, targetSteer = 0, targetSpeed = 60;
	int isInc = 0;

	while (1) {
		if (1) { // lol.m_bDataAvailable
			/**/
			lol.imgLiDAR = Mat::zeros(MAPY, MAPX, CV_8UC3); //default == 2160,3840
			lol.GetValidDataRTheta(lol.validScans);
			lol.Conversion(lol.validScans, lol.finQVecXY);
			lol.Average(lol.finQVecXY, lol.finVecXY);
			lol.Clustering(lol.finVecXY, lol.finObjData);
			lol.Vector(lol.finObjData, lol.finVecData, lol.finBoolData);
			lol.DrawData(lol.finVecXY, lol.finObjData, lol.finVecData, lol.finBoolData, lol.imgLiDAR);

			//cout << "Reset" << endl;
			
			//imshow("DrawLiDARData", resizeLiDAR);
			
			clock_t  begin, end;
			begin = clock();

			//Map mapLiDAR = Map(picLiDAR);
			Map mapLiDAR = Map(lol.imgLiDAR);
			State start = State(startX, startY, startTheta, startSteer, startSpeed);
			State target = State(targetX, targetY, targetTheta, targetSteer, targetSpeed);
			
			int finalSteer = astar.plan(start, target, mapLiDAR);
			//cout << "final : " << finalSteer << " 앙개꿀띠" << endl;

			mapLiDAR.deleteMaps();

			end = clock();
			cout << "LOS 수행시간 : " << (double)(end - begin) / 1000 << "s" << endl;

			int incX = 20, incY = 0, incTheta = 20;

			if (targetX >= MAPX / 2) {
				if (isInc == 1) {
					targetX += incX;
					targetY -= incY;
					targetTheta += incTheta;

					if (targetX > MAPX - BOT_L) {
						targetX -= incX;
						targetY += incY;
						targetTheta -= incTheta;
						isInc = 0;
					}
				}
				else {
					targetX -= incX;
					targetY += incY;
					targetTheta -= incTheta;
				}
			}
			else {
				if (isInc == 0) {
					targetX -= incX;
					targetY -= incY;
					targetTheta -= incTheta;

					if (targetX < BOT_L) {
						targetX += incX;
						targetY += incY;
						targetTheta += incTheta;
						isInc = 1;
					}
				}
				else {
					targetX += incX;
					targetY += incY;
					targetTheta += incTheta;
				}
			}
		}

		int key = waitKey(1);

		if (key == 27) {
			break;
		}
	}

	lol.StopCapture();
	lol.UnInitialize();
	
	return 0;
}