#pragma once
#include <mutex>
#include <vector>
#include <QThread>
#include <QMutex>

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

	//lidar 코드 보고 수정 필요
	//m_lidar_obj[100];

	//
	//카메라1: 주행용 (차선 인식) 
	//
	int m_camera1_steer;

	QMutex mtx_camera1_steer;

	//
	//카메라2: 미정 (주차용) 
	//

	//
	//gps:
	//
	int m_missiontrigger = 0;
	int m_gps_geofence = 0;

	QMutex mtx_missiontrigger;
	QMutex mtx_gps_geofence;

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
	// 수정 필요
	/*void setData_lidar_object(object* objL, int num);
	void getData_lidar_object(object* objM);
	void deleteData_lidar_object();*/

	//
	//	카메라1
	//

	int getValue_camera1_steer();
	void setValue_camera1_steer(int value);

	//
	//	GPS
	//

	int get_missiontrigger();
	void set_missiontrigger(int value);

	int get_gps_geofence();
	void set_gps_geofence(int value);

};