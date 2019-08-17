//
// Created by �ڱԿ� on 2019-08-10.
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

    int kidSafeCount; // ��� ��ȣ���� ����. 0: ��ȣ���� �ƴ�, 1: ��ȣ����

    // �νĵ� ������Ʈ�� ũ��κ��� ���赵(�Ÿ�)�� 4�ܰ�� ��ȯ�Ѵ�(Ŭ�������� ũ�� �Ӱ谪 �ٸ�)
    int sizecheck(const string &name, float area);

    // ���ο� ������Ʈ ��ü�� �����ؼ� ��ȯ
    Object* createObject(const string &name, float area);

    // ����Ʈ�� �ش� ������ ���� ��ü�� �ִ��� ã�´�
    Object* findObject(const string &name, float area);

    // ��ȿ�� �̼��� ������Ʈ�Ѵ�
    void updateMissions();

    int getGPSdirection();

    // �� ū �� ��ȯ
    int bigger(int a, int b) {
        if (a >= b)
            return a;
        else
            return b;
    }

    int object2num(const string &name);

public:
    MissionContainer() { kidSafeCount = 0; }
    map<string, int> getObjects() { return activated_Objects; } // �νĵ� ������Ʈ ����Ʈ�� ��ȯ�Ѵ�
    map<string, int> getMissions() { return recognized_Missions; } // �νĵ� �̼� ����Ʈ�� ��ȯ�Ѵ�

    // �νĵ� �ڽ��� �־� ���� ũ�� �̻��̸� �ν��ؼ� ����Ʈ�� �ִ´�.
    void putbox(const string &name, float width, float height);

    // ������Ʈ
    void update();

#ifdef OPENCV_YOLO
    // ���� �νĵ� �̼ǿ� ���� missions ������ �ִ� �ش� ������ ����
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


    // ���� �νĵ� ������Ʈ�� ���� missions ������ �ִ� �ش� ������ ����
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