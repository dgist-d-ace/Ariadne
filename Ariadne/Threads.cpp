#include "Threads.h"
#include "DataContainer.h"
#include <QString>

// HyeAhnView.cpp 2619~
void PlatformComThread::comPlatform(CString tempPort)
{
    //플랫폼 통신 코드입니다.
    if (_serial.OpenPort(tempPort))   // 실제 사용될 COM Port 를 넣어야합니다. 
    {
        // BaudRate, ByteSize, fParity, Parity, StopBit 정보를 설정해줍니다.  
        _serial.ConfigurePort(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT);
        // Timeout 설정입니다. 별다른거 없으면 전부 zero 설정해도 되구요.  
        _serial.SetCommunicationTimeouts(0, 0, 0, 0, 0);


        while (loopStatusPlatform)
        {
            _serial.MyCommRead();
            _serial.MyCommWrite();

            //연결 상태 관리용
            dataContainer->updateValue_platform_status();
            //this_thread::sleep_for(100ms);
        }
    }
    else
    {
        _serial.ClosePort();
    }
}

void Camera1ComThread::comCamera1()
{
    // 카메라 통신 코드
}

void Camera2ComThread::comCamera2()
{
    // 카메라 통신 코드
}
