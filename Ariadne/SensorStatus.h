#pragma once

#include "ComPlatform.h"
#include "atlstr.h"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QString>
#include <QTimer>
#include <iostream>
#include <iterator>
#include <QtCore>
<<<<<<< Updated upstream
=======
#include <Thread>
>>>>>>> Stashed changes

using namespace std;

class PlatformComThread : public QThread
{
    Q_OBJECT
protected:

public:
    DataContainer *dataContainer;
	bool loopStatusPlatform = true;
	void comPlatform();
    void run();

	//PlatformComThread() {
	//	//dataContainer = DataContainer::getInstance();
	//}
	//~PlatformComThread() {
	//	cout << "플랫폼 스레드가 종료되었습니다.\n";
	//	this->wait();
	//}


private:
	/// void run() Q_DECL_OVERRIDE; //thread 생성 후 자동 실행되는 함수

signals: /// thread가 보낼 broadcast 함수들
    void AorMChanged(int);
    void EStopChanged(int);
    void GearChanged(int);
    void SpeedChanged(int);
    void SteerChanged(int);
    void BreakChanged(int);
    void EncChanged(int);
    void AliveChanged(int);
	void PlatformExit();

public slots:

};



class LidarComThread : public QThread
{
	Q_OBJECT
protected:

public:
	DataContainer *dataContainer;
	
	void comLidar();
	void run();

private:
	/// void run() Q_DECL_OVERRIDE; //thread 생성 후 자동 실행되는 함수

signals: /// thread가 보낼 broadcast 함수들
	void LidarExit();

public slots:
<<<<<<< Updated upstream
=======
};


class ScnnThread : public QThread
{
    Q_OBJECT
protected:

public:
    DataContainer *dataContainer;
    void run();
    //int mainfun();
>>>>>>> Stashed changes

};



