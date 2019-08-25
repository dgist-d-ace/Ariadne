//
// Created by �ڱԿ� on 2019-08-10.
//

#include "missionTrigger.h"
using namespace std;


// �νĵ� ������Ʈ�� ũ��κ��� ���赵(�Ÿ�)�� 4�ܰ�� ��ȯ�Ѵ�(Ŭ�������� ũ�� �Ӱ谪 �ٸ�)
int MissionContainer::sizecheck(const string &name, float area) {

    // ������, ����� �ﰢ�� ǥ���� (����ǥ��)
    if (name == "straight" || name == "construction" || name == "crossWalk" || name == "bicycle" || name == "bust") {
        if (area < 0.0008) return 0;
        else if (area < 0.002) return 1;
        else if (area < 0.0037) return 2;
        else return 3;
    }
    // �Ķ��� �簢�� ǥ���� (����ǥ��1)
    else if (name == "parking" || name == "busArrowDown") {
        if (area < 0.0008) return 0;
        else if (area < 0.002) return 1;
        else if (area < 0.0037) return 2;
        else return 3;
    }
    // �Ķ��� �� ǥ���� (����ǥ��2) - ������
    if (name == "left" || name == "right") {
        if (area < 0.0008) return 0;
        else if (area < 0.002) return 1;
        else if (area < 0.0037) return 2;
        else return 3;
    }
    // ��ȣ��
    if (name == "trafficGreen" || name == "trafficLightGreenLeft" || name == "trafficLightRed" ||
        name == "trafficLightRedLeft" || name == "trafficLightRedYellow" || name == "trafficLightYellow") {
        if (area < 0.0008) return 0;
        else if (area < 0.0012) return 1;
        else if (area < 0.0027) return 2;
        else return 3;
    }
    // �ε帶ũ Ⱦ�ܺ��� (���ǥ��)
    if (name == "roadDiamond") {
        if (area < 0.002) return 0;
        else if (area < 0.007) return 1;
        else if (area < 0.01) return 2;
        else return 3;
    }
    // �ε帶ũ ȭ��ǥ (���ǥ��)
    if (name == "roadArrowStraight" || name == "roadArrowStraightLeft" || name == "roadArrowStraightRight" ||
        name == "roadArrowLeft" || name == "roadArrowRight" || name == "roadArrowLeftRight" || name == "roadArrowThree") {
        if (area < 0.002) return 0;
        else if (area < 0.004) return 1;
        else if (area < 0.006) return 2;
        else return 3;
    }
    // ���ӹ����� (����)
    if (name == "bustReal") {
        if (area < 0.002) return 0;
        else if (area < 0.01) return 1;
        else if (area < 0.02) return 2;
        else return 3;
    }
    // Ⱦ�ܺ��� (����)
    if (name == "crossWalkReal") {
        if (area < 0.002) return 0;
        else if (area < 0.01) return 1;
        else if (area < 0.02) return 2;
        else return 3;
    }
    // ��̺�ȣ���� (����)
    if (name == "kidSafeSero") {
        if (area < 0.001) return 0;
        else if (area < 0.02) return 1;
        else if (area < 0.03) return 2;
        else return 3;
    }

    // ��̺�ȣ���� (����)
    if (name == "kidSafeExit") {
        if (area < 0.001) return 0;
        else if (area < 0.02) return 1;
        else if (area < 0.03) return 2;
        else return 3;
    }

    // Ŭ���� �̸��� �ƹ��͵� ��Ī���� ���� ���
    return -1;
}


// ���ο� ������Ʈ ��ü�� �����ؼ� ��ȯ
Object* MissionContainer::createObject(const string &name, float area) {
    auto newObject = new Object(name, area);
    return newObject;
}


// ����Ʈ�� �ش� ������ ���� ��ü�� �ִ��� ã�´�
Object* MissionContainer::findObject(const string &name, float area) {

    list<Object*>::iterator iter;
    // �ش� Ŭ���� ��ü�� ������, ũ�⸦ ���� ���� ��ü�̸� �ش� ������ ��ȯ (�Ӱ谪 ������� ��)
    for (iter = recognized_Objects.begin(); iter != recognized_Objects.end(); ++iter) {
        if ((*iter)->getName() == name) {
            if (((*iter)->getArea() - area) < 0.01 && (area - (*iter)->getArea()) < 0.01)
                return *iter;
        }
    }

    // ������ �������� ��ȯ
    return nullptr;
}


// *** ������� ����
int MissionContainer::object2num(const string &name) {

    if (name == "straight") return 1;
    else if (name == "construction") return 2;
    else if (name == "crossWalk") return 3;
    else if (name == "bicycle") return 4;
    else if (name == "bust") return 5;
    else if (name == "parking") return 6;
    else if (name == "busArrowDown") return 7;
    else if (name == "left") return 8;
    else if (name == "right") return 9;
    else if (name == "trafficGreen") return 10;
    else if (name == "trafficLightGreenLeft") return 11;
    else if (name == "trafficLightRed") return 12;
    else if (name == "trafficLightRedLeft") return 13;
    else if (name == "trafficLightRedYellow") return 14;
    else if (name == "trafficLightYellow") return 15;
    else if (name == "roadDiamond") return 16;
    else if (name == "roadArrowStraight") return 17;
    else if (name == "roadArrowStraightLeft") return 18;
    else if (name == "roadArrowStraightRight") return 19;
    else if (name == "roadArrowLeft") return 20;
    else if (name == "roadArrowRight") return 21;
    else if (name == "roadArrowLeftRight") return 22;
    else if (name == "roadArrowThree") return 23;
    else if (name == "bustReal") return 24;
    else if (name == "crossWalkReal") return 25;
    else if (name == "kidSafeSero") return 26;
    else if (name == "kidSafeExit") return 27;

}


