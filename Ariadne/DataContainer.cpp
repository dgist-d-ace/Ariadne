#include "DataContainer.h"
#include <iostream>
#include <QMutex>

using namespace std;

bool DataContainer::instanceFlag = false;
DataContainer* DataContainer::instance = NULL;
// BK: Ariadne* DataContainer::window = NULL;

DataContainer* DataContainer::getInstance() {
	if (!instance) {
		instance = new DataContainer();
		instanceFlag = true;
	}
	return instance;
}

// Ariadne* DataContainer::getWindow() { return window; }
// void DataContainer::setWindow(Ariadne* w) { window = w; }

//
// 센서 연결 상태 정보
//

int DataContainer::getValue_platform_status() { mtx_platform_status.lock(); int temp = m_platform_status; mtx_platform_status.unlock(); return temp; }
int DataContainer::getValue_lidar_status() { mtx_lidar_status.lock(); int temp = m_lidar_status; mtx_lidar_status.unlock(); return temp; }
int DataContainer::getValue_camera1_status() { mtx_camera1_status.lock(); int temp = m_camera1_status; mtx_camera1_status.unlock(); return temp; }
int DataContainer::getValue_gps_status() { mtx_gps_status.lock(); int temp = m_gps_status; mtx_gps_status.unlock(); return temp; }

void DataContainer::setValue_platform_status(int value) { mtx_platform_status.lock(); m_platform_status = value; mtx_platform_status.unlock(); }
void DataContainer::setValue_lidar_status(int value) { mtx_lidar_status.lock(); m_lidar_status = value; mtx_lidar_status.unlock(); }
void DataContainer::setValue_camera1_status(int value) { mtx_camera1_status.lock(); m_camera1_status = value; mtx_camera1_status.unlock(); }
void DataContainer::setValue_gps_status(int value) { mtx_gps_status.lock(); m_gps_status = value; mtx_gps_status.unlock(); }

void DataContainer::updateValue_platform_status() { mtx_platform_status.lock(); m_platform_status < 256 ? m_platform_status++ : 256; mtx_platform_status.unlock(); }
void DataContainer::updateValue_lidar_status() { mtx_lidar_status.lock(); m_lidar_status < 256 ? m_lidar_status++ : 256; mtx_lidar_status.unlock(); }
void DataContainer::updateValue_camera1_status() { mtx_camera1_status.lock(); m_camera1_status < 256 ? m_camera1_status++ : 256; mtx_camera1_status.unlock(); }
void DataContainer::updateValue_gps_status() { mtx_gps_status.lock(); m_gps_status < 256 ? m_gps_status++ : 256; mtx_gps_status.unlock(); }



//
//	플랫폼
//

//UPPER to PCU (플랫폼에 보내는 값)
int DataContainer::getValue_UtoP_AorM() { mtx_UtoP_AorM.lock(); int temp = m_UtoP_AorM; mtx_UtoP_AorM.unlock(); return temp; }
int DataContainer::getValue_UtoP_E_STOP() { mtx_UtoP_E_STOP.lock(); int temp = m_UtoP_E_STOP; mtx_UtoP_E_STOP.unlock(); return temp; }
int DataContainer::getValue_UtoP_GEAR() { mtx_UtoP_GEAR.lock(); int temp = m_UtoP_GEAR; mtx_UtoP_GEAR.unlock(); return temp; }
int DataContainer::getValue_UtoP_SPEED() { mtx_UtoP_SPEED.lock(); int temp = m_UtoP_SPEED; mtx_UtoP_SPEED.unlock(); return temp; }
int DataContainer::getValue_UtoP_STEER() { mtx_UtoP_STEER.lock(); int temp = m_UtoP_STEER; mtx_UtoP_STEER.unlock(); return temp; }
int DataContainer::getValue_UtoP_BRAKE() { mtx_UtoP_BRAKE.lock(); int temp = m_UtoP_BRAKE; mtx_UtoP_BRAKE.unlock(); return temp; }
int DataContainer::getValue_UtoP_ALIVE() { mtx_UtoP_ALIVE.lock(); int temp = m_UtoP_ALIVE; mtx_UtoP_ALIVE.unlock(); return temp; }

