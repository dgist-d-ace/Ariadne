//
// Created by 박규열 on 2019-08-10.
//

#include "missionTrigger.h"
using namespace std;


// 인식된 오브젝트의 크기로부터 위험도(거리)를 4단계로 반환한다(클래스마다 크기 임계값 다름)
int MissionContainer::sizecheck(const string &name, float area) {

    // 빨간색, 노란색 삼각형 표지판 (주의표지)
    if (name == "straight" || name == "construction" || name == "crossWalk" || name == "bicycle" || name == "bust") {
        if (area < 0.0008) return 0;
        else if (area < 0.002) return 1;
        else if (area < 0.0037) return 2;
        else return 3;
    }
    // 파란색 사각형 표지판 (지시표지1)
    else if (name == "parking" || name == "busArrowDown") {
        if (area < 0.0008) return 0;
        else if (area < 0.002) return 1;
        else if (area < 0.0037) return 2;
        else return 3;
    }
    // 파란색 원 표지판 (지시표지2) - 교차로
    if (name == "left" || name == "right") {
        if (area < 0.0008) return 0;
        else if (area < 0.002) return 1;
        else if (area < 0.0037) return 2;
        else return 3;
    }
    // 신호등
    if (name == "trafficGreen" || name == "trafficLightGreenLeft" || name == "trafficLightRed" ||
        name == "trafficLightRedLeft" || name == "trafficLightRedYellow" || name == "trafficLightYellow") {
        if (area < 0.0008) return 0;
        else if (area < 0.0012) return 1;
        else if (area < 0.0027) return 2;
        else return 3;
    }
    // 로드마크 횡단보도 (노면표시)
    if (name == "roadDiamond") {
        if (area < 0.002) return 0;
        else if (area < 0.007) return 1;
        else if (area < 0.01) return 2;
        else return 3;
    }
    // 로드마크 화살표 (노면표시)
    if (name == "roadArrowStraight" || name == "roadArrowStraightLeft" || name == "roadArrowStraightRight" ||
        name == "roadArrowLeft" || name == "roadArrowRight" || name == "roadArrowLeftRight" || name == "roadArrowThree") {
        if (area < 0.002) return 0;
        else if (area < 0.004) return 1;
        else if (area < 0.006) return 2;
        else return 3;
    }
    // 과속방지턱 (실제)
    if (name == "bustReal") {
        if (area < 0.002) return 0;
        else if (area < 0.01) return 1;
        else if (area < 0.02) return 2;
        else return 3;
    }
    // 횡단보도 (실제)
    if (name == "crossWalkReal") {
        if (area < 0.002) return 0;
        else if (area < 0.01) return 1;
        else if (area < 0.02) return 2;
        else return 3;
    }
    // 어린이보호구역 (실제)
    if (name == "kidSafeSero") {
        if (area < 0.001) return 0;
        else if (area < 0.02) return 1;
        else if (area < 0.03) return 2;
        else return 3;
    }

    // 어린이보호구역 (실제)
    if (name == "kidSafeExit") {
        if (area < 0.001) return 0;
        else if (area < 0.02) return 1;
        else if (area < 0.03) return 2;
        else return 3;
    }

    // 클래스 이름이 아무것도 매칭되지 않은 경우
    return -1;
}


// 새로운 오브젝트 객체를 생성해서 반환
Object* MissionContainer::createObject(const string &name, float area) {
    auto newObject = new Object(name, area);
    return newObject;
}


// 리스트에 해당 성질을 가진 객체가 있는지 찾는다
Object* MissionContainer::findObject(const string &name, float area) {

    list<Object*>::iterator iter;
    // 해당 클래스 객체가 있으면, 크기를 비교해 동일 물체이면 해당 포인터 반환 (임계값 정해줘야 함)
    for (iter = recognized_Objects.begin(); iter != recognized_Objects.end(); ++iter) {
        if ((*iter)->getName() == name) {
            if (((*iter)->getArea() - area) < 0.01 && (area - (*iter)->getArea()) < 0.01)
                return *iter;
        }
    }

    // 없으면 널포인터 반환
    return nullptr;
}


// *** 사용하지 않음
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


// GPS로부터 전방 교차로에서의 진행방향을 받아옴 (작성 필요)
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

    // 교차로 방향 표지판이 인식되지 않은 경우
    return -2;
}


