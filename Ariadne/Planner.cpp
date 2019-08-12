#include "Planner.hpp"

State previous[GX][GY][Theta_Res];

int Planner::plan(State start, State target, Map map) {
	Compare cmp;
	cmp.initValues(target, map);
	cmp.runDijkstra();

	priority_queue<State, vector<State>, Compare> pq;
	start.cost3d=0;
	pq.push(start); // 시작점 들어가서 본격 연산 시작

	GUI display(MAPX*MAP_Scale, MAPY*MAP_Scale); // output image 해상도 조절 - 현 600 * 900, 대회 Output 항상 800 * 800
	display.drawObs(map);
	display.drawCar(start);
	display.drawCar(target);
	
	int vis[GX][GY][Theta_Res];
	memset(vis, 0, sizeof(int)*GX*GY*Theta_Res);

	while (pq.size()>0)
	{
		State current=pq.top();
		pq.pop();

		int iter = 0;

		if (abs(current.gy - target.gy) <= 5) //(abs(current.gx - target.gx) <= 5 && abs(current.gy - target.gy) <= 5 && abs(current.gtheta - target.gtheta) <= 5)//(abs(current.gx - target.gx) <= 5 && abs(current.gy-target.gy)<=5 && abs(current.gtheta-target.gtheta)<=5) //
		{
			//cout<<"Reached target."<<endl;
			State Dummy;

			while(current.x!=start.x || current.y!=start.y || current.heading!=start.heading)
			{
				display.drawCar(current);

				Dummy=previous[current.gx][current.gy][current.gtheta];

				//cout << "Current theta: " << current.heading << ", Previous theta: " << Dummy.heading << ", Steer angle: " << current.steer << endl;
				finalSteer = current.steer;
				current=Dummy;
			}

			//cout<<"Let's Go!!!!"<<endl
			display.show();

			cmp.unInitValues();

			return finalSteer;
		}

		if(vis[current.gx][current.gy][current.gtheta]){
			continue;
		}

		vis[current.gx][current.gy][current.gtheta]=1;

		vector<State> next=current.getNextStates();

		for (int i = 0; i < next.size(); i++) 
		{
			//display.drawCar(next[i]); //여기
			if (!map.checkCollision(next[i])) {
				if (!vis[next[i].gx][next[i].gy][next[i].gtheta]) {
					//display.drawCar(next[i]); //여기
					current.next = &(next[i]);
					next[i].previous = &(current);

					if (i == 0)											//개******************
						next[i].cost3d = current.cost3d + 5;
					else
						next[i].cost3d = current.cost3d + 5;

					pq.push(next[i]);
					previous[next[i].gx][next[i].gy][next[i].gtheta] = current;
				}
			}
		}
	}
	//cout<<"Can't Go...."<<endl;
	display.show(); // 경로생성 못하는 경우 보여줌

	cmp.unInitValues();

	return 90;
}
