/// This cpp and h file are for Lidar Communcation, vector calculation
/// made by Junho Jeong, JunK Cho
/// this LidarFunc.cpp & h are combined with ObejectVector & LastofLidar in LOL.sln


#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include "LidarFunc.h"
#include <iostream>

using namespace std;

LastOfLiDAR::LastOfLiDAR()
{
    m_bConnect = false;
    m_bRunThread = false;
    m_bDataAvailable = false;
    m_nSETContinuous = -1;
    m_nACKContinuous = -1;

    m_status = SICK_STATUS_UNDEFINED;
}

LastOfLiDAR::~LastOfLiDAR()
{
    if (m_status != SICK_STATUS_MEASUREMENT || m_bRunThread)
        UnInitialize();
}

bool LastOfLiDAR::Initialize(std::string host_ip/*="169.254.65.192"*/)
{
    if (!ConnectToDevice(host_ip, 2111)) {
        return false;
    }

    LOG_OUT("Wait for ready status ...");

    while (m_status != SICK_STATUS_MEASUREMENT) {
        m_status = QueryStatus();
    }

    LOG_OUT("Laser is ready...");

    return true;
}

bool LastOfLiDAR::UnInitialize()
{
    if (m_bRunThread) {
        //StopCapture();
    }

    if (m_bConnect)
        DisconnectToDevice();

    return true;
}

bool LastOfLiDAR::StartCapture()
{
    if (m_bRunThread) {
        LOG_OUT("thread is now running..");
        return false;
    }

    if (m_status != SICK_STATUS_MEASUREMENT) {
        LOG_OUT("initialize SICK first..");
        return false;
    }

    m_status = SICK_STATUS_SCANCONTINOUS;

    m_strRemain.clear();
    m_bRunThread = true;
    m_hThread = (HANDLE)_beginthreadex(NULL, 0, &LastOfLiDAR::proc, (void *)this, 0, NULL);

    return ScanContinuous(1);
}

bool LastOfLiDAR::StopCapture()
{
    if (!m_bRunThread) {
        LOG_OUT("thread has already been stopped..");
        return false;
    }

    if (ScanContinuous(0)) {

        m_bRunThread = false;

        long ld = WaitForSingleObject(m_hThread, 1000);
        if (ld == WAIT_OBJECT_0)
            printf("Thread[%s] : 정상 종료\n", "SICK");
        else if (ld == WAIT_TIMEOUT)
            printf("Thread[%s] : 종료 Timeout...\n", "SICK");

        CloseHandle(m_hThread);

        while (m_status != SICK_STATUS_MEASUREMENT) {
            m_status = QueryStatus();
        }
    }

    return true;
}

bool LastOfLiDAR::GetValidDataRTheta(vector<pair<int, double> > &vecRTheta)
{
    if (!m_bRunThread) {
        LOG_OUT("thread is not running..");
        return false;
    }

    m_bDataAvailable = false;
    vecRTheta = m_vecValidScanRTheta;

    return true;
}


/////////////////////////////////////////////////////////////////////////

bool LastOfLiDAR::ConnectToDevice(std::string host, int port)
{
    if (m_bConnect) {
        LOG_OUT("Sick handle has already been opened...");
        return false;
    }

    WSAStartup(MAKEWORD(2, 2), &m_wsadata);				// <------ DLG

    SOCKADDR_IN my_addr;
    my_addr.sin_family = PF_INET;
    my_addr.sin_addr.s_addr = inet_addr(host.c_str());
    my_addr.sin_port = htons(port);

    m_sockDesc = socket(PF_INET, SOCK_STREAM, 0);

    ULONG nonBlock = TRUE;
    ioctlsocket(m_sockDesc, FIONBIO, &nonBlock);

    ::connect(m_sockDesc, (SOCKADDR*)&my_addr, sizeof(my_addr));

    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(m_sockDesc, &fdset);

    timeval timevalue;
    timevalue.tv_sec = 1;
    timevalue.tv_usec = 500000;

    ::select(m_sockDesc + 1, NULL, &fdset, NULL, &timevalue);

    if (!FD_ISSET(m_sockDesc, &fdset)) {
        LOG_OUT("connection error...timeout...");

        closesocket(m_sockDesc);
        WSACleanup();

        return false;
    }

    return m_bConnect = true;
}

