#pragma once

// TODO: automode thread �����ϱ�

class Mission
{
private:

protected:
	
    int MissionNumber = 0;

public:
	Mission();
    bool emergencyStop();
    int DecideMission();
    void setMission(int k);// k��° �̼����� �Ѿ
    void doMission0();
    void Connect();
};
