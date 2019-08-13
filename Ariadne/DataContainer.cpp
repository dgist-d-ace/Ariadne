#include "DataContainer.h"
#include <iostream>
#include <QMutex>

using namespace std;

bool DataContainer::instanceFlag = false;
DataContainer* DataContainer::instance = NULL;

DataContainer* DataContainer::getInstance() {
	if (!instance) {
		instance = new DataContainer();
		instanceFlag = true;
	}
	return instance;
}

// port 연결
CString DataContainer::getValue_platform_port() { return m_platform_port; }
void DataContainer::setValue_platform_port(CString port) { m_platform_port = port; }
CString DataContainer::getValue_gps_port() { return m_gps_port;}
void DataContainer::setValue_gps_port(CString port) { m_gps_port = port; }

//
// 센서 연결 상태 정보
//

int DataContainer::getValue_platform_status() { mtx_platform_status.lock(); int temp = m_platform_status; mtx_platform_status.unlock(); return temp; }
int DataContainer::getValue_lidar_status() { mtx_lidar_status.lock(); int temp = m_lidar_status; mtx_lidar_status.unlock(); return temp; }
int DataContainer::getValue_scnn_status() { mtx_scnn_status.lock(); int temp = m_scnn_status; mtx_scnn_status.unlock(); return temp; }
int DataContainer::getValue_gps_status() { mtx_gps_status.lock(); int temp = m_gps_status; mtx_gps_status.unlock(); return temp; }

void DataContainer::setValue_platform_status(int value) { mtx_platform_status.lock(); m_platform_status = value; mtx_platform_status.unlock(); }
void DataContainer::setValue_lidar_status(int value) { mtx_lidar_status.lock(); m_lidar_status = value; mtx_lidar_status.unlock(); }
void DataContainer::setValue_scnn_status(int value) { mtx_scnn_status.lock(); m_scnn_status = value; mtx_scnn_status.unlock(); }
void DataContainer::setValue_gps_status(int value) { mtx_gps_status.lock(); m_gps_status = value; mtx_gps_status.unlock(); }

void DataContainer::updateValue_platform_status() { mtx_platform_status.lock(); m_platform_status < 256 ? m_platform_status++ : 256; mtx_platform_status.unlock(); }
void DataContainer::updateValue_lidar_status() { mtx_lidar_status.lock(); m_lidar_status < 256 ? m_lidar_status++ : 256; mtx_lidar_status.unlock(); }
void DataContainer::updateValue_scnn_status() { mtx_scnn_status.lock(); m_scnn_status < 256 ? m_scnn_status++ : 256; mtx_scnn_status.unlock(); }
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
//	라이다				???????싫비???????
//

vector<cv::Point2d> DataContainer::getValue_lidar_VecXY() { return m_lidar_VecXY; }
queue<vector<vector<double> > > DataContainer::getValue_lidar_Data() { return m_lidar_Data; }
vector<cv::Point2d> DataContainer::getValue_lidar_VecData() { return m_lidar_VecData; }
vector<bool> DataContainer::getValue_lidar_BoolData() { return m_lidar_BoolData; }
cv::Mat DataContainer::getValue_lidar_imgData() { return m_lidar_imgData; }

void DataContainer::setValue_lidar_VecXY(vector<cv::Point2d> VecXY) { m_lidar_VecXY = VecXY; }
void DataContainer::setValue_lidar_Data(queue<vector<vector<double> > > data) { m_lidar_Data = data; }
void DataContainer::setValue_lidar_VecData(vector<cv::Point2d> vecData) { m_lidar_VecData = vecData; }
void DataContainer::setValue_lidar_BoolData(vector<bool> boolData) { m_lidar_BoolData = boolData; }
void DataContainer::setValue_lidar_ImgData(cv::Mat imgData) { m_lidar_imgData = imgData; }


// 수정 필요
/*
void setData_lidar_object(object* objL, int num){}
void getData_lidar_object(object* objM){}
void deleteData_lidar_object(){}
*/

//
//	카메라1
//
void DataContainer::setValue_scnn_lanes(vector<vector<cv::Point2i>> lanes) { mtx_scnn_lanes.lock(); m_scnn_lanes = lanes; mtx_scnn_lanes.unlock(); }
void DataContainer::setValue_scnn_existLanes(vector<int> existLanes) { mtx_scnn_existLanes.lock(); m_scnn_existLanes = existLanes; mtx_scnn_existLanes.unlock(); }
vector<vector<cv::Point2i>> DataContainer::getValue_scnn_lanes() { mtx_scnn_lanes.lock(); vector<vector<cv::Point2i>> temp = m_scnn_lanes; mtx_scnn_lanes.unlock(); return temp;}
vector<int> DataContainer::getValue_scnn_existLanes() { mtx_scnn_existLanes.lock(); vector<int> temp = m_scnn_existLanes;  mtx_scnn_existLanes.unlock(); return temp; }

//
//	GPS
//
double DataContainer::getValue_imu_roll() { mtx_imu_roll.lock(); double temp = m_gps_roll; mtx_imu_roll.unlock(); return temp; }
double DataContainer::getValue_imu_pitch() { mtx_imu_pitch.lock(); double temp = m_gps_pitch; mtx_imu_pitch.unlock(); return temp; }
double DataContainer::getValue_imu_yaw() { mtx_imu_yaw.lock(); double temp = m_gps_yaw; mtx_imu_yaw.unlock(); return temp; }
double DataContainer::getValue_imu_accelX() { mtx_imu_accelX.lock(); double temp = m_gps_accelX; mtx_imu_accelX.unlock(); return temp; }
double DataContainer::getValue_imu_accelY() { mtx_imu_accelY.lock(); double temp = m_gps_accelY; mtx_imu_accelY.unlock(); return temp; }
double DataContainer::getValue_imu_accelZ() { mtx_imu_accelZ.lock(); double temp = m_gps_accelZ; mtx_imu_accelZ.unlock(); return temp; }

