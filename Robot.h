#ifndef _ROBOT_H
#define _ROBOT_H

#include "Cargo.h"
#include "Berth.h"
#include "tool.h"

class Robot
{
public:
    int id;
    int x, y;
    Cargo *target_cargo = nullptr;
    Berth *target_berth = nullptr;
    Point berth_pos;
    bool is_carring_cargo = false;
    int is_running = true;
    // 被困死的机器人视为死亡
    bool dead = false;

    // 存储生成的路径上的点的位置信息
    std::vector<Point> path;
    // 当前位置在path中的index
    int path_index = -1;

    Robot();
    Robot(int start_x, int start_y);

    bool is_available(int Blocks[WIDTH][WIDTH], Point dest);
    int generate_path(int Blocks[WIDTH][WIDTH], Point dest, Robot *RobotList);
    int move_to_cargo(int Blocks[WIDTH][WIDTH], std::set<Cargo *> &CargoSet, Robot *RobotList);
    int move_to_berth(int Blocks[WIDTH][WIDTH], std::vector<Berth *> &BerthList, Robot *RobotList);
    void act(int Blocks[WIDTH][WIDTH], std::set<Cargo *> &CargoSet, std::vector<Berth *> &BerthList, Robot *RobotList);
};

#endif //_ROBOT_H