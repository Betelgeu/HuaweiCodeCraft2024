#include "Boat.h"
#include "Berth.h"
#include "Cargo.h"
#include "tool.h"

Boat::Boat() {}

double* berth_w (Berth berthes[], vector<Cargo> goodslist, int berthnum, int Blocks[Width][Width]) {
	double* first_berth=new double[berthnum];
	Search search;
	for (int i = 0; i < berthnum; i++) {
		first_berth[i] = 0;
	}
	//未考虑三个值的占比！！！
	for (int i = 0; i < berthnum; i++) {
		first_berth[i] += berthes[i].transport_time;//运输时间
		first_berth[i] += 1 / static_cast<double>((berthes[i].loading_speed));//装一个货的时间
		for (const auto& element : goodslist)
		{
			std::pair<int, int> start = {berthes[i].x, berthes[i].y};
			std::pair<int, int> end = {element.x, element.y};
			std::vector<Point> path = search.Astar(Blocks, start, end);
			size_t dist = path.size();
			first_berth[i] += element.val / dist;
		}
		//first_berth[i] +=距离/价值（一点价值需要的距离，即帧数）
	}
	return first_berth;
}


void boat_action(Boat boats[], Berth berthes[], vector<Cargo> goodslist, int boatnum, int berthnum, int frameid, int boat_capacity, int Blocks[Width][Width]) {
	for (int i = 0; i < berthnum, i++)
	{
		berthes[i].flush();
	}
	if (frameid == 1) {//第一帧船统一在虚拟节点，去（最近+装货最快+物品价值/距离）最高的五个泊口（调参？？？）
		for (int i = 0; i < boatnum; i++) {
			boats[i].state = 1;
			boats[i].pos = -1;
		}
		double* first_berth_w = berth_w(berthes, goodslist, berthnum, Blocks);

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
		for (int i = 0; i < boatnum; i++) //10是船的数量
		{
			std::pair<int, int> item = {i, frameid + berthes[boats[i].pos].transport_time}; //船只id，到达时间
			berthes[boats[i].pos].queue.push_back(item);
		}

		for (int i = 0; i < boatnum; i++) {
			cout << "ship" << " " << boatnum << " " << boats[boatnum].pos << endl;
		}
	}

	else {
		
		for (int i = 0; i < boatnum; i++) {
			double* all_berth_w = berth_w(berthes, goodslist, berthnum, Blocks);
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
						boats[i].pos = maxid; //为啥要注释

						std::pair<int, int> item = {i, frameid + berthes[boats[i].pos].transport_time}; //船只id，到达时间
						berthes[boats[i].pos].queue.push_back(item);

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
								if (!berthes[boats[i].pos].queue.empty())
								{	
									berthes[boats[i].pos].queue.erase(berthes[boats[i].pos].queue.begin());
								}
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
	for (int i = 0; i < berthnum, i++)
	{
		berthes[i].flush();
	}
}
