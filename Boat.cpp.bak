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
			Point start(berthes[i].x, berthes[i].y);
			Point end(element.x, element.y);
			std::vector<Point> path = search.Astar(Blocks, start, end);
			size_t dist = path.size();
			first_berth[i] += element.val / dist;
		}
		//first_berth[i] +=距离/价值（一点价值需要的距离，即帧数）
	}
	return first_berth;
}

void Boat::action(vector<Berth*> &berthes, vector<double>& Ber_weight, int frameid, int BerthFrame[], int boat_capacity, int id) {
    if (frameid == 1) {//第一帧船统一在虚拟节点，去（最近+装货最快+物品价值/距离）最高的五个泊口（调参？？？）
        this->state = 1;
        this->pos = -1;
        int maxid = 0;
        for (int j = 0; j < BerthNum; j++) {
            if (Ber_weight[j] > Ber_weight[maxid]) {
                maxid = j;
            }
        }
        Ber_weight[maxid] = 0;//第一帧Ber_weight同时处理所有船,后面的帧需要另外处理
        this->pos = maxid;
        info("state of boat" + to_string(this->last_state) + " " + to_string(this->state) + " "+to_string(this->pos)+"\n");
        cout << "ship " << id << " " << this->pos << endl;
    }

    else {
        info("state of boat" + to_string(this->last_state) + " " + to_string(this->state) + " " + to_string(this->pos) +" "+ to_string(BerthFrame[id]) + " " + to_string(frameid) + "\n");
        if (this->state == 0) {//移动中
        }

        else if (this->state == 1) {//装货状态或者运输完成状态
            if (this->pos == -1) {//到虚拟节点
                int maxid = 0;
                for (int j = 0; j < BerthNum; j++) {
                    if (berthes[j]->ship_select == false) {
                        if (Ber_weight[j] > Ber_weight[maxid]) {
                            maxid = j;
                        }
                    }
                }
                berthes[maxid]->ship_select = true;
                cout << "ship " << id << " " << maxid;
                Ber_weight[maxid] = 0;//防止同一帧都去同一个泊口（虽然概率很低）
            }
            else {//到泊口
                if (this->last_state == 0) {//刚到泊口，记录当前帧
                    berthes[this->pos]->ship_select = false;//泊口可选
                    BerthFrame[id] = frameid;
                }
                if ((frameid- BerthFrame[id]) - (boat_capacity / berthes[this->pos]->loading_speed + 1) > 0) {
                    cout << "go " << id << endl;
                }
            }
        }

        else if (this->state == 2) {//泊位外等待状态

        }
    }


}