#ifndef UTILS_HPP
#define UTILS_HPP
#include <WinSock2.h>	
#include <Windows.h>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

#define MAPX 600
#define MAPY 900
#define MAP_Scale 0.8 // 창 크기 맞추기 위한 스케일

#define GX 100
#define GY 100
#define Grid_ResX (MAPX/GX) // grid x res 가 png 해상도보다 커야함
#define Grid_ResY (MAPY/GY)

#define D_Times 5
#define DX (GX*D_Times)//240
#define DY (GY*D_Times)

#define Theta_Res 72
#define Gtheta_Res (360/Theta_Res)
#define Deg2Rad (PI/180)

#define BOT_L 160
#define BOT_W 120
#define BOT_M_ALPHA 27
#define BOT_CEN_MASS 0.5
#define BOT_V (30*50/18)
#define BOT_T 1

#define PI 3.14159

#endif
