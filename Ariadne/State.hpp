#ifndef STATE_HPP
#define STATE_HPP

#include "Utils.hpp"

class State {
public:
    float x;
    float y;
    float heading;
	float steer;
	float speed;

    //gx, gy and gtheta are co-ordinates in the 80X80 grid
    int gx;
    int gy;
    int gtheta;

    //for running dijkstra
    int dx;
    int dy;

    float cost2d;
    float cost3d;

    State* next;
    State* previous;

    State();
    State(float x, float y, float heading, float steer, float speed);
    vector<State> getNextStates();
};

#endif
