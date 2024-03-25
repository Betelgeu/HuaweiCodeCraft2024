#ifndef _TOOL_H
#define _TOOL_H

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <set>
#include <map>
#include <cmath>
#include <string>
#include <climits>
#include <algorithm>

#define WIDTH 200              // 地图宽度
#define ROBOT_NUM 10           // 控制的机器人数量
#define BOAT_NUM 5             // 控制的船只数量
#define BERTH_NUM 10           // 泊位数量
#define BERTH_MOVE_TIME 500    // 泊位间的移动时间都是 500 帧(10s)
#define CARGO_LEFT_TIME 1000   // 每个货物会在指定位置停留 1000 帧(20s)
#define ROBOT_RECOVERY_TIME 20 // 机器人的移动目标位置和墙壁重合 或 两个机器人的目标位置重合， 或者互相前往对方当前所在地，原地停留20帧
#define MAX_SEARCH_TIME 5      // 每帧的最大寻路次数，超过这个值会丢帧

class Cargo;
class Robot;
class Berth;
class Boat;

void info(const std::string msg);

struct Node
{
    int x, y;     // 坐标
    int g, h;     // f = g + h
    Node *parent; // 父节点

    Node(int _x, int _y) : x(_x), y(_y), g(0), h(0), parent(nullptr) {}
    Node(int _x, int _y, int _g, int _h) : x(_x), y(_y), g(_g), h(_h), parent(nullptr) {}
    Node(int _x, int _y, int _g, int _h, Node *p) : x(_x), y(_y), g(_g), h(_h), parent(p) {}

    bool operator<(const Node &rhs) const
    {
        return g + h > rhs.g + rhs.h;
    }
};

class Point
{
public:
    int x, y;
    Point() {}
    Point(int _x, int _y) : x(_x), y(_y) {}
};

bool isValid(int x, int y, int maze[WIDTH][WIDTH]);
bool isDestination(int x, int y, const Node &dest);
int calculateHValue(int x, int y, const Node &dest);

class Search
{
public:
    static int count_a, count_b;
    static int count;
    std::vector<Point> Astar(int maze[WIDTH][WIDTH], Point start, Point dest);
    std::vector<Point> Astar_robot_without_collision(int maze[WIDTH][WIDTH], Point s, Point d, Robot *RobotList, int robot_id);
};

class Allocator
{
public:
    Cargo *alloc_robot_cargo(Robot *robot, std::set<Cargo *> &CargoSet, int maze[WIDTH][WIDTH], Robot *RobotList);
    std::pair<Berth *, Point> alloc_robot_berth(Robot *robot, std::vector<Berth *> &BerthList, int maze[WIDTH][WIDTH], Robot *RobotList);
    Berth *alloc_boat_berth(Boat *boat, std::vector<Berth *> &BerthList);
};

#endif //_TOOL_H