bool LastOfLiDAR::DisconnectToDevice()
{
    if (!m_bConnect) {
        LOG_OUT("Sick handle has already been closed...");
        return false;
    }

    closesocket(m_sockDesc);
    WSACleanup();

    m_bConnect = false;

    return true;
}

status_t LastOfLiDAR::QueryStatus() 
{
    char buf[256 + 1];
    sprintf_s(buf, "%c%s%c", 0x02, "sRN STlms", 0x03);

    send(m_sockDesc, buf, strlen(buf), 0);

    fd_set fdset;
    timeval timevalue;

    int len = -1;
    while (len < 0) {
        FD_ZERO(&fdset);
        FD_SET(m_sockDesc, &fdset);

        timevalue.tv_sec = 0;
        timevalue.tv_usec = 10000; // 10ms

        ::select(m_sockDesc + 1, NULL, &fdset, NULL, &timevalue);
        if (!FD_ISSET(m_sockDesc, &fdset)) continue;

        len = recv(m_sockDesc, buf, 256, 0);
    }
    buf[len] = 0;

    int ret;
    sscanf_s((buf + 10), "%d", &ret);

    return (status_t)ret;
}

bool LastOfLiDAR::ScanContinuous(int start)
{
    char buf[100];
    sprintf_s(buf, "%c%s%d%c", 0x02, "sEN LMDscandata ", start, 0x03);

    m_nSETContinuous = start;
    send(m_sockDesc, buf, strlen(buf), 0);
    m_nSETContinuous = m_nACKContinuous = -1;
    LOG_OUT("ScanContinuous OK");

    return true;
}

unsigned int CALLBACK LastOfLiDAR::proc(void* pParam)
{
    LastOfLiDAR* obj = (LastOfLiDAR*)pParam;

    const int kSizeBuf = 1024;
    char buf[kSizeBuf + 1];
    std::string strBuf;

    fd_set fdset;
    timeval timevalue;

    while (obj->m_bRunThread) {
        FD_ZERO(&fdset);
        FD_SET(obj->m_sockDesc, &fdset);

        timevalue.tv_sec = 0;
        timevalue.tv_usec = 10000; // 10ms

        ::select(obj->m_sockDesc + 1, NULL, &fdset, NULL, &timevalue);
        if (!FD_ISSET(obj->m_sockDesc, &fdset)) continue;

        int len = recv(obj->m_sockDesc, buf, kSizeBuf, 0);

        if (len > 0) {
            buf[len] = 0;
            strBuf = buf;
            obj->m_strRemain += strBuf;

            int posSTX = obj->m_strRemain.find(SICK_STX);
            int posETX;
            while (posSTX != -1 && obj->m_bRunThread) {
                obj->m_strRemain = obj->m_strRemain.substr(posSTX);

                posETX = obj->m_strRemain.find(SICK_ETX);
                if (posETX == -1) break;

                std::string strNewBuff = obj->m_strRemain.substr(0, posETX + 1);

                if (strcmp(strNewBuff.substr(1, 3).c_str(), "sSN") == 0) {
                    obj->ParseSickData(strNewBuff);
                }
                else if (strcmp(strNewBuff.substr(1, 3).c_str(), "sEA") == 0) {
                    obj->m_nACKContinuous = strNewBuff[20] - '0';
                }

                obj->m_strRemain = obj->m_strRemain.substr(posETX + 1);
                posSTX = obj->m_strRemain.find(SICK_STX);
            }
        }
    }

    return 0;
}