void DataContainer::setValue_UtoP_AorM(int value) { mtx_UtoP_AorM.lock(); m_UtoP_AorM = (value != 0 ? value != 1 ? 0 : 1 : 0); mtx_UtoP_AorM.unlock(); }
void DataContainer::setValue_UtoP_E_STOP(int value) { mtx_UtoP_E_STOP.lock(); m_UtoP_E_STOP = (value != 0 ? value != 1 ? 1 : 1 : 0); mtx_UtoP_E_STOP.unlock(); }
void DataContainer::setValue_UtoP_GEAR(int value) { mtx_UtoP_GEAR.lock(); m_UtoP_GEAR = (value > 0 ? (value < 2 ? value : 2) : 0); mtx_UtoP_GEAR.unlock(); }
void DataContainer::setValue_UtoP_SPEED(int value) { mtx_UtoP_SPEED.lock(); m_UtoP_SPEED = (value > 0 ? (value < 200 ? value : 200) : 0); mtx_UtoP_SPEED.unlock(); }
void DataContainer::setValue_UtoP_STEER(int value) { mtx_UtoP_STEER.lock(); m_UtoP_STEER = (value > -2000 ? (value < 2000 ? value : 2000) : -2000); mtx_UtoP_STEER.unlock(); }
void DataContainer::setValue_UtoP_BRAKE(int value) { mtx_UtoP_BRAKE.lock(); m_UtoP_BRAKE = (value > 0 ? (value < 200 ? value : 200) : 0); mtx_UtoP_BRAKE.unlock(); }
void DataContainer::setValue_UtoP_ALIVE(int value) { mtx_UtoP_ALIVE.lock(); m_UtoP_ALIVE = (value % 256); mtx_UtoP_ALIVE.unlock(); }

// 값을 통째로 가져오고, 지정하는 함수. 순서대로 (AorM, E_STOP, GEAR, SPEED, STEER, BRAKE)
std::vector<int> DataContainer::getValue_UtoP_list()
{
	std::vector<int> value_UtoP_list{ getValue_UtoP_AorM(), getValue_UtoP_E_STOP(), getValue_UtoP_GEAR()
										, getValue_UtoP_SPEED(), getValue_UtoP_STEER(), getValue_UtoP_BRAKE() };
	return value_UtoP_list;
}

void DataContainer::setValue_UtoP_list(int AorM, int E_STOP, int GEAR, int SPEED, int STEER, int BRAKE)
{
	setValue_UtoP_AorM(AorM);
	setValue_UtoP_E_STOP(E_STOP);
	setValue_UtoP_GEAR(GEAR);
	setValue_UtoP_SPEED(SPEED);
	setValue_UtoP_STEER(STEER);
	setValue_UtoP_BRAKE(BRAKE);
}

//PCU to UPPER (플랫폼에서 받아오는 값)
int DataContainer::getValue_PtoU_AorM() { mtx_PtoU_AorM.lock(); int temp = m_PtoU_AorM; mtx_PtoU_AorM.unlock(); return temp; }
int DataContainer::getValue_PtoU_E_STOP() { mtx_PtoU_E_STOP.lock(); int temp = m_PtoU_E_STOP; mtx_PtoU_E_STOP.unlock(); return temp; }
int DataContainer::getValue_PtoU_GEAR() { mtx_PtoU_GEAR.lock(); int temp = m_PtoU_GEAR; mtx_PtoU_GEAR.unlock(); return temp; }
int DataContainer::getValue_PtoU_SPEED() { mtx_PtoU_SPEED.lock(); int temp = m_PtoU_SPEED; mtx_PtoU_SPEED.unlock(); return temp; }
int DataContainer::getValue_PtoU_STEER() { mtx_PtoU_STEER.lock(); int temp = m_PtoU_STEER; mtx_PtoU_STEER.unlock(); return temp; }
int DataContainer::getValue_PtoU_BRAKE() { mtx_PtoU_BRAKE.lock(); int temp = m_PtoU_BRAKE; mtx_PtoU_BRAKE.unlock(); return temp; }
long DataContainer::getValue_PtoU_ENC() { mtx_PtoU_ENC.lock(); long temp = m_PtoU_ENC; mtx_PtoU_ENC.unlock(); return temp; }
int DataContainer::getValue_PtoU_ALIVE() { mtx_PtoU_ALIVE.lock(); int temp = m_PtoU_ALIVE; mtx_PtoU_ALIVE.unlock(); return temp; }

