// This is for executing each mission

#include "MissionAct.h"
#include <iostream>

using namespace std;



Mission::Mission() {
	dataContainer = DataContainer::getInstance();
}

// GPS의 이벤트 트리거를 이용해 각 미션으로 넘어가는 함수
int Mission::DecideMission()
{
	int k;
	// step 0. manual mode에서 auto mode로 전환
	dataContainer->setValue_UtoP_AorM(1);
	// step 1. gps의 mission trigger를 받아와서 k를 판단하는 과정 거치기

	return k;
}

void Mission::setMission(int k)// k번째 미션으로 넘어감
{
	if (k == 0) { doMission0(); }
	// if (k ==1) { doMission1(); } ...etc
}

bool Mission::emergencyStop() // Lidar의 dangerous region이 가까우면 E-stop
{
	return true;
}

void Mission::doMission0() // 가장 기본 상태일때 주행
{	
	cout << "기본 주행 모드를 시작합니다." << endl;

	/// SensorStatus* sensorStatus = new SensorStatus;

	while (!emergencyStop())
	{
		int temp = dataContainer->getValue_camera1_steer();
		dataContainer->setValue_UtoP_list(1, 0, 0, 0, temp, 0);
	}
}