void LastOfLiDAR::ParseSickData(std::string& strBuf)
{
    vector<double> rawScans;

    char* buf = strdup(strBuf.c_str());

    char* tok = strtok(buf, " "); //Type of command
    tok = strtok(NULL, " "); //Command
    tok = strtok(NULL, " "); //VersionNumber
    tok = strtok(NULL, " "); //DeviceNumber
    tok = strtok(NULL, " "); //Serial number
    tok = strtok(NULL, " "); //DeviceStatus
    tok = strtok(NULL, " "); //MessageCounter
    tok = strtok(NULL, " "); //ScanCounter
    tok = strtok(NULL, " "); //PowerUpDuration
    tok = strtok(NULL, " "); //TransmissionDuration
    tok = strtok(NULL, " "); //InputStatus
    tok = strtok(NULL, " "); //OutputStatus
    tok = strtok(NULL, " "); //ReservedByteA
    tok = strtok(NULL, " "); //ScanningFrequency
    tok = strtok(NULL, " "); //MeasurementFrequency
    tok = strtok(NULL, " ");
    tok = strtok(NULL, " ");
    tok = strtok(NULL, " ");
    tok = strtok(NULL, " "); //NumberEncoders
    int NumberEncoders;
    sscanf(tok, "%d", &NumberEncoders);
    for (int i = 0; i < NumberEncoders; i++) {
        tok = strtok(NULL, " "); //EncoderPosition
        tok = strtok(NULL, " "); //EncoderSpeed
    }

    tok = strtok(NULL, " "); //NumberChannels16Bit
    int NumberChannels16Bit;
    sscanf(tok, "%d", &NumberChannels16Bit);
    for (int i = 0; i < NumberChannels16Bit; i++) {
        int type = -1; // 0 DIST1 1 DIST2 2 RSSI1 3 RSSI2
        char content[6];
        tok = strtok(NULL, " "); //MeasuredDataContent
        sscanf(tok, "%s", content);
        if (!strcmp(content, "DIST1")) {
            type = 0;
        }
        else if (!strcmp(content, "DIST2")) {
            type = 1;
        }
        else if (!strcmp(content, "RSSI1")) {
            type = 2;
        }
        else if (!strcmp(content, "RSSI2")) {
            type = 3;
        }
        tok = strtok(NULL, " "); //ScalingFactor
        tok = strtok(NULL, " "); //ScalingOffset
        tok = strtok(NULL, " "); //Starting angle
        tok = strtok(NULL, " "); //Angular step width
        tok = strtok(NULL, " "); //NumberData
        int NumberData;
        sscanf(tok, "%X", &NumberData);

        if (type == 0) {
            //data.dist_len1 = NumberData;
        }
        else if (type == 1) {
            //data.dist_len2 = NumberData;
        }
        else if (type == 2) {
            //data.rssi_len1 = NumberData;
        }
        else if (type == 3) {
            //data.rssi_len2 = NumberData;
        }

        for (int i = 0; i < NumberData; i++) {
            int dat;
            tok = strtok(NULL, " "); //data
            sscanf(tok, "%X", &dat);

            if (type == 0) {
                rawScans.push_back(dat);
            }
            else if (type == 1) {
                //data.dist2[i] = dat;
            }
            else if (type == 2) {
                //data.rssi1[i] = dat;
            }
            else if (type == 3) {
                //data.rssi2[i] = dat;
            }
        }
    }

    tok = strtok(NULL, " "); //NumberChannels8Bit
    int NumberChannels8Bit;
    sscanf(tok, "%d", &NumberChannels8Bit);
    for (int i = 0; i < NumberChannels8Bit; i++) {
        int type = -1;
        char content[6];
        tok = strtok(NULL, " "); //MeasuredDataContent
        sscanf(tok, "%s", content);
        if (!strcmp(content, "DIST1")) {
            type = 0;
        }
        else if (!strcmp(content, "DIST2")) {
            type = 1;
        }
        else if (!strcmp(content, "RSSI1")) {
            type = 2;
        }
        else if (!strcmp(content, "RSSI2")) {
            type = 3;
        }
        tok = strtok(NULL, " "); //ScalingFactor
        tok = strtok(NULL, " "); //ScalingOffset
        tok = strtok(NULL, " "); //Starting angle
        tok = strtok(NULL, " "); //Angular step width
        tok = strtok(NULL, " "); //NumberData
        int NumberData;
        sscanf(tok, "%X", &NumberData);

        if (type == 0) {
            //data.dist_len1 = NumberData;
        }
        else if (type == 1) {
            //data.dist_len2 = NumberData;
        }
        else if (type == 2) {
            //data.rssi_len1 = NumberData;
        }
        else if (type == 3) {
            //data.rssi_len2 = NumberData;
        }

        for (int i = 0; i < NumberData; i++) {
            int dat;
            tok = strtok(NULL, " "); //data
            sscanf(tok, "%X", &dat);

            if (type == 0) {
                // 				if(dat >= SICK_SCAN_DIST_MIN && dat <= SICK_SCAN_DIST_MAX) {
                // 					pair<int, double> pairTemp;
                // 					pairTemp.first = i;
                // 					pairTemp.second = dat/1000.0; // meter to mm
                // 					m_aScanData.push_back(pairTemp);
                // 				}
            }
            else if (type == 1) {
                //data.dist2[i] = dat;
            }
            else if (type == 2) {
                //data.rssi1[i] = dat;
            }
            else if (type == 3) {
                //data.rssi2[i] = dat;
            }
        }
    }

    int flag;
    tok = strtok(NULL, " "); // Position
    sscanf(tok, "%d", &flag);
    if (flag)
        for (int i = 0; i < 7; i++) tok = strtok(NULL, " ");
    tok = strtok(NULL, " "); // Name
    sscanf(tok, "%d", &flag);
    if (flag) tok = strtok(NULL, " ");
    tok = strtok(NULL, " "); // Comment
    sscanf(tok, "%d", &flag);
    if (flag) tok = strtok(NULL, " ");
    tok = strtok(NULL, " "); // Time
    sscanf(tok, "%d", &flag);
    struct tm lms_time;
    lms_time.tm_isdst = -1;
    if (flag) {
        tok = strtok(NULL, " ");
        sscanf(tok, "%X", &lms_time.tm_year);
        tok = strtok(NULL, " ");
        sscanf(tok, "%X", &lms_time.tm_mon);
        tok = strtok(NULL, " ");
        sscanf(tok, "%X", &lms_time.tm_mday);
        tok = strtok(NULL, " ");
        sscanf(tok, "%X", &lms_time.tm_hour);
        tok = strtok(NULL, " ");
        sscanf(tok, "%X", &lms_time.tm_min);
        tok = strtok(NULL, " ");
        sscanf(tok, "%X", &lms_time.tm_sec);
        tok = strtok(NULL, " ");
        //sscanf(tok, "%X", &data.timestamp.tv_usec);
        lms_time.tm_year -= 1900;
        lms_time.tm_mon--;
        //data.timestamp.tv_sec = mktime(&lms_time);
    }

    m_bDataAvailable = false;
    m_vecRawScans = rawScans;
    ConvertRawToRTheta();
    m_bDataAvailable = true;

   // cout << "lol m_bDataAvailable: " << m_bDataAvailable << endl;



    free(buf);
}