void DataContainer::setValue_PtoU_AorM(int value) { mtx_PtoU_AorM.lock(); m_PtoU_AorM = value; mtx_PtoU_AorM.unlock(); }
void DataContainer::setValue_PtoU_E_STOP(int value) { mtx_PtoU_E_STOP.lock(); m_PtoU_E_STOP = value; mtx_PtoU_E_STOP.unlock(); }
void DataContainer::setValue_PtoU_GEAR(int value) { mtx_PtoU_GEAR.lock(); m_PtoU_GEAR = value; mtx_PtoU_GEAR.unlock(); }
void DataContainer::setValue_PtoU_SPEED(int value) { mtx_PtoU_SPEED.lock(); m_PtoU_SPEED = value; mtx_PtoU_SPEED.unlock(); }
void DataContainer::setValue_PtoU_STEER(int value) { mtx_PtoU_STEER.lock(); m_PtoU_STEER = value; mtx_PtoU_STEER.unlock(); }
void DataContainer::setValue_PtoU_BRAKE(int value) { mtx_PtoU_BRAKE.lock(); m_PtoU_BRAKE = value; mtx_PtoU_BRAKE.unlock(); }
void DataContainer::setValue_PtoU_ENC(long value) { mtx_PtoU_ENC.lock(); m_PtoU_ENC = value; mtx_PtoU_ENC.unlock(); }
void DataContainer::setValue_PtoU_ALIVE(int value) { mtx_PtoU_ALIVE.lock(); m_PtoU_ALIVE = value; mtx_PtoU_ALIVE.unlock(); }

// 값을 통째로 가져오고, 지정하는 함수. 순서대로 (AorM, E_STOP, GEAR, SPEED, STEER, BRAKE, ENC)
std::vector<int> DataContainer::getValue_PtoU_list()
{
	std::vector<int> value_PtoU_list{ getValue_PtoU_AorM(), getValue_PtoU_E_STOP(), getValue_PtoU_GEAR()
				, getValue_PtoU_SPEED(), getValue_PtoU_STEER(), getValue_PtoU_BRAKE(), getValue_PtoU_ENC() };
	return value_PtoU_list;
}

void DataContainer::setValue_PtoU_list(int AorM, int E_STOP, int GEAR, int SPEED, int STEER, int BRAKE, long ENC)
{
	setValue_PtoU_AorM(AorM);
	setValue_PtoU_E_STOP(E_STOP);
	setValue_PtoU_GEAR(GEAR);
	setValue_PtoU_SPEED(SPEED);
	setValue_PtoU_STEER(STEER);
	setValue_PtoU_BRAKE(BRAKE);
	setValue_PtoU_ENC(ENC);
}


// UPPER to PCU or PCU to UPPER 값 보여주기
void DataContainer::show_UtoP()
{
	cout << "[ UPPER to PCU DATA |";
	cout << " AorM: " << getValue_UtoP_AorM();
	cout << " E-STOP: " << getValue_UtoP_E_STOP();
	cout << " GEAR: " << getValue_UtoP_GEAR();
	cout << " SPEED: " << getValue_UtoP_SPEED();
	cout << " STEER: " << getValue_UtoP_STEER();
	cout << " BRAKE: " << getValue_UtoP_BRAKE();
	cout << " ALIVE: " << getValue_UtoP_ALIVE();
	cout << " ]" << endl;
}


void DataContainer::show_PtoU()
{
	cout << "[ PCU to UPPER DATA |";
	cout << " AorM: " << getValue_PtoU_AorM();
	cout << " E-STOP: " << getValue_PtoU_E_STOP();
	cout << " GEAR: " << getValue_PtoU_GEAR();
	cout << " SPEED: " << getValue_PtoU_SPEED();
	cout << " STEER: " << getValue_PtoU_STEER();
	cout << " BRAKE: " << getValue_PtoU_BRAKE();
	cout << " ENC: " << getValue_PtoU_ENC();
	cout << " ALIVE: " << getValue_PtoU_ALIVE();
	cout << " ]" << endl;
}

//
//	라이다
//


// 수정 필요
/*
void setData_lidar_object(object* objL, int num){}
void getData_lidar_object(object* objM){}
void deleteData_lidar_object(){}
*/

//
//	카메라1
//

int DataContainer::getValue_camera1_steer() { mtx_camera1_steer.lock(); int temp = m_camera1_steer; mtx_camera1_steer.unlock(); return temp; }
void DataContainer::setValue_camera1_steer(int value) { mtx_camera1_steer.lock(); m_camera1_steer = value; mtx_camera1_steer.unlock(); }

//
//	GPS
//

int DataContainer::get_missiontrigger() { mtx_missiontrigger.lock(); int temp = m_missiontrigger; mtx_missiontrigger.unlock(); return temp; }
void DataContainer::set_missiontrigger(int value) { mtx_missiontrigger.lock(); m_missiontrigger = value; mtx_missiontrigger.unlock();}

int DataContainer::get_gps_geofence() { mtx_gps_geofence.lock(); int temp = m_missiontrigger; mtx_gps_geofence.unlock(); return temp; }
void DataContainer::set_gps_geofence(int value) { mtx_gps_geofence.lock(); m_gps_geofence = value; mtx_gps_geofence.unlock();}