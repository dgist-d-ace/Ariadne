#pragma once

#include "DataContainer.h"
#include "CSerialPort.h"
#include <QThread>

class ComPlatform :
	public CSerialPort
{
private:
	DataContainer *dataContainer;
	BYTE writeBuffer[14];
	BYTE upper(int n) { return (BYTE)((n) & 0xff); }
	BYTE lower(int n) { return (BYTE)((n >> 8) & 0xff); }
	/*
	int f_mode = 1;
	int f_Estop = 0;
	int f_gear = 0;
	int f_speed = 20;
	int f_steer = 0;  //steer �ݴ�γ���
	int f_brake = 0;
	*/
	void setWritePram(BYTE* writeBuffer);

	int setData_steering(double desired_steering);
	int setData_speed(double desired_speed);
	int setData_brake(double desired_brake);
	double previous_steering;
public:
	ComPlatform();
	~ComPlatform();
	bool MyCommRead();
	void MyCommWrite();
};

