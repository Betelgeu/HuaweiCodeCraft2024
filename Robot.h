#ifndef _ROBOT_H
#define _ROBOT_H

#include "Cargo.h"
#include "Berth.h"
#include "tool.h"

class Robot {
public:
    int id;
	int x, y;//坐标
	Cargo *target_cargo = nullptr;
	Berth *target_berth = nullptr;
    Point berth_pos;
    bool is_carring_cargo = false;
	int is_running = true;

    bool dead = false;
    int searched_fail_time = 0;

    // 存储生成的路径上的点的位置信息
    std::vector<Point> path;
    // 当前位置在path中的index
    int path_index = -1;


	Robot();
	Robot(int start_x, int start_y);


    bool is_available(int Blocks[Width][Width], Point dest);
    int generate_path(int Blocks[Width][Width], Point dest, Robot *RobotList);

	int move_to_cargo(int Blocks[Width][Width], std::set<Cargo*> &CargoSet, Robot *RobotList);
    int move_to_berth(int Blocks[Width][Width], std::vector<Berth*> &BerthList, Robot *RobotList);

	void act(int Blocks[Width][Width], std::set<Cargo*> &CargoSet, std::vector<Berth*> &BerthList, Robot *RobotList);

};

#endif //_ROBOT_H