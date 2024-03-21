#ifndef _BOAT_H
#define _BOAT_H

#include "tool.h"
#include "Berth.h"

using namespace std;
class Boat {
public:
	int state;//״̬
	int pos;//Ŀ겴λ

	int last_state;
	Boat();
	void action(vector<Berth*> &berthes, set<Cargo*> goodslist, vector<double>& Ber_weight, int frameid, int BerthFrame[],int boat_capacity,int id);
};

#endif //_BOAT_H
