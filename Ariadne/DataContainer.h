#pragma once

#include <mutex>
#include <QThread>
#include <QMutex>
#include "LidarFunc.h"

class DataContainer {
private:

	DataContainer() {};

	static bool instanceFlag;

	static DataContainer* instance;

	//
	// UPPER to PCU (플랫폼에 보내는 값)
	//
	int m_UtoP_AorM = 0;
	int m_UtoP_E_STOP = 0;
	int m_UtoP_GEAR = 0;
	int m_UtoP_SPEED = 0;
	int m_UtoP_STEER = 0;
	int m_UtoP_BRAKE = 0;
	int m_UtoP_ALIVE = 0;

	//
	// PCU to UPPER (플랫폼에서 받아오는 값)
	//
	int m_PtoU_AorM = 0;
	int m_PtoU_E_STOP = 0;
	int m_PtoU_GEAR = 0;
	int m_PtoU_SPEED = 0;
	int m_PtoU_STEER = 0;
	int m_PtoU_BRAKE = 0;
	long m_PtoU_ENC = 0;
	int m_PtoU_ALIVE = 0;


	QMutex mtx_UtoP_AorM;
	QMutex mtx_UtoP_E_STOP;
	QMutex mtx_UtoP_GEAR;
	QMutex mtx_UtoP_SPEED;
	QMutex mtx_UtoP_STEER;
	QMutex mtx_UtoP_BRAKE;
	QMutex mtx_UtoP_ALIVE;

	QMutex mtx_PtoU_AorM;
	QMutex mtx_PtoU_E_STOP;
	QMutex mtx_PtoU_GEAR;
	QMutex mtx_PtoU_SPEED;
	QMutex mtx_PtoU_STEER;
	QMutex mtx_PtoU_BRAKE;
	QMutex mtx_PtoU_ENC;
	QMutex mtx_PtoU_ALIVE;

	//
	// 센서 및 플랫폼 연결 상태 정보
	//

	int m_platform_status = 0;
	int m_lidar_status = 0;
	int m_camera1_status = 0;
	int m_gps_status = 0;

	QMutex mtx_platform_status;
	QMutex mtx_lidar_status;
	QMutex mtx_camera1_status;
	QMutex mtx_camera2_status;
	QMutex mtx_gps_status;
	
	//
	//라이다
	//
	vector<cv::Point2d> m_lidar_VecXY; //최종 좌표 집합 - 1프레임
	queue<vector<vector<double> > > m_lidar_Data; //최종 데이터 집합 - 2프레임
	vector<cv::Point2d> m_lidar_VecData; //물체의 벡터 집합 - 1프레임
	vector<bool> m_lidar_BoolData; //물체의 동정적 여부 집합 - 1프레임

	//
	//카메라1: 주행용 (차선 인식) 
	//
	vector<vector<cv::Point2i>> m_camera1_lanes;
	vector<int> m_camara1_existLanes;

	QMutex mtx_camera1_lanes;
	QMutex mtx_camera1_existLanes;

	//
	//카메라2: 미정 (주차용) 
	//

	//
	//gps:
	//
	double m_gps_roll = 0;
	double m_gps_pitch = 0;
	double m_gps_yaw = 0;
	double m_gps_accelX = 0;
	double m_gps_accelY = 0;
	double m_gps_accelZ = 0;

	int m_gps_Valid = 0;
	double m_gps_latitude = 0;
	double m_gps_longitude = 0;
	double m_gps_velocity = 0;
	double m_gps_heading = 0;

	double m_gps_flted_x = 0;
	double m_gps_flted_y = 0;
	double m_gps_flted_Vx = 0;
	double m_gps_flted_Vy = 0;

	QMutex mtx_imu_roll;
	QMutex mtx_imu_pitch;
	QMutex mtx_imu_yaw;
	QMutex mtx_imu_accelX;
	QMutex mtx_imu_accelY;
	QMutex mtx_imu_accelZ;


	
	QMutex mtx_gps_valid;
	QMutex mtx_gps_latitude;
	QMutex mtx_gps_longitude;
	QMutex mtx_gps_velocity;
	QMutex mtx_gps_heading;

	QMutex mtx_gps_flted_x;
	QMutex mtx_gps_flted_y;
	QMutex mtx_gps_flted_vx;
	QMutex mtx_gps_flted_vy;


public:

	//인스턴스 반환
	static DataContainer* getInstance();
	// BK: static Ariadne* getWindow();
	// BK: void setWindow(Ariadne* w);

	//소멸자
	virtual ~DataContainer() {
		instanceFlag = false;
	}
	
	//
	//센서 및 플랫폼 연결 상태
	//

	int getValue_platform_status();
	void setValue_platform_status(int value);
	void updateValue_platform_status();

	int getValue_lidar_status();
	void setValue_lidar_status(int value);
	void updateValue_lidar_status();

	int getValue_camera1_status();
	void setValue_camera1_status(int value);
	void updateValue_camera1_status();

	int getValue_gps_status();
	void setValue_gps_status(int value);
	void updateValue_gps_status();

