#ifndef _BOAT_H
#define _BOAT_H

#include "tool.h"
#include "Berth.h"

using namespace std;
class Boat {
public:
    int id;
    static int capacity;
	int state;
	int target;

    int loaded_cargo_num = 0;

    Boat();

	void act(std::vector<Berth*> &BerthList);
};

#endif //_BOAT_H
