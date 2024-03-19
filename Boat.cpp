#include "Boat.h"
#include "Berth.h"
#include "Cargo.h"

Boat::Boat() {}

double* berth_w (Berth berthes[], vector<Cargo> goodslist, int berthnum) {
	double* first_berth=new double[berthnum];
	for (int i = 0; i < berthnum; i++) {
		first_berth[i] = 0;
	}
	//未考虑三个值的占比！！！
	for (int i = 0; i < berthnum; i++) {
		first_berth[i] += berthes[i].transport_time;//运输时间
		first_berth[i] += 1 / static_cast<double>((berthes[i].loading_speed));//装一个货的时间
		//first_berth[i] +=距离/价值（一点价值需要的距离，即帧数）
	}
	return first_berth;
}


void boat_action(Boat boats[], Berth berthes[], vector<Cargo> goodslist, int boatnum, int berthnum, int frameid, int boat_capacity) {
	if (frameid == 1) {//第一帧船统一在虚拟节点，去（最近+装货最快+物品价值/距离）最高的五个泊口（调参？？？）
		for (int i = 0; i < boatnum; i++) {
			boats[i].state = 1;
			boats[i].pos = -1;
		}
		double* first_berth_w = berth_w(berthes, goodslist, berthnum);

		int maxid = 0;
		for (int i = 0; i < boatnum; i++) {
			for (int j = 0; j < berthnum; j++) {
				if (first_berth_w[j] > first_berth_w[maxid]) {
					maxid = j;
				}
			}
			boats[i].pos = maxid;
			first_berth_w[maxid] = 0;
		}

		for (int i = 0; i < boatnum; i++) {
			cout << "ship" << " " << boatnum << " " << boats[boatnum].pos << endl;
		}
	}

	else {
		
		for (int i = 0; i < boatnum; i++) {
			double* all_berth_w = berth_w(berthes, goodslist, berthnum);
			if (boats[i].state == 0) {//移动中
				boats[i].moving_time++;
			}

			else if (boats[i].state == 1) {//装货状态或者运输完成状态
				if (boats[i].pos == -1) {//到虚拟节点
					if (boats[i].moving_time == berthes[boats[i].last_pos].transport_time) {//刚到虚拟节点立刻离开
						int maxid = 0;
						for (int j = 0; j < berthnum; j++) {
							if (all_berth_w[j] > all_berth_w[maxid]) {
								maxid = j;
							}
						}
						///boats[i].pos = maxid;
						cout << "ship" << " " << i << " " << maxid;
						all_berth_w[maxid] = 0;//防止都去同一个泊口
						boats[i].loaded_num = 0;
						boats[i].moving_time = 1;
						boats[i].last_pos = boats[i].pos;
					}
				}
				else {//到泊口
					if (boats[i].last_pos==-1&&boats[i].moving_time == berthes[boats[i].pos].transport_time) {//从虚拟节点来（暂时未考虑泊口之间的移动）
						boats[i].loaded_num += berthes[boats[i].pos].loading_speed;//从虚拟节点来泊口立马装货
						boats[i].moving_time = 0;
						boats[i].last_pos = boats[i].pos;
					}
					else if (boats[i].last_pos != -1) {//在节点装货
						if (boats[i].last_pos == boats[i].pos) {
							if (boats[i].loaded_num > boat_capacity - berthes[boats[i].last_pos].loading_speed) {
								//last_pos不变
								cout << "go" << " " << i << endl;
							}
							else {
								boats[i].loaded_num += berthes[boats[i].last_pos].loading_speed;
							}
						}

						else if (boats[i].last_pos != boats[i].pos) {//从别的泊口来暂未考虑

						}
					}
				}
				
			}

			else if (boats[i].state == 2) {//泊位外等待状态
				boats[i].last_pos = boats[i].pos;//可能回去别的泊口，故last_pos更改
			}
		}
	}
}
