#include "Robot.h"

Robot::Robot() {}

Robot::Robot(int startX, int startY) {
    this->x = startX, this->y = startY;
}

int Robot::generate_path(int Blocks[Width][Width], Point dest){
    std::pair<int, int> start = {this->x, this->y};
    std::pair<int, int> end = {dest.x, dest.y};
    Search search;
    std::vector<Point> P = search.Astar(Blocks, start, end);
    if(P.size() <= 1 || this->path.size() != 0) {
        info("robot generate path error!");
        return -1;
    }

    Point last = P[0];
    for(int i = 1; i < P.size(); i++) {
        Point cur = P[1];
        if(cur.x == last.x && cur.y == last.y + 1)this->path.push_back(0);
        else if(cur.x == last.x && cur.y == last.y - 1)this->path.push_back(1);
        else if(cur.x == last.x - 1 && cur.y == last.y)this->path.push_back(2);
        else if(cur.x == last.x + 1 && cur.y == last.y)this->path.push_back(3);
        else {
            info("generated path incontinuous!");
            return -1;
        }
    }
    return 0;
}

int Robot::move_to_cargo(int Blocks[Width][Width], std::vector<Cargo*> &CargoList) {
    // 没有路径，选择货物
    if(this->target_cargo == nullptr) {
        Allocator allocator;
        this->target_cargo = allocator.alloc_robot_cargo(this, CargoList);
        if(this->target_cargo != nullptr)this->target_cargo->selected = true;
    }


    // 已有路径，直接按照路径前往货物
    if(this->target_cargo != nullptr) {

        if(this->path.size() == 0) {
            Point cargo(this->target_cargo->x, this->target_cargo->y);

            if(this->x == cargo.x && this->y == cargo.y) {
                //到货物处了，取货
                std::cout << "get " << this->id << std::endl;
            }
            if(this->generate_path(Blocks, cargo) == -1) {
                //到不了这个货，重新选货
                Allocator allocator;
                this->target_cargo = allocator.alloc_robot_cargo(this, CargoList);
            }

        }

        // display path
        info("size: " + std::to_string(this->path.size()) + "\n");
        info("path: ");
        for(auto direct: this->path) {
            info(std::to_string(direct) + " ");
        }
        info("\n");

        int move = path[0];
        this->path.erase(path.begin());
        return move;
    }
    else {// 如果没有找到可达的货物，则返回一个特殊值表示未找到
        info("no target cargo\n");
        return -1;
    }
}

void Robot::act(int Blocks[Width][Width], std::vector<Cargo*> &CargoList) {
    if(this->is_carring_cargo == false) {
        this->move_to_cargo(Blocks, CargoList);
    }
    else {
//        move_to_berth();
    }
}

