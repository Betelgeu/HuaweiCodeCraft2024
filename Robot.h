#ifndef _ROBOT_H
#define _ROBOT_H

#include "Cargo.h"
#include "Berth.h"
#include "tool.h"

class Robot {
public:
	int x, y;//坐标
	Cargo *target_cargo = nullptr;
	Berth *target_berth = nullptr;
	int carrying_cargo;
	int running;
	// 0->空闲 1->前往货物 2->前往泊位 3->到达 4->碰撞恢复中

	Robot();
	Robot(int start_x, int start_y);

	int move_to_cargo(int Blocks[Width][Width], vector<Cargo*> &CargoList);
	int move_to_berth();
	void move();
};

#endif //_ROBOT_H