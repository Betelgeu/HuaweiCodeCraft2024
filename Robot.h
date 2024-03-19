#ifndef _ROBOT_H
#define _ROBOT_H

#include "Cargo.h"
#include "Berth.h"
#include "tool.h"

class Robot {
public:
    int id;
	int x, y;//坐标
    Point target;
	Cargo *target_cargo = nullptr;
	Berth *target_berth = nullptr;
    bool is_carring_cargo = false;
	int is_running = true;
	// 0->空闲 1->前往货物 2->前往泊位 3->到达 4->碰撞恢复中

    std::vector<int> path;


	Robot();
	Robot(int start_x, int start_y);

    int generate_path(int Blocks[Width][Width], Point dest);
	int move_to_cargo(int Blocks[Width][Width], std::vector<Cargo*> &CargoList);
    //TO_DO
    // 1. 碰撞检测和避免
    // 2. 有新的货物出现时，机器人再决定是否修改路线
    // 3. 取货
    // 4. 送货
    int move_to_berth(int Blocks[Width][Width], std::vector<Berth*> &BerthList);
	void act(int Blocks[Width][Width], std::vector<Cargo*> &CargoList, std::vector<Berth*> &BerthList);

    void get_cargo();
    void pull_cargo();

};

#endif //_ROBOT_H