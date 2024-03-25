#ifndef _CARGO_H
#define _CARGO_H

#include "tool.h"
#include "Robot.h"

using namespace std;
class Cargo
{
public:
	int exist_time, x, y, val;
	// 被哪个机器人id选择了
	int selected = -1;
	std::set<Robot *> select_failed_robots;

	Cargo();
	Cargo(int startx, int starty, int val);
};

#endif //_CARGO_H