// GPS�κ��� ���� �����ο����� ��������� �޾ƿ� (�ۼ� �ʿ�)
int MissionContainer::getGPSdirection() {

    map<string, int>::iterator mapiter;

    mapiter = activated_Objects.find("left");
    if (mapiter != activated_Objects.end())
        return -1;
    mapiter = activated_Objects.find("parking");
    if (mapiter != activated_Objects.end())
        return 0;
    mapiter = activated_Objects.find("right");
    if (mapiter != activated_Objects.end())
        return 1;

    // ������ ���� ǥ������ �νĵ��� ���� ���
    return -2;
}


void MissionContainer::updateMissions() {

    map<string, int> dic;
    map<string, int> mission_dic;
    list<Object*>::iterator iter;
    map<string, int>::iterator mapiter;

    // recognized_Objects �� Ȱ��ȭ �� ������Ʈ�� ��ųʸ�(��)�� ������Ʈ
    for (iter = recognized_Objects.begin(); iter != recognized_Objects.end(); ++iter) {
        // ī��Ʈ 5 �̻��� ��츸 �ʿ� ����
        if ((*iter)->getCount() >= 5) {
            auto obname = ((*iter)->getName());
            auto obdanger = ((*iter)->getDanger());
            // �ʿ� ����
            auto pr = dic.insert(pair<string, int>(obname, obdanger));

            // �̹� �ش� ������Ʈ�� �� ���ο� �����ϸ� ���赵 ���ؼ� ������Ʈ
            if (!pr.second)
                if (pr.first->second < obdanger) pr.first->second = obdanger;
        }
    }

    // Ȱ��ȭ�� ������Ʈ �ٱ����� �Ѱ��� (�ν��׽�Ʈ �뵵) ******* (���� ������) ********
    this->activated_Objects = dic;
    if (dic.empty()) cout << "�νĵ� ������Ʈ ����" << endl;

    // �ش��ϴ� �̼� �ν��ؼ� mission_dic�� ����
    if (!dic.empty()) {

        map<string, int>::iterator mapiter2;
        pair<map<string, int>::iterator, bool> pr;
        int intersectionCount = 0; // ������ ���赵
        int direction = 0; // ������ ȸ������. 0: ����, -1: ��ȸ��, 1: ��ȸ��
        int intersectionLight = 0; // ����������, �ʷϺ����� ���࿩�� �Ǵ�. 0: ��ȣ�� ����, -2: ������ ��ȸ��, -1: ����, 1: ����, 2: �ʷϺ� ��ȸ��

        // ������ ����ȣ (�� �����ΰ� ��Ÿ��)
        {
            // ��ȣ��
            mapiter2 = dic.find("trafficGreen");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("intersectionReady", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
                intersectionLight = 1;
            }
            mapiter2 = dic.find("trafficLightGreenLeft");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("intersectionReady", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
                intersectionLight = 2;
            }
            mapiter2 = dic.find("trafficLightRed");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("intersectionReady", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
                intersectionLight = -1;
            }
            mapiter2 = dic.find("trafficLightRedLeft");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("intersectionReady", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
                intersectionLight = -2;
            }
            mapiter2 = dic.find("trafficLightRedYellow");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("intersectionReady", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
                intersectionLight = -1;
            }
            mapiter2 = dic.find("trafficLightYellow");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("intersectionReady", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
                intersectionLight = -1;
            }

            // �ε帶ũ
            mapiter2 = dic.find("roadArrowStraight");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("intersectionReady", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
            }
            mapiter2 = dic.find("roadArrowStraightLeft");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("intersectionReady", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
            }
            mapiter2 = dic.find("roadArrowStraightRight");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("intersectionReady", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
            }
            mapiter2 = dic.find("roadArrowLeft");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("intersectionReady", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
            }
            mapiter2 = dic.find("roadArrowRight");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("intersectionReady", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
            }
            mapiter2 = dic.find("roadArrowLeftRight");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("intersectionReady", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
            }
            mapiter2 = dic.find("roadArrowThree");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("intersectionReady", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
            }

            // GPS�� Ȯ���ؼ� ������ �����ΰ� �ƴϸ� ������ �����¸� ����� ������ ���赵�� 0���� �ٲ۴�
            direction = getGPSdirection();
            if (direction == -2) {
                mission_dic.erase("intersectionReady");
                intersectionCount = 0;
            }
        }

        // ������ ���� Ʈ����
        {
            if (intersectionCount >= 2) {
                //direction = getGPSdirection(); // �����ΰ� ��������� GPS ��ǥ�κ��� ���� �����ο����� ��������� �޾ƿ´�
                mission_dic.erase("intersectionReady"); // ������ �����¸� �����

                if ((direction == 0) && (intersectionLight >= 0))
                    mission_dic.insert(pair<string, int>("intersectionStraight", intersectionCount));
                else if ((direction == -1) && (intersectionLight == 0 || intersectionLight == 2 || intersectionLight == -2))
                    mission_dic.insert(pair<string, int>("intersectionLeft", intersectionCount));
                else if ((direction == 1) && (intersectionLight >= 0))
                    mission_dic.insert(pair<string, int>("intersectionRight", intersectionCount));
                else if (intersectionLight < 0)
                    mission_dic.insert(pair<string, int>("intersectionStop", intersectionCount));

            }
        }

        // ��Ÿ ǥ���� �̼�
        {
            // ��� ��ȣ���� (kidSafe)
            int kidSafe_max = 0;

            mapiter2 = dic.find("kidSafeSero");
            if (mapiter2 != dic.end()) { this->kidSafeCount = 1; kidSafe_max = bigger(kidSafe_max, mapiter2->second); }
            mapiter2 = dic.find("kidSafeExit");
            if ((mapiter2 != dic.end()) && (mapiter2->second >= 2)) { this->kidSafeCount = 0; }

            if (this->kidSafeCount == 1) {
                if (kidSafe_max == 0)
                    mission_dic.insert(pair<string, int>("kidSafe", 3));
                else
                    mission_dic.insert(pair<string, int>("kidSafe", kidSafe_max));
            }

            // ���ӹ����� (bust)
            mapiter2 = dic.find("bust");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("bust", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
            }
            mapiter2 = dic.find("bustReal");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("bust", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
            }


            // Ⱦ�ܺ��� (crossWalk)
            mapiter2 = dic.find("crossWalk");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("crossWalk", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
            }
            mapiter2 = dic.find("crossWalkReal");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("crossWalk", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
            }

            // ���� (parking)
            mapiter2 = dic.find("parking");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("parking", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
            }

            // ������������ ���� (busArrowDown)
            mapiter2 = dic.find("busArrowDown");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("busLane", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
            }

            // ������ֹ� (staticObstacle)
            mapiter2 = dic.find("construction");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("staticObstacle", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
            }

            // ������ֹ� (dynamicObstacle)
            mapiter2 = dic.find("bicycle");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("dynamicObstacle", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
            }
        }

    }
    else {
        cout << "�νĵ� �̼� ����" << endl;
    }

    this->recognized_Missions = mission_dic;

}


