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

    vector<Node> path;


	Robot();
	Robot(int start_x, int start_y);

    void generate_path(int Blocks[Width][Width], Node dest);
	int move_to_cargo(int Blocks[Width][Width], vector<Cargo*> &CargoList);
    //TO_DO
    // 1. 碰撞检测和避免
    // 2. 有新的货物出现时，机器人再决定是否修改路线
    // 3. 取货
    // 4. 送货
    int move_to_berth();
	void move(int Blocks[Width][Width], Node dest);

    void get_cargo();
    void pull_cargo();

};

#endif //_ROBOT_H