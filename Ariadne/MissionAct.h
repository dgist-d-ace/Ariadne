#pragma once
#include "DataContainer.h"

// TODO: automode thread �����ϱ�

class Mission
{
private:
	DataContainer* dataContainer;

protected:
	
    int MissionNumber = 0;

public:
	Mission();
    bool emergencyStop();
    int DecideMission();
    void setMission(int k);// k��° �̼����� �Ѿ
    void doMission0();
 
};
