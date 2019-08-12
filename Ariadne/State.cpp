#include "State.hpp"

State::State(float x, float y, float heading, float steer, float speed) {
	this->x=x;
	this->y=y;
	this->heading=heading;
	this->steer=steer;
	this->speed=speed;

	this->gx=x/Grid_ResX;
	this->gy=y/Grid_ResY;
	this->gtheta=heading/Theta_Res;
}

State::State() {
	this->x = -1;
	this->y = -1;
	this->heading = -1;
	this->steer = -1;
	this->speed = -1;
}

vector<State> State::getNextStates() { // 현재 나의 위치: x, y, 현재 나의 헤딩: theta
	vector<State> next;
	State n;

	int alphaList[11] = { 0, 15, -15, 10, -10, 20, -20, 5, -5, 25, -25 };
	float alpha, beta, r, d = BOT_V * BOT_T; // BOT_L*BOT_CEN_MASS; // alpha: 조향각, beta: d/r_r, r: 회전반경, d: 뒷바퀴에서 무게중심까지 거리
	
	for (alpha = -BOT_M_ALPHA; alpha <= BOT_M_ALPHA + 0.01; alpha += 3) //(int i = 0; i < 11; i++)
	{
		//alpha = alphaList[i];
		beta=d*tan(alpha*PI/180)/BOT_L;

		if(abs(beta)<0.001){ // 직진
			n.x = x + d * cos(heading * Deg2Rad);
			n.y = y + d * sin(heading * Deg2Rad);
			n.heading=heading;
			n.steer = alpha;
		}
		else{ // 회전
			r=BOT_L/tan(alpha*PI/180);
			n.x = x + r * sin(heading * Deg2Rad + beta) - r * sin(heading * Deg2Rad);
			n.y = y - r * cos(heading * Deg2Rad + beta) + r * cos(heading * Deg2Rad);
			n.heading = heading + beta / Deg2Rad;
			n.steer = alpha;
		}

		n.gx=n.x/Grid_ResX;
		n.gy=n.y/Grid_ResY;
		n.gtheta=n.heading/Theta_Res;
		next.push_back(n);
	}

	// cout<<"getNextStates() called from "<<x<<","<<y<<","<<heading<<endl;
	//for(int i=0;i<3;i++) cout<<next[i].x<<","<<next[i].y<<","<<next[i].heading<<"  "; cout<<endl;

	return next;
}
