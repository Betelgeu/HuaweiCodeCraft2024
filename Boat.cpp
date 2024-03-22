#include "Boat.h"
#include "tool.h"

Boat::Boat(){}

void Boat::act(std::vector<Berth*> &BerthList) {
    if(this->state == 1) {
        if(this->target == -1) {    // 1. 在虚拟点，已经完成送货
            this->loaded_cargo_num = 0;
            Allocator alloc;
            Berth *target_berth = alloc.alloc_boat_berth(this, BerthList);
            cout << "ship " << this->id << " " << target_berth->id << endl;
        }
        else {                      // 2. 在泊位装货
            Berth *berth = BerthList[this->target];
            int boat_left_space = Boat::capacity - this->loaded_cargo_num;

            if(boat_left_space && berth->cargo_values.size()) {//能装就装
                berth->give_boat(this);
            }
            else {//不能装就走
                cout << "go " << this->id << " " << endl;
            }
        }
    }


    else if(this->state == 0){//移动中, 以下情况由判题器自动维护
        if(this->target == -1) {}   // 3. 送货到虚拟点
        else {}                     // 4. 送货到泊位
    }
    else {                          // 5. 泊位外等待
    }
}

