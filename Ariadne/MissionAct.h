#pragma once

// TODO: automode thread 생성하기

class Mission
{
private:

protected:
	
    int MissionNumber = 0;

public:
	Mission();
    bool emergencyStop();
    int DecideMission();
    void setMission(int k);// k번째 미션으로 넘어감
    void doMission0();
    void Connect();
};
