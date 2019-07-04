#include "Threads.h"
#include "DataContainer.h"
#include <QString>

// HyeAhnView.cpp 2619~
void PlatformComThread::comPlatform(CString tempPort)
{
    //�÷��� ��� �ڵ��Դϴ�.
    if (_serial.OpenPort(tempPort))   // ���� ���� COM Port �� �־���մϴ�. 
    {
        // BaudRate, ByteSize, fParity, Parity, StopBit ������ �������ݴϴ�.  
        _serial.ConfigurePort(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT);
        // Timeout �����Դϴ�. ���ٸ��� ������ ���� zero �����ص� �Ǳ���.  
        _serial.SetCommunicationTimeouts(0, 0, 0, 0, 0);


        while (loopStatusPlatform)
        {
            _serial.MyCommRead();
            _serial.MyCommWrite();

            //���� ���� ������
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
    // ī�޶� ��� �ڵ�
}

void Camera2ComThread::comCamera2()
{
    // ī�޶� ��� �ڵ�
}
