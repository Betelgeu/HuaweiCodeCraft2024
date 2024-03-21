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
#include <algorithm>

#define Width 200
#define RobotNum 10
#define BoatNum 5
#define BerthNum 10
#define BerthMovementTime 500//泊位间的移动时间都是 500 帧(10s)
#define cargoLeftTime 1000//每个货物会在指定位置停留 1000 帧(20s)
#define RobotRecoveryTime 20//机器人的移动目标位置和墙壁重合 或 两个机器人的目标位置重合， 或者互相前往对方当前所在地，原地停留20帧
#define MaxSearchTimePerFrame 5//每帧的最大寻路次数，超过这个值会丢帧

class Cargo;
class Robot;
class Berth;

void info(const std::string msg);

struct Node {
    int x, y; // 坐标
    int g, h; // f = g + h
    Node* parent; // 父节点

    Node(int _x, int _y) : x(_x), y(_y), g(0), h(0), parent(nullptr) {}
    Node(int _x, int _y, int _g, int _h) : x(_x), y(_y), g(_g), h(_h), parent(nullptr) {}
    Node(int _x, int _y, int _g, int _h, Node *p) : x(_x), y(_y), g(_g), h(_h), parent(p) {}

    bool operator < (const Node& rhs) const {
        return g + h > rhs.g + rhs.h;
    }
};

class Point {
public:
    int x, y;
    Point() {}
    Point(int _x, int _y): x(_x), y(_y) {}
};

bool isValid(int x, int y, int maze[Width][Width]);
bool isDestination(int x, int y, const Node& dest);
int calculateHValue(int x, int y, const Node& dest);


class Search {
public:
    static int count_a, count_b;
    static int count;
    std::vector<Point> Astar(int maze[Width][Width], Point start, Point dest);
    std::vector<Point> Astar_robot_without_collision(int maze[Width][Width], Point s, Point d, Robot *RobotList, int robot_id);
};

class Allocator {
public:
    Cargo* alloc_robot_cargo(Robot *robot, std::set<Cargo*> &CargoSet, int maze[Width][Width], Robot *RobotList);
    std::pair<Berth*, Point> alloc_robot_berth(Robot *robot, std::vector<Berth*> &BerthList, int maze[Width][Width], Robot *RobotList);

    std::vector<double> Berth_w(std::vector<Berth*> berthes);
};

#endif //_TOOL_H