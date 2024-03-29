#ifndef PLANNER_HPP
#define PLANNER_HPP

#include "Map.hpp"
#include "State.hpp"
#include "Compare.hpp"
#include "Gui.hpp"
#include "Utils.hpp"

class Planner {
public:
	int finalSteer;

	int plan(State start, State target, Map map);
};

#endif