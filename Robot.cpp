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
        Point cur = P[i];
        if(cur.x == last.x && cur.y == last.y + 1)this->path.push_back(0);
        else if(cur.x == last.x && cur.y == last.y - 1)this->path.push_back(1);
        else if(cur.x == last.x - 1 && cur.y == last.y)this->path.push_back(2);
        else if(cur.x == last.x + 1 && cur.y == last.y)this->path.push_back(3);
        else {
            info("generated path incontinuous!");
            return -1;
        }
        last = cur;
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
                this->target_cargo = nullptr;
                std::cout << "get " << this->id << std::endl;
//                target_cargo == nullptr;
                return -1;
            }
            if(this->generate_path(Blocks, cargo) == -1) {
                //到不了这个货，重新选货
                Allocator allocator;
                Cargo* cur = this->target_cargo;
                this->target_cargo = allocator.alloc_robot_cargo(this, CargoList);
                cur->selected = false;
            }

        }

        // display path
//        info("size: " + std::to_string(this->path.size()) + "\n");
//        info("path: ");
//        for(auto direct: this->path) {
//            info(std::to_string(direct) + " ");
//        }
//        info("\n");

        int move = path[0];
        this->path.erase(path.begin());
        return move;
    }
    else {// 如果没有找到可达的货物，则返回一个特殊值表示未找到
        info("no target cargo\n");
        return -1;
    }
}

int Robot::move_to_berth(int Blocks[Width][Width], std::vector<Berth*> &BerthList) {
    if(this->target_berth == nullptr) {
        Allocator allocator;
        std::pair<Berth*, Point> t = allocator.alloc_robot_berth(this, BerthList);
        this->target_berth = t.first;
        if(this->target_berth != nullptr) {
            this->target = Point(t.second.x, t.second.y);
            this->target_berth->space[this->target.x][this->target.y] = true;
        }

    }

    if(this->target_berth != nullptr) {
        if(path.size() == 0) {
            Point berth_point(this->target_berth->x + this->target.x, this->target_berth->y + this->target.y);
            if(this->x == berth_point.x && this->y == berth_point.y) {
                //到泊位点了，放货
                this->target_berth = nullptr;
                this->target = Point(-1, -1);
                std::cout << "pull " << this->id << std::endl;
                return -1;
            }


            if(this->generate_path(Blocks, berth_point) == -1) {
                //到不了这个泊位，重新选泊位
                Allocator allocator;
                Berth *cur = this->target_berth;
                Point cur_target(this->target.x, this->target.y);
                std::pair<Berth*, Point> t = allocator.alloc_robot_berth(this, BerthList);
                this->target_berth = t.first;
                this->target = t.second;
                this->target_berth->space[this->target.x][this->target.y] = true;
                cur->space[cur_target.x][cur_target.y] = false;
            }

        }

        int move = path[0];
        this->path.erase(path.begin());
        return move;
    }
    else {
        info("no target berth\n");
        return -1;
    }


}

void Robot::act(int Blocks[Width][Width], std::vector<Cargo*> &CargoList, std::vector<Berth*> &BerthList) {
    int dir;
    if(this->is_carring_cargo == false) {
        dir = this->move_to_cargo(Blocks, CargoList);
    }
    else {
        dir = this->move_to_berth(Blocks, BerthList);
    }
    if(dir != -1) {
        cout << "move " << this->id << " " << dir << endl;
//                info("move " + to_string(i) + " " + to_string(dir) + "\n");
//                info("pos: " + to_string(Robots[i].x) + " " + to_string(Robots[i].y) + "\n");
    }


}

