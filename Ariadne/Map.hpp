#ifndef MAP_HPP
#define MAP_HPP

#include "State.hpp"
#include "Utils.hpp"

class Map {
public:
    int** obs_map;
    int** acc_obs_map;
    int** nearest_obstacle;
    int obs_dist_max;

    Map(Mat &imgLiDAR);
	void initObsMap(Mat obsmap);
    void initCollisionChecker();
    void find_near_obs();
    bool is_boundary_obstacle(int i, int j);
    bool checkCollision(State pos);
    int  nearest_obstacle_distance(State pos);
	void deleteMaps();
};

class node {
public:
	int x,y,nearest_obstacle;
};

#endif