void MissionContainer::updateMissions() {

    map<string, int> dic;
    map<string, int> mission_dic;
    list<Object*>::iterator iter;
    map<string, int>::iterator mapiter;

    // recognized_Objects 중 활성화 된 오브젝트만 딕셔너리(맵)에 업데이트
    for (iter = recognized_Objects.begin(); iter != recognized_Objects.end(); ++iter) {
        // 카운트 5 이상인 경우만 맵에 저장
        if ((*iter)->getCount() >= 5) {
            auto obname = ((*iter)->getName());
            auto obdanger = ((*iter)->getDanger());
            // 맵에 저장
            auto pr = dic.insert(pair<string, int>(obname, obdanger));

            // 이미 해당 오브젝트가 맵 내부에 존재하면 위험도 비교해서 업데이트
            if (!pr.second)
                if (pr.first->second < obdanger) pr.first->second = obdanger;
        }
    }

    // 활성화된 오브젝트 바깥으로 넘겨줌 (인식테스트 용도) ******* (이후 삭제필) ********
    this->activated_Objects = dic;
    if (dic.empty()) cout << "인식된 오브젝트 없음" << endl;

    // 해당하는 미션 인식해서 mission_dic에 저장
    if (!dic.empty()) {

        map<string, int>::iterator mapiter2;
        pair<map<string, int>::iterator, bool> pr;
        int intersectionCount = 0; // 교차로 위험도
        int direction = 0; // 교차로 회전방향. 0: 직진, -1: 좌회전, 1: 우회전
        int intersectionLight = 0; // 빨간불인지, 초록불인지 진행여부 판단. 0: 신호등 없음, -2: 빨간불 좌회전, -1: 정지, 1: 진행, 2: 초록불 좌회전

        // 교차로 대기신호 (곧 교차로가 나타남)
        {
            // 신호등
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

            // 로드마크
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

            // GPS로 확인해서 전방이 교차로가 아니면 교차로 대기상태를 지우고 교차로 위험도를 0으로 바꾼다
            direction = getGPSdirection();
            if (direction == -2) {
                mission_dic.erase("intersectionReady");
                intersectionCount = 0;
            }
        }

        // 교차로 진행 트리거
        {
            if (intersectionCount >= 2) {
                //direction = getGPSdirection(); // 교차로가 가까워지면 GPS 좌표로부터 전방 교차로에서의 진행방향을 받아온다
                mission_dic.erase("intersectionReady"); // 교차로 대기상태를 지운다

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

        // 기타 표지판 미션
        {
            // 어린이 보호구역 (kidSafe)
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

            // 과속방지턱 (bust)
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


            // 횡단보도 (crossWalk)
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

            // 주차 (parking)
            mapiter2 = dic.find("parking");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("parking", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
            }

            // 버스전용차로 주의 (busArrowDown)
            mapiter2 = dic.find("busArrowDown");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("busLane", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
            }

            // 정적장애물 (staticObstacle)
            mapiter2 = dic.find("construction");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("staticObstacle", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
            }

            // 동적장애물 (dynamicObstacle)
            mapiter2 = dic.find("bicycle");
            if (mapiter2 != dic.end()) {
                pr = mission_dic.insert(pair<string, int>("dynamicObstacle", mapiter2->second));
                if (!pr.second) if (pr.first->second < mapiter2->second) pr.first->second = mapiter2->second;
                intersectionCount = pr.first->second;
            }
        }

    }
    else {
        cout << "인식된 미션 없음" << endl;
    }

    this->recognized_Missions = mission_dic;

}


// 인식된 박스를 넣어 일정 크기 이상이면 인식해서 리스트에 넣는다.
void MissionContainer::putbox(const string &name, float width, float height) {

    float area = width * height;
    int danger = sizecheck(name, area);


    // 만약 위험도가 1~3이 아닐 경우 박스를 업데이트하지 않음
    if (danger != 1 && danger != 2 && danger != 3)
        return;

    // 이미 해당 오브젝트가 인식되고 있는지 확인
    Object* findedObject = findObject(name, area);

    // 인식되고 있는 오브젝트가 없으면 리스트에 새로 넣는다
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
    }// 있으면 해당 객체 카운트 업데이트

}


// 업데이트
void MissionContainer::update() {
    cout << "update &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << "\n";
    list<Object*>::iterator iter;

    // 해당 프레임에서 인식 안된 물체들 카운트 줄이고, 카운트가 0이 되면 삭제한다
    for (iter = recognized_Objects.begin(); iter != recognized_Objects.end(); ++iter) {
        if ((*iter)->getRecog() == 0) {
            (*iter)->downcount(); // 인식 안된 물체면 카운트 - 1
            cout << "update - Unrecognized ==================================" << " name : " << (*iter)->getName() << " / count : " << (*iter)->getCount() << "\n";
        }
        else {
            (*iter)->unrecognized(); // 인식 된 물체면 recog 초기화
            cout << "update - Recognized ===================================" << " name : " << (*iter)->getName() << " / count : " << (*iter)->getCount() << "\n";
        }
        if ((*iter)->getCount() == 0) {
            cout << "Erased (low count) : " << (*iter)->getName() << "\n";
            delete *iter;
            iter = recognized_Objects.erase(iter); // 카운트가 0이면 삭제..
            iter--;
        }
    }

    // 인식된 물체들로부터 미션 업데이트
    updateMissions();

    cout << "\n\n\n\n\n";
}