void LastOfLiDAR::ConvertRawToRTheta()
{
    m_vecValidScanRTheta.clear();

    pair<int, double> pairRTheta;

    for (int i = 0; i < (int)m_vecRawScans.size(); ++i) {
        int dat = m_vecRawScans[i];
        if (dat >= SICK_SCAN_DIST_MIN && dat <= SICK_SCAN_DIST_MAX) {
            pairRTheta.first = i;
            pairRTheta.second = dat; // mm
            m_vecValidScanRTheta.push_back(pairRTheta);
        }
    }
}

//(R, THETA) -> (X,Y)
void LastOfLiDAR::Conversion(vector<pair<int, double> > &vecRTheta, queue<vector<cv::Point2d> > &finQVecXY)
{
	vector<pair<int, double> > vecRT = vecRTheta;

	vector<cv::Point2d> vecXY;

	for (int i = 0; i < vecRT.size(); ++i) { //R, Theta to X, Y
		double deg = SICK_SCAN_DEG_START + (double)vecRT[i].first * (SICK_SCAN_DEG_RESOLUTION / 1000.0); //0도부터 180도까지 0.25도 간격

		cv::Point2d xyTemp(0, 0);

		//radian, degree 확인 필수!!!!!!!!!!!!@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		//r, theta -> x, y
		xyTemp.x -= vecRT[i].second * cos(deg / 180 * CV_PI);
		xyTemp.y += vecRT[i].second * sin(deg / 180 * CV_PI);

		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
		//@@@@@@@@@@@@@@@@@@@@@@    R    O    I     @@@@@@@@@@@@@@@@@@@@@@//
		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
		/*
		if (xyTemp.x > SICK_SCAN_ROI_X) {
			xyTemp.x = SICK_SCAN_ROI_X;
		}
		if (xyTemp.x < (-1) * SICK_SCAN_ROI_X) {
			xyTemp.x = (-1) * SICK_SCAN_ROI_X;
		}
		if (xyTemp.y > SICK_SCAN_ROI_Y) {
			xyTemp.y = SICK_SCAN_ROI_Y;
		}
		*/
		vecXY.push_back(xyTemp);
	}

	if (finQVecXY.size() == SICK_SCAN_FRAME_NUM) {
		finQVecXY.pop();
	}

	finQVecXY.push(vecXY);
}