	//
	// 플랫폼 데이터
	//

	// UPPER to PCU (플랫폼에 보내는 값)
	int getValue_UtoP_AorM();
	int getValue_UtoP_E_STOP();
	int getValue_UtoP_GEAR();
	int getValue_UtoP_SPEED();
	int getValue_UtoP_STEER();
	int getValue_UtoP_BRAKE();
	int getValue_UtoP_ALIVE();

	void setValue_UtoP_AorM(int value);
	void setValue_UtoP_E_STOP(int value);
	void setValue_UtoP_GEAR(int value);
	void setValue_UtoP_SPEED(int value);
	void setValue_UtoP_STEER(int value);
	void setValue_UtoP_BRAKE(int value);
	void setValue_UtoP_ALIVE(int value);

	// 값을 통째로 가져오고, 지정하는 함수. 순서대로 (AorM, E_STOP, GEAR, SPEED, STEER, BRAKE)
	std::vector<int> getValue_UtoP_list();
	void setValue_UtoP_list(int AorM, int E_STOP, int GEAR, int SPEED, int STEER, int BRAKE);

	// PCU to UPPER (플랫폼에서 받아오는 값)
	int getValue_PtoU_AorM();
	int getValue_PtoU_E_STOP();
	int getValue_PtoU_GEAR();
	int getValue_PtoU_SPEED();
	int getValue_PtoU_STEER();
	int getValue_PtoU_BRAKE();
	long getValue_PtoU_ENC();
	int getValue_PtoU_ALIVE();

	void setValue_PtoU_AorM(int value);
	void setValue_PtoU_E_STOP(int value);
	void setValue_PtoU_GEAR(int value);
	void setValue_PtoU_SPEED(int value);
	void setValue_PtoU_STEER(int value);
	void setValue_PtoU_BRAKE(int value);
	void setValue_PtoU_ENC(long value);
	void setValue_PtoU_ALIVE(int value);

	// 값을 통째로 가져오고, 지정하는 함수. 순서대로 (AorM, E_STOP, GEAR, SPEED, STEER, BRAKE, ENC)
	std::vector<int> getValue_PtoU_list();
	void setValue_PtoU_list(int AorM, int E_STOP, int GEAR, int SPEED, int STEER, int BRAKE, long ENC);

	// UPPER to PCU or PCU to UPPER 값 보여주기
	void show_UtoP();
	void show_PtoU();

	//
	//	라이다
	//

	vector<cv::Point2d> getValue_lidar_VecXY(); //최종 좌표 집합 - 1프레임
	queue<vector<vector<double> > > getValue_lidar_Data(); //최종 데이터 집합 - 2프레임
	vector<cv::Point2d> getValue_lidar_VecData(); //물체의 벡터 집합 - 1프레임
	vector<bool> getValue_lidar_BoolData(); //물체의 동정적 여부 집합 - 1프레임

	void setValue_lidar_VecXY(vector<cv::Point2d> VecXY); //최종 좌표 집합 - 1프레임
	void setValue_lidar_Data(queue<vector<vector<double> > > data); //최종 데이터 집합 - 2프레임
	void setValue_lidar_VecData(vector<cv::Point2d> vecData); //물체의 벡터 집합 - 1프레임
	void setValue_lidar_BoolData(vector<bool> boolData); //물체의 동정적 여부 집합 - 1프레임

	// 수정 필요
	/*void setData_lidar_object(object* objL, int num);
	void getData_lidar_object(object* objM);
	void deleteData_lidar_object();*/

	//
	//	카메라1
	//

	vector<vector<cv::Point2i>> getValue_camera1_lanes();
	vector<int> getValue_camera1_existLanes();
	void setValue_camera1_lanes(vector<vector<cv::Point2i>> lanes);
	void setValue_camera1_existLanes(vector<int> existLanes);
	

	//
	//	GPS
	//
	double getValue_imu_roll();
	double getValue_imu_pitch();
	double getValue_imu_yaw();
	double getValue_imu_accelX();
	double getValue_imu_accelY();
	double getValue_imu_accelZ();

	int getValue_gps_valid();
	double getValue_gps_latitude();
	double getValue_gps_longitude();
	double getValue_gps_velocity();
	double getValue_gps_heading();

	double getValue_flted_x();
	double getValue_flted_y();
	double getValue_flted_Vx();
	double getValue_flted_Vy();

	void setValue_imu_roll(double value);
	void setValue_imu_pitch(double value);
	void setValue_imu_yaw(double value);
	void setValue_imu_accelX(double value);
	void setValue_imu_accelY(double value);
	void setValue_imu_accelZ(double value);

	void resetValue_gps_valid();
	void count_gps_valid();
	void setValue_gps_latitude(double value);
	void setValue_gps_longitude(double value);
	void setValue_gps_velocity(double value);
	void setValue_gps_heading(double value);

	void setValue_flted_x(double value);
	void setValue_flted_y(double value);
	void setValue_flted_Vx(double value);
	void setValue_flted_Vy(double value);

	void show_imu();
	void show_gps();
};