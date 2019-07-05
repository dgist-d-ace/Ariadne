// This is for executing each mission

#include "MissionAct.h"
#include "SensorStatus.h"
#include <iostream>

using namespace std;

DataContainer* dataContainer;

Mission::Mission() {
	dataContainer = DataContainer::getInstance();
}

// GPS�� �̺�Ʈ Ʈ���Ÿ� �̿��� �� �̼����� �Ѿ�� �Լ�
int Mission::DecideMission()
{
	int k;
	// step 0. manual mode���� auto mode�� ��ȯ
	dataContainer->setValue_UtoP_AorM(1);
	// step 1. gps�� mission trigger�� �޾ƿͼ� k�� �Ǵ��ϴ� ���� ��ġ��

	return k;
}

void Mission::setMission(int k)// k��° �̼����� �Ѿ
{
	if (k == 0) { doMission0(); }
	// if (k ==1) { doMission1(); } ...etc
}

bool Mission::emergencyStop() // Lidar�� dangerous region�� ������ E-stop
{
	return true;
}

void Mission::Connect()
{
    // UI�� comport�� �޾ƿͼ� �ش� ������ connect �� �� thread�� ����
    SensorStatus* sensorStatus = new SensorStatus;
}

void Mission::doMission0() // ���� �⺻ �����϶� ����
{	
	cout << "�⺻ ���� ��带 �����մϴ�." << endl;

	while (!emergencyStop())
	{
		int temp = dataContainer->getValue_camera1_steer();
		dataContainer->setValue_UtoP_list(1, 0, 0, 0, temp, 0);
	}
}