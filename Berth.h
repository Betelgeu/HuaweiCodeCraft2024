#ifndef _BERTH_H
#define _BERTH_H

#include "tool.h"
class Berth {
public:
    int x; // 左上角坐标
    int y;
    int transport_time;//轮船从泊位到虚拟点时间
    int loading_speed;//每帧可以装载的数量
    Berth();
    Berth(int x, int y, int transport_time, int loading_speed);

    bool is_full  = false;
    bool space[2][4];
    int CargoNum = 0;

    void flush();
};

#endif //_BERTH_H