//AVERAGE FILTER
bool LastOfLiDAR::Average(queue<vector<Point2d> > &finQVecXY, vector<Point2d> &finVecXY)
{
	if (finQVecXY.size() < SICK_SCAN_FRAME_NUM) {
		finVecXY = finQVecXY.back();

		return false;
	}

	queue<vector<Point2d> > qVecXY = finQVecXY;

	vector<Point2d> finAverVecXY;


	for (int i = 0; i < qVecXY.back().size(); i++) {
		queue<vector<Point2d> > qVecXYTemp = qVecXY;

		Point2d averVecXY(0, 0);

		for (int j = 0; j < SICK_SCAN_FRAME_NUM; j++) {
			vector<Point2d> vecXY = qVecXYTemp.front();

			averVecXY.x += vecXY[i].x;
			averVecXY.y += vecXY[i].y;

			qVecXYTemp.pop();
		}

		averVecXY.x = averVecXY.x / SICK_SCAN_FRAME_NUM;
		averVecXY.y = averVecXY.y / SICK_SCAN_FRAME_NUM;

		if (sqrt(pow(averVecXY.x - qVecXY.back()[i].x, 2) + pow(averVecXY.y - qVecXY.back()[i].y, 2)) < SICK_SCAN_DIST_AVERAGE) {
			finAverVecXY.push_back(averVecXY);
		}
		else {
			finAverVecXY.push_back(qVecXY.back()[i]);
		}
	}

	finVecXY = finAverVecXY;

	return true;
}

//DATA CLUSTERING
void LastOfLiDAR::Clustering(vector<Point2d> &finVecXY, queue<vector<vector<double> > > &finObjData)
{
	vector<Point2d> vecXY = finVecXY;

	vector<Point2d> objPoint;
	vector<vector<Point2d> > objPointSet;

	//두 점 사이의 거리 및 점 그룹화
	for (int i = 0; i < vecXY.size() - 1; ++i) {
		double dist = sqrt(pow(vecXY[i].x - vecXY[i + 1].x, 2) + pow(vecXY[i].y - vecXY[i + 1].y, 2));

		objPoint.push_back(vecXY[i]);

		if (dist >= SICK_SCAN_DIST_OBJECT) {
			objPointSet.push_back(objPoint);
			objPoint.clear();
		}
	}

	objPointSet.push_back(objPoint);

	vector<vector<double> > objDataSet;

	//각 그룹 물체화
	for (int i = 0; i < objPointSet.size(); i++) {
		vector<Point2d> objPoint = objPointSet[i];

		if (objPoint.size() <= 4) {
			continue;
		}

		vector<double> objData;	//[center_x, center_y, radius]

		Point2d cirCen(0, 0);

		for (int j = 0; j < objPoint.size(); j++) {
			cirCen.x += objPoint[j].x;
			cirCen.y += objPoint[j].y;
		}

		cirCen.x = cirCen.x / objPoint.size();
		cirCen.y = cirCen.y / objPoint.size();

		double radius = 0;

		for (int j = 0; j < objPoint.size(); j++) {
			radius += sqrt(pow(cirCen.x - objPoint[j].x, 2) + pow(cirCen.y - objPoint[j].y, 2));
		}

		radius = radius / objPoint.size() * SICK_SCAN_OBJ_OFFSET;

		double slop = sqrt(pow(cirCen.x, 2) + pow(cirCen.y, 2));

		cirCen.x = cirCen.x + radius * cirCen.x / slop * 0.5;
		cirCen.y = cirCen.y + radius * cirCen.y / slop * 0.5;

		if (cirCen.x < SICK_SCAN_ROI_X && cirCen.x >(-1) * SICK_SCAN_ROI_X && cirCen.y < SICK_SCAN_ROI_Y) {
			if (radius <= SICK_SCAN_DIST_CIRCLE_MAX && radius >= SICK_SCAN_DIST_CIRCLE_MIN) { //반지름 문턱값 이하만 추가
				objData.push_back(cirCen.x);
				objData.push_back(cirCen.y);
				objData.push_back(radius);
				//cout << "Center X of Circle: " << cirCen.x;
				//cout << ", Center Y of Circle: " << cirCen.y;
				//cout << ", Radius of circle: " << radius << endl;
				objDataSet.push_back(objData);
			}
		}
	}

	if (finObjData.size() == 2) { //Queue 꽉 차면 pop
		finObjData.pop();
	}

	finObjData.push(objDataSet);
}

//JUDGEMENT OF "STATIC OR MOVING"
bool LastOfLiDAR::Vector(queue<vector<vector<double> > > &finObjData, vector<Point2d> &finVecData, vector<bool> &finBoolData) //플렛폼 속도, 조향각 참조 추가
{
	if (finObjData.size() < 2) {
		return false;
	}

	queue<vector<vector<double> > > LiDARData = finObjData;

	vector<Point2d> vecData;
	vector<bool> boolData;

	for (int i = 0; i < LiDARData.back().size(); i++) {
		double minVecDistX = 1000;
		double minVecDistY = 1000;
		double minVecDist = 1000;

		for (int j = 0; j < LiDARData.front().size(); j++) {
			double vecDistX = LiDARData.back()[i][0] - LiDARData.front()[j][0];
			double vecDistY = LiDARData.back()[i][1] - LiDARData.front()[j][1]; //(objDistX, objDistY) 단위 mm
			double vecDist = sqrt(pow(vecDistX, 2) + pow(vecDistY, 2)); //한 물체가 1프레임동안 이동한 거리
			//double vecSpeed = objDist / 0.02 / 0.01; // m/s 헿 이거도 필요없네

			if (minVecDist > vecDist) { //거리 최솟값 설정
				minVecDistX = vecDistX;
				minVecDistY = vecDistY;
				minVecDist = vecDist;
			}
		}

		if (minVecDist < SICK_SCAN_VEC_SAME_OBJECT) { //i와 j는 같은 오브젝트이다 이마리야
			Point2d minVecDistXY(minVecDistX, minVecDistY);

			vecData.push_back(minVecDistXY);

			if (abs(minVecDistX) > SICK_SCAN_VEC_IS_MOVED && abs(minVecDistY) > SICK_SCAN_VEC_IS_MOVED) { // 이면 정적이다 2마리야
				boolData.push_back(true);
			}
			else {
				boolData.push_back(false);
			}
		}
	}

	finVecData = vecData;
	finBoolData = boolData;

	return true;
}

/*
< How to set up OpenCV in VS studio >
0. this project used OpenCV 3.4.5.20
1. build openCV that you needed by using cmake
2. setup OpenCV Environment Path
3. Project in VS top bar -> <Project name> Property -> linker -> general -> additional
library directory -> opencv dll setting
ex) C:\Users\DY\Desktop\opencv-3.4.5\opencv-3.4.5\build\x64\vc15\lib
4. 속성 페이지의 상단에 있는 구성을 release와 debug 각각 세팅해준다
링커 -> 일반
링커 -> 입력 모두 적절한 dll을 집어넣는다.
즉 각각 4번의 dll 세팅을 하는 것 (release, debug모드 * 링커 일반, 링커 입력 = 4가지 케이스)
5. 속성 페이지 C/C++ 일반 -> 추가 포함 디렉터리에도 openCV 버전을 설정해준다.
*/