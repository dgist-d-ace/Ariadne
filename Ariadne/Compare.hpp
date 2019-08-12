#ifndef COMPARE_HPP
#define COMPARE_HPP

#include "State.hpp"
#include "Map.hpp"
#include "Utils.hpp"

class Compare {
public:
	static State target;
	static int** obs_map;
	static int** grid_obs_map;
	static float** shortest_2d;
	static float** cost;

    bool operator() (const State s1, const State s2);
	void initValues(State target, Map map);
    float holonomic_with_obs(State src);
    void runDijkstra();
	void unInitValues();
};

#endif COMPARE_HPP