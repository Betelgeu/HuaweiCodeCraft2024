#ifndef _BERTH_H
#define _BERTH_H

#include "tool.h"
class Boat;
class Berth {
public:
    //初始化变量
    int id;
    int x; // 左上角坐标
    int y;
    int time;//轮船从泊位到虚拟点时间
    int velocity;//每帧可以装载的数量
    //判题器维护变量
    int selected_ship_num = 0;

    //本程序维护变量
    int totalValues = 0;
    std::queue<int> cargo_values; // 别的船转走货物后的剩余物品价值队列
    std::set<Robot*> select_failed_robots;


    Berth();
    Berth(int id, int x, int y, int transport_time, int loading_speed);


    void take_in_cargo(int cargo_value);
    void give_boat(Boat* boat);
};

#endif //_BERTH_H