int DataContainer::getValue_gps_valid() { mtx_gps_valid.lock();	int temp = m_gps_Valid;	mtx_gps_valid.unlock(); return temp; }
double DataContainer::getValue_gps_latitude() { mtx_gps_latitude.lock(); double temp = m_gps_latitude; mtx_gps_latitude.unlock(); return temp; }
double DataContainer::getValue_gps_longitude() { mtx_gps_longitude.lock(); double temp = m_gps_longitude; mtx_gps_longitude.unlock(); return temp; }
double DataContainer::getValue_gps_velocity() { mtx_gps_velocity.lock(); double temp = m_gps_velocity; mtx_gps_velocity.unlock(); return temp; }
double DataContainer::getValue_gps_heading() { mtx_gps_heading.lock(); double temp = m_gps_heading; mtx_gps_heading.unlock(); return temp; }

double DataContainer::getValue_flted_x() { mtx_gps_flted_x.lock(); double temp = m_gps_flted_x; mtx_gps_flted_x.unlock(); return temp; }
double DataContainer::getValue_flted_y() { mtx_gps_flted_y.lock(); double temp = m_gps_flted_y; mtx_gps_flted_y.unlock(); return temp; }
double DataContainer::getValue_flted_Vx() { mtx_gps_flted_vx.lock(); double temp = m_gps_flted_Vx; mtx_gps_flted_vx.unlock(); return temp; }
double DataContainer::getValue_flted_Vy() { mtx_gps_flted_vy.lock(); double temp = m_gps_flted_Vy; mtx_gps_flted_vy.unlock(); return temp; }

void DataContainer::setValue_imu_roll(double value) { mtx_imu_roll.lock(); m_gps_roll = value; mtx_imu_roll.unlock(); }
void DataContainer::setValue_imu_pitch(double value) { mtx_imu_pitch.lock(); m_gps_pitch = value; mtx_imu_pitch.unlock(); }
void DataContainer::setValue_imu_yaw(double value) { mtx_imu_yaw.lock(); m_gps_yaw = value; mtx_imu_yaw.unlock(); }
void DataContainer::setValue_imu_accelX(double value) { mtx_imu_accelX.lock(); m_gps_accelX = value; mtx_imu_accelX.unlock(); }
void DataContainer::setValue_imu_accelY(double value) { mtx_imu_accelY.lock(); m_gps_accelY = value; mtx_imu_accelY.unlock(); }
void DataContainer::setValue_imu_accelZ(double value) { mtx_imu_accelZ.lock(); m_gps_accelZ = value; mtx_imu_accelZ.unlock(); }

void DataContainer::resetValue_gps_valid() { mtx_gps_valid.lock(); m_gps_Valid = 0;mtx_gps_valid.unlock(); }
void DataContainer::count_gps_valid() { mtx_gps_valid.lock(); m_gps_Valid++; mtx_gps_valid.unlock(); }
void DataContainer::setValue_gps_latitude(double value) { mtx_gps_latitude.lock(); m_gps_latitude = value; mtx_gps_latitude.unlock(); }
void DataContainer::setValue_gps_longitude(double value) { mtx_gps_longitude.lock(); m_gps_longitude = value; mtx_gps_longitude.unlock(); }
void DataContainer::setValue_gps_velocity(double value) { mtx_gps_velocity.lock(); m_gps_velocity = value; mtx_gps_velocity.unlock(); }
void DataContainer::setValue_gps_heading(double value) { mtx_gps_heading.lock(); m_gps_heading = value; mtx_gps_heading.unlock(); }

void DataContainer::setValue_flted_x(double value) { mtx_gps_flted_x.lock(); m_gps_flted_x = value; mtx_gps_flted_x.unlock(); }
void DataContainer::setValue_flted_y(double value) { mtx_gps_flted_y.lock(); m_gps_flted_y = value; mtx_gps_flted_y.unlock(); }
void DataContainer::setValue_flted_Vx(double value) { mtx_gps_flted_vx.lock(); m_gps_flted_Vx = value; mtx_gps_flted_vx.unlock(); }
void DataContainer::setValue_flted_Vy(double value) { mtx_gps_flted_vy.lock(); m_gps_flted_Vy = value; mtx_gps_flted_vy.unlock(); }

void DataContainer::show_imu()
{
	printf("[ imu data | roll : %6.2f pitch : %6.2f yaw : %6.2f accel X : %6.3f accel Y : %6.3f accel Z : %6.3f ]\n",
		m_gps_roll, m_gps_pitch, m_gps_yaw, m_gps_accelX, m_gps_accelY, m_gps_accelZ);
}

void DataContainer::show_gps()
{
	printf("[ gps data | valid : %u latitude : %6.2f longitude : %6.2f velocity : %6.2f heading : %6.3f ]\n",
	m_gps_Valid, m_gps_latitude, m_gps_longitude, m_gps_velocity, m_gps_heading);
}


//
// view: 전조등 컨트롤 시스템
//


int DataContainer::getValue_view_number() { mtx_view_number.lock(); int temp = m_view_number; mtx_view_number.unlock(); return temp; }
void DataContainer::setValue_view_number(int value) { mtx_view_number.lock(); m_view_number = value; mtx_view_number.unlock(); }