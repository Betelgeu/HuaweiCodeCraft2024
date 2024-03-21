#ifndef _CARGO_H
#define _CARGO_H

#include "tool.h"
#include "Robot.h"

using namespace std;
class Cargo {
public:
	int exist_time, x, y, val;
	bool selected;
    std::set<Robot*> select_failed_robots;

	Cargo();
	Cargo(int startx, int starty, int val);

	void time_decay();
};



#endif //_CARGO_H