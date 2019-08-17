//
// Created by 박규열 on 2019-08-10.
//

#pragma once;

#define OPENCV_YOLO

#ifdef OPENCV_YOLO
#include <opencv2/opencv.hpp>
using namespace cv;
#endif

#include <iostream>
#include <string>
#include <list>
#include <cmath>
#include <map>

using namespace std;


class Object {
private:
    string name;
    float area;
    int danger;
    int recog;
    int count;

public:
    Object(const string &name, float area) {
        this->name = name;
        this->area = area;
        this->count = 1;
        this->recog = 1;
        this->danger = 0;
    }

    void setDanger(int danger) { this->danger = danger; }
    void setArea(float area) { this->area = area; }
    void recognized() { this->recog = 1; }
    void unrecognized() { this->recog = 0; }
    void upcount() {
        if (count >= 10)
            return;
        else {
            this->count = this->count + 1;
        }
    }
    void downcount() {
        if (count <= 0)
            return;
        else
            count--;
    }

    string getName() { return name; }
    float getArea() { return area; }
    int getDanger() { return danger; }
    int getCount() { return count; }
    int getRecog() { return recog; }

};


class MissionContainer {
private:
    std::list<Object*> recognized_Objects;
    std::map<string, int> activated_Objects;
    std::map<string, int> recognized_Missions;

    int kidSafeCount; // 어린이 보호구역 여부. 0: 보호구역 아님, 1: 보호구역

    // 인식된 오브젝트의 크기로부터 위험도(거리)를 4단계로 반환한다(클래스마다 크기 임계값 다름)
    int sizecheck(const string &name, float area);

    // 새로운 오브젝트 객체를 생성해서 반환
    Object* createObject(const string &name, float area);

    // 리스트에 해당 성질을 가진 객체가 있는지 찾는다
    Object* findObject(const string &name, float area);

    // 유효한 미션을 업데이트한다
    void updateMissions();

    int getGPSdirection();

    // 더 큰 수 반환
    int bigger(int a, int b) {
        if (a >= b)
            return a;
        else
            return b;
    }

    int object2num(const string &name);

public:
    MissionContainer() { kidSafeCount = 0; }
    map<string, int> getObjects() { return activated_Objects; } // 인식된 오브젝트 리스트를 반환한다
    map<string, int> getMissions() { return recognized_Missions; } // 인식된 미션 리스트를 반환한다

    // 인식된 박스를 넣어 일정 크기 이상이면 인식해서 리스트에 넣는다.
    void putbox(const string &name, float width, float height);

    // 업데이트
    void update();

#ifdef OPENCV_YOLO
    // 현재 인식된 미션에 대해 missions 폴더에 있는 해당 사진을 띄운다
    void showMissions(int waitTime) {
        int missionSize = recognized_Missions.size();
        Mat image;
        map<string, int>::iterator mapiter;
        string filename;
        string missionName;
        int danger;
        string ls[] = { "intersectionReady", "intersectionStraight", "intersectionLeft", "intersectionRight", "intersectionStop",
                       "crossWalk", "parking", "busLane", "staticObstacle", "dynamicObstacle", "kidSafe", "bust" };

        if (missionSize != 0) {
            for (const auto & l : ls) {
                mapiter = recognized_Missions.find(l);
                if (mapiter != recognized_Missions.end()) {
                    missionName = mapiter->first;
                    danger = mapiter->second;
                    filename = string("missions/") + missionName + ".png";
                    image = imread(filename, IMREAD_COLOR);
                    if (image.empty()) {
                        cout << "Could not open folder(missions) image : " << filename << endl;
                        continue;
                    }
                    namedWindow(missionName, WINDOW_AUTOSIZE);
                    putText(image, string("danger = ") + to_string(danger), Point(10, 40), 2, 1, Scalar(0, 0, 255));
                    imshow(missionName, image);
                    waitKey(waitTime);
                }
                else { destroyWindow(l); }
            }
        }

    }


    // 현재 인식된 오브젝트에 대해 missions 폴더에 있는 해당 사진을 띄운다
    void showObjects(int waitTime) {
        int objectSize = activated_Objects.size();
        Mat image;
        map<string, int>::iterator mapiter;
        string filename;
        string objectName;
        int danger;

        string ls[] = { "straight", "construction", "crossWalk", "bicycle", "bust", "parking", "busArrowDown", "left",
                       "right", "trafficGreen", "trafficLightGreenLeft", "trafficLightRed", "trafficLightRedLeft",
                       "trafficLightRedYellow", "trafficLightYellow", "roadDiamond", "roadArrowStraight",
                       "roadArrowStraightLeft", "roadArrowStraightRight", "roadArrowLeft", "roadArrowRight",
                       "roadArrowLeftRight", "roadArrowThree", "bustReal", "crossWalkReal", "kidSafeSero", "kidSafeExit" };

        if (objectSize != 0) {
            for (const auto &l : ls) {
                mapiter = activated_Objects.find(l);
                if (mapiter != activated_Objects.end()) {
                    objectName = mapiter->first;
                    danger = mapiter->second;
                    filename = string("objects/") + objectName + ".png";
                    image = imread(filename, IMREAD_COLOR);
                    if (image.empty()) {
                        cout << "Could not open folder(objects) image : " << filename << endl;
                        continue;
                    }
                    namedWindow(objectName, WINDOW_AUTOSIZE);
                    putText(image, string("danger = ") + to_string(danger), Point(10, 40), 2, 1.2,
                        Scalar(0, 0, 255));
                    imshow(objectName, image);
                    waitKey(waitTime);
                }
                else { destroyWindow(l); }
            }
        }

    }
#endif

};