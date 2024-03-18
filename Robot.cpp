#include "Robot.h"

Robot::Robot() {}
Robot::Robot(int startX, int startY) {
    this->x = startX, this->y = startY;
    this->carrying_cargo = 0;
    this->running = 1;
}

int Robot::move_to_cargo(int Blocks[Width][Width], vector<Cargo*> &CargoList) {
    // 定义对于这个robot，每个未被选择的货物的value

    Cargo *cargo_max_value = this->target_cargo;
    if(cargo_max_value == nullptr) {
        double max_value = 0;
//        info(std::to_string(CargoList.size()) + "\n");
        for(auto cargo : CargoList) {
            if(cargo->selected == false) {
//                info(std::to_string(cargo->x) + " " + std::to_string(cargo->y) + "\n");
                int dist = abs(cargo->x - this->x) + abs(cargo->y - this->y);
                double value = double(cargo->val) / dist;
//                info(std::to_string(value) + "\n");
                if(value > max_value) {
                    max_value = value;
                    cargo_max_value = cargo;
                }
            }
        }
    }


    // 如果找到了一个货物，则前往这个货物 || 或者本来就有目标了
    if(cargo_max_value != nullptr) {
        this->target_cargo = cargo_max_value;
        cargo_max_value->selected = true;

        //A*算法前往target
        Node start(this->x, this->y);
        Node end(cargo_max_value->x, cargo_max_value->y);
        info("cargo: " + std::to_string(cargo_max_value->x) + " " + std::to_string(cargo_max_value->y) + "\n");
        Search search;
        std::vector<Node> path = search.Astar(Blocks, start, end);
        if(path.size() <= 1){
            info("no need to move or can't reach\n");
            return -1;
        }

        info("size: " + to_string(path.size()) + "\n");
        info("path: ");
        for(auto Node : path) {
            int x = Node.x, y = Node.y;
            info(std::to_string(x) + " " + std::to_string(y) + ", ");
        }
        info("\n");

        Node next = path[1];
        if(next.x == this->x + 1 && next.y == this->y)return 0;
        else if(next.x == this->x - 1 && next.y == this->y)return 1;
        else if(next.x == this->x && next.y == this->y + 1)return 2;
        else if(next.x == this->x && next.y == this->y - 1)return 3;
        else {
            info("invalid move\n");
            info(std::to_string(next.x) + " " + std::to_string(next.y));
            return -1;
        }
    }
    else {// 如果没有找到可达的货物，则返回一个特殊值表示未找到
        info("no target cargo\n");
        return -1;
    }
}

void Robot::move() {}