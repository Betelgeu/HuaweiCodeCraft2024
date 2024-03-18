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
    // 选择货物
    if(cargo_max_value == nullptr) {
        double max_value = 0;
        for(auto cargo : CargoList) {
            if(cargo->selected == false) {
                int dist = abs(cargo->x - this->x) + abs(cargo->y - this->y);
                double value = double(cargo->val) / dist;
                if(value > max_value) {
                    max_value = value;
                    cargo_max_value = cargo;
                }
            }
        }
    }


    // 直接前往货物
    if(cargo_max_value != nullptr) {
        this->target_cargo = cargo_max_value;
        cargo_max_value->selected = true;

        //A*算法前往target
        std::pair<int, int> start = {this->x, this->y};
        std::pair<int, int> end = {cargo_max_value->x, cargo_max_value->y};
        info("cargo: " + std::to_string(cargo_max_value->x) + " " + std::to_string(cargo_max_value->y) + "\n");
        Search search;
        std::vector<Node> path = search.Astar(Blocks, start, end);
        if(path.size() <= 1){
            info("no need to move or can't reach\n");
            return -1;
        }

        // display path
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
            info("invalid move:");
            info(std::to_string(next.x) + " " + std::to_string(next.y) + "\n");
            return -1;
        }
    }
    else {// 如果没有找到可达的货物，则返回一个特殊值表示未找到
        info("no target cargo\n");
        return -1;
    }
}

void Robot::generate_path(int Blocks[Width][Width], Node dest) {

}

void Robot::move(int Blocks[Width][Width], Node dest) {
    if(this->path.size() == 0)generate_path(Blocks,dest);

}

