#include "Compare.hpp"

State Compare::target;
int** Compare::obs_map;
float** Compare::shortest_2d;
int** Compare::grid_obs_map;
float** Compare::cost;

bool Compare::operator() (const State s1, const State s2) {
	//to do: replace by heuristic+cost comparison
	return s1.cost3d + holonomic_with_obs(s1) > s2.cost3d + holonomic_with_obs(s2);
}

void Compare::initValues(State target, Map map) {
	//initialize variables for the Compare class
	Compare::target = target;
	Compare::obs_map = map.obs_map;

	Compare::grid_obs_map = new int*[DX];

	for (int i = 0; i < DX; i++)
	{
		Compare::grid_obs_map[i] = new int[DY];

		for (int j = 0; j < DY; j++)
			Compare::grid_obs_map[i][j] = 0;
	}

	for (int i = 0; i < MAPX; i++)
		for (int j = 0; j < MAPY; j++)
		{
			if (Compare::obs_map[i][j])
				Compare::grid_obs_map[i*DX / MAPX][j*DY / MAPY] = 1;
		}

	Compare::cost = new float*[DX];

	for (int i = 0; i < DX; i++)
	{
		Compare::cost[i] = new float[DY];

		for (int j = 0; j < DY; j++)
			Compare::cost[i][j] = 0;
	}
}

typedef bool (*compare2dSignature)(State, State);
bool compare2d(State a, State b) {
   //return a.cost2d>b.cost2d;	//simple dijkstra
   return a.cost2d+abs(Compare::target.dx-a.dx)+abs(Compare::target.dy-a.dy)>b.cost2d+abs(Compare::target.dx-b.dx)+abs(Compare::target.dy-b.dy);
}

//currently uses dijkstra's algorithm in x-y space
float Compare::holonomic_with_obs(State src) {
	return Compare::shortest_2d[(int)src.x*DX/MAPX][(int)src.y*DY/MAPY];
}

void Compare::runDijkstra() {
	State src = Compare::target;

	src.dx=src.gx*DX/GX;
	src.dy=src.gy*DY/GY;

	priority_queue<State, vector<State>, compare2dSignature> frontier(&compare2d);

	int vis[DX][DY];
	memset(vis, 0, sizeof(int) * DX * DY);

	for(int i=0;i<DX;i++)
		for(int j=0;j<DY;j++)
			Compare::cost[i][j]=10000;

	Compare::cost[src.dx][src.dy]=0;

	frontier.push(src);

	while (!frontier.empty()) {
		State current = frontier.top();
		frontier.pop();

		int x=current.dx;
		int y=current.dy;

		if(vis[x][y])
			continue;

		vis[x][y]=1;

		for (int i = -D_Times/2; i <= D_Times/2; i++)
			for (int j = -D_Times/2; j <= D_Times/2; j++)
			{
				if (x + i < 0 || x + i >= DX || y + j < 0 || y + j >= DY)
					continue;
				if ((i == 0 && j == 0) || Compare::grid_obs_map[x + i][y + j] != 0)
					continue;

				if (Compare::cost[x + i][y + j] > Compare::cost[x][y] + sqrt(i*i + j*j)) {
					Compare::cost[x + i][y + j] = Compare::cost[x][y] + sqrt(i*i + j*j);
					State tempstate;

					tempstate.dx = current.dx + i;
					tempstate.dy = current.dy + j;
					tempstate.cost2d = Compare::cost[x + i][y + j];
					frontier.push(tempstate);
				}
			}
	}

	Compare::shortest_2d= Compare::cost;
	/*
	Mat dist(DX, DY, CV_8UC3, Scalar(255, 255, 255));

	for(int i=0;i<DX;i++)
		for(int j=0;j<DY;j++)
		{
			Vec3b vec(255-0.6*shortest_2d[i][j], 255-0.6*shortest_2d[i][j], 255-0.6*shortest_2d[i][j]);
			dist.at<Vec3b>(j,i)=vec;
		}

	resize(dist, dist, Size(MAPX, MAPY));
	imshow("Field", dist);*/
}

void Compare::unInitValues() {
	for (int i = 0; i < DX; i++)
	{
		delete[] Compare::grid_obs_map[i];
	}
	delete[] Compare::grid_obs_map;
	Compare::grid_obs_map = nullptr;

	for (int i = 0; i < DX; i++)
	{
		delete[] Compare::cost[i];
	}
	delete[] Compare::cost;
	Compare::cost = nullptr;
}