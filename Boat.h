#ifndef _BOAT_H
#define _BOAT_H

#include "tool.h"

using namespace std;
class Boat {
public:
	int state;//状态
	int pos;//目标泊位

    int last_state;
	int loaded_num;//货物数
	int last_pos;//上一个泊位
	int moving_time;//移动时间
	Boat();
    void action(vector<Berth*> &berthes, vector<double>& Ber_weight, int frameid, int BerthFrame[],int boat_capacity,int id);
};




#endif //_BOAT_H
