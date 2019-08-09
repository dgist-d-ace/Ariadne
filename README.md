# Ariadne
2019 International Student Car Competition : Auto-Driving Team D-Ace

# Ariadne UI Function Description
![alt text](https://postfiles.pstatic.net/MjAxOTA4MTBfMjA5/MDAxNTY1Mzc4MTg1OTI5.Uc9IyeYz87Rm5piWzKrALXFD_KqsIp3Lwi6GZF68ra4g.RD79EQsvF_q3hrbZAfZsx9zyCeTlRPQheP6Apx6a43Ug.PNG.cdoyu/UIButton.png?type=w580)
![alt text](https://postfiles.pstatic.net/MjAxOTA4MTBfMTI1/MDAxNTY1Mzc4MTg5MjUy.mvBA7EznlP-HFZAsT3slBDBrhCkz9Drac7JXc4X6W8sg.Bx69BciOl03HzycmM64dG9261Ay8CdoPZysc_TBtc2Eg.PNG.cdoyu/UIButton2.png?type=w580)

# Ariadne program hierarchy
![alt text](https://postfiles.pstatic.net/MjAxOTA4MTBfMjAw/MDAxNTY1Mzc4MTc1ODUx.CWugtekXf_WcWrr2mYzbEi_SFn2pVIg-m80nwYfAsBMg.vHEthG3dxMyB4Chh3K8eDxuBzLfNfPoPVF8XPme6Wpgg.PNG.cdoyu/ariadneHierarchy.png?type=w580)
main.cpp의 main 함수에서 ui_Ariadne 창을 생성합니다.
Ariadne.h에는 GPS, Platform 클래스와 Ariadne 클래스가 정의되어있으며, Ariadne.cpp에서 버튼을 함수와 연결하고 쓰레드를 생성하며 ui를 업데이트합니다.
SensorStatus.h에는 yolo, scnn class가 정의되어있으며, SensorStatus.cpp파일에서 실행되는 comLidar, comScnn 함수에서 전처리가 끝난 데이터를 dataContainer에 보관합니다.

# Data Processing in each Sensors: Platform, LiDAR, GPS, SCNN - which contained in dataContainer
각 데이터를 어떻게 가공했는가, DataContainer에 들어가는 변수들의 의미_추후 업데이트 예정

# Platform Communication functions to design autodriving algorithm
플랫폼 communication 함수 설정 방법
![alt text](https://postfiles.pstatic.net/MjAxOTA4MTBfNDgg/MDAxNTY1Mzc4MTgyMjUx.28JR8RDkz5GPoAI1lYPJMlSCCwni8X199G8US_fs46og.NerUntpluxmQpXulz7Gjl2M0SE2KUY4h5L69xkM-HTsg.PNG.cdoyu/PlatformFunction.png?type=w580)
