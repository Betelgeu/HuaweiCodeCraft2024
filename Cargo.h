#ifndef _CARGO_H
#define _CARGO_H

#include "tool.h"

using namespace std;
class Cargo {
public:
	int exist_time, x, y, val;
	bool selected;
	Cargo();
	Cargo(int startx, int starty, int val);
	void time_decay();
};

//check and give up outdated cargoes
void removeCargo(vector<Cargo*> &CargoList);


#endif //_CARGO_H