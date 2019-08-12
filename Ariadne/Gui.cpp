#include "Gui.hpp"

GUI::GUI(int rows, int cols) {
	display_size=cv::Size(rows, cols);
	display=Mat(display_size, CV_8UC3, Scalar(255,255,255));
}

void GUI::drawCar(State src) {
	RotatedRect rotRect = RotatedRect(Point2f(src.x*display_size.width / MAPX, src.y*display_size.height / MAPY), Size2f(BOT_L*display_size.width / MAPX, BOT_W*display_size.height / MAPY), src.heading);
	Point2f vert[4];
	rotRect.points(vert);
	for(int i=0;i<4;i++)
		line(display, vert[i], vert[(i+1)%4], Scalar(200, 0, 0));

	//cout << "Current theta : " << src.theta <<  ", " <<src.gtheta << endl;

	//circle(display, Point2f((src.x + 17 * cos(src.theta*2.0*PI / Theta_res))*display_size.width / MAPX, (src.y + 17 * sin(src.theta*2.0*PI / Theta_res))*display_size.height / MAPY), 5, Scalar(255, 0, 0));
	circle(display, Point2f((src.x + 17 * cos(src.heading*Deg2Rad))*display_size.width / MAPX, (src.y + 17 * sin(src.heading*Deg2Rad))*display_size.width / MAPX), 5, Scalar(255, 0, 0));
}

void GUI::drawObs(Map map) {
	for(int i=0;i<MAPX;i++)
		for(int j=0;j<MAPY;j++)
			if(map.obs_map[i][j]) {
				for(int k=i*display_size.width/MAPX;k<(i+1)*display_size.width/MAPX;k++)
					for(int l=j*display_size.height/MAPY;l<(j+1)*display_size.height/MAPY;l++)
						display.at<Vec3b>(l,k)={128, 128, 128};
			}
}

void GUI::markPoint(int i, int j) {
	if(i<0 || i>MAPX || j<0 || j>MAPY)
		return;

	for(int k=i*display_size.width/MAPX;k<(i+1)*display_size.width/MAPX;k++)
		for(int l=j*display_size.height/MAPY;l<(j+1)*display_size.height/MAPY;l++)
			display.at<Vec3b>(k,l)={0, 255, 0};
}

void GUI::show() {
	Mat temp=display;
	resize(display, temp, display_size);
	imshow("Display", temp);
	/*
	Mat cirImg = Mat::zeros(600, 600, CV_8UC3);

	circle(cirImg, Point2f(315.089, 424.9976), 5, Scalar(0, 255, 0), -1); // 왼쪽차선 원 중심
	circle(cirImg, Point2f(320.6, 430), 5, Scalar(0, 255, 0), -1); // 오른쪽 차선 원 중심

	ellipse(cirImg, Point2f(315.089, 424.9976), Size(113, 113), 0, -48.6, 41.4, Scalar(255, 0, 0), 3, 8); // 왼쪽 차선
	ellipse(cirImg, Point2f(320.6, 430), Size(150, 150), 0, -53.29, 36.71, Scalar(255, 0, 0), 3, 8); //오른쪽 차선

	imshow("Circle", cirImg);*/
	return;
}