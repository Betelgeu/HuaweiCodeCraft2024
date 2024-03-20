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

    int CargoNum = 0;
    int CargoValue = 0;
    std::vector<std::pair<int, int>> queue; //维护一个港口的船只排队vector，包含哪艘船在什么时刻到达，方便冲突时进行选择

    bool is_full = false;
    bool space[4][4];

    void flush ();
};

#endif //_BERTH_H