// �νĵ� �ڽ��� �־� ���� ũ�� �̻��̸� �ν��ؼ� ����Ʈ�� �ִ´�.
void MissionContainer::putbox(const string &name, float width, float height) {

    float area = width * height;
    int danger = sizecheck(name, area);


    // ���� ���赵�� 1~3�� �ƴ� ��� �ڽ��� ������Ʈ���� ����
    if (danger != 1 && danger != 2 && danger != 3)
        return;

    // �̹� �ش� ������Ʈ�� �νĵǰ� �ִ��� Ȯ��
    Object* findedObject = findObject(name, area);

    // �νĵǰ� �ִ� ������Ʈ�� ������ ����Ʈ�� ���� �ִ´�
    if (findedObject == nullptr) {
        Object* newObject = createObject(name, area);
        newObject->setDanger(danger);
        recognized_Objects.push_back(newObject);
    }
    else {
        findedObject->recognized();
        findedObject->upcount();
        findedObject->setDanger(danger);
        findedObject->setArea(area);
    }// ������ �ش� ��ü ī��Ʈ ������Ʈ

}


// ������Ʈ
void MissionContainer::update() {
    cout << "update &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << "\n";
    list<Object*>::iterator iter;

    // �ش� �����ӿ��� �ν� �ȵ� ��ü�� ī��Ʈ ���̰�, ī��Ʈ�� 0�� �Ǹ� �����Ѵ�
    for (iter = recognized_Objects.begin(); iter != recognized_Objects.end(); ++iter) {
        if ((*iter)->getRecog() == 0) {
            (*iter)->downcount(); // �ν� �ȵ� ��ü�� ī��Ʈ - 1
            cout << "update - Unrecognized ==================================" << " name : " << (*iter)->getName() << " / count : " << (*iter)->getCount() << "\n";
        }
        else {
            (*iter)->unrecognized(); // �ν� �� ��ü�� recog �ʱ�ȭ
            cout << "update - Recognized ===================================" << " name : " << (*iter)->getName() << " / count : " << (*iter)->getCount() << "\n";
        }
        if ((*iter)->getCount() == 0) {
            cout << "Erased (low count) : " << (*iter)->getName() << "\n";
            delete *iter;
            iter = recognized_Objects.erase(iter); // ī��Ʈ�� 0�̸� ����..
            iter--;
        }
    }

    // �νĵ� ��ü��κ��� �̼� ������Ʈ
    updateMissions();

    cout << "\n\n\n\n\n";
}