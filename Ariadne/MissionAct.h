
#include "DataContainer.h"

// TODO: automode thread �����ϱ�

class Mission
{
private:

protected:
	
    int MissionNumber = 0;

public:
    DataContainer* dataContainer;
	Mission();
    bool emergencyStop();
    int DecideMission();
    void setMission(int k);// k��° �̼����� �Ѿ
    void doMission0();
 
};
