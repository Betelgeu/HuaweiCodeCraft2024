#include "Robot.h"

Robot::Robot() {}

Robot::Robot(int startX, int startY) {
    this->x = startX, this->y = startY;
}


//bool Robot::is_available(int Blocks[Width][Width], Point dest) {
//    Point start(this->x, this->y);
//    Search search;
//    std::vector<Point> path = search.Astar(Blocks, start, dest);
//
//    if(path.size() == 0)return false;
//    else return true;
//}

int Robot::generate_path(int Blocks[Width][Width], Point dest, Robot *RobotList){
    Point start(this->x, this->y);
    Search search;
//    std::vector<Point> path;
    std::vector<Point> path = search.Astar_robot_without_collision(Blocks, start, dest, RobotList, this->id);
    this->searched_time++;

    if(path.size() <= 1 || this->path.size() != 0) {
        info("robot generate path error!");
        return -1;
    }

    this->path = path;
    this->path_index = 0;

    return 0;
}

int Robot::move_to_cargo(int Blocks[Width][Width], std::set<Cargo*> &CargoSet, Robot *RobotList) {
    // 没有路径，选择货物
    if(this->target_cargo == nullptr && Search::count < MaxSearchTimePerFrame && this->searched_time <= 20) {
        Allocator allocator;
        this->target_cargo = allocator.alloc_robot_cargo(this, CargoSet);
        if(this->target_cargo != nullptr)this->target_cargo->selected = true;
    }


    // 已有路径，直接按照路径前往货物
    if(this->target_cargo != nullptr) {

        Point cargo(this->target_cargo->x, this->target_cargo->y);
        if(this->x == cargo.x && this->y == cargo.y) {
            //到货物处了，取货
//                this->target_cargo = nullptr;
            this->path.clear();
            this->path_index = -1;
            std::cout << "get " << this->id << std::endl;
            return -1;
        }

        if(this->path.size() == 0 &&  Search::count < MaxSearchTimePerFrame && this->searched_time <= 20) {
            if(this->generate_path(Blocks, cargo, RobotList) == -1) {
                //到不了这个货，重新选货
                Allocator allocator;
                Cargo* cur = this->target_cargo;
                this->target_cargo = allocator.alloc_robot_cargo(this, CargoSet);
                this->target_cargo = nullptr;
                this->move_to_cargo(Blocks, CargoSet, RobotList);
                cur->selected = false;
            }
        }

        if(path.size() <= 1)return -1;


        this->path_index++;
        Point cur = this->path[path_index], last = this->path[path_index - 1];
        if(cur.x == last.x && cur.y == last.y + 1)return 0;
        else if(cur.x == last.x && cur.y == last.y - 1)return 1;
        else if(cur.x == last.x - 1 && cur.y == last.y)return 2;
        else if(cur.x == last.x + 1 && cur.y == last.y)return 3;
        else {
            info("generated path incontinuous!");
            return -1;
        }
    }
    else {// 如果没有找到可达的货物，则返回一个特殊值表示未找到
        info("no target cargo\n");
        return -1;
    }
}

int Robot::move_to_berth(int Blocks[Width][Width], std::vector<Berth*> &BerthList, Robot *RobotList) {
    if(this->target_berth == nullptr) {
        Allocator allocator;
        std::pair<Berth*, Point> t = allocator.alloc_robot_berth(this, BerthList);
        this->target_berth = t.first;
        if(this->target_berth != nullptr) {
            this->berth_pos = Point(t.second.x, t.second.y);
            this->target_berth->space[this->berth_pos.x][this->berth_pos.y] = true;
        }

    }

    if(this->target_berth != nullptr) {
        Point berth_point(this->target_berth->x + this->berth_pos.x, this->target_berth->y + this->berth_pos.y);
        if(this->x == berth_point.x && this->y == berth_point.y) {
            //到泊位点了，放货
            this->target_berth->space[this->berth_pos.x][this->berth_pos.y] = false;
            this->target_berth->CargoNum++;
//            this->target_berth->CargoValue += this->target_cargo->val;

            this->target_cargo = nullptr;
            this->target_berth = nullptr;
            this->berth_pos = Point(-1, -1);
            this->path.clear();
            this->path_index = -1;

            std::cout << "pull " << this->id << std::endl;
            return -1;
        }



        if(path.size() == 0) {

            if(this->generate_path(Blocks, berth_point, RobotList) == -1) {
                //到不了这个泊位，重新选泊位
                Allocator allocator;
                Berth *cur = this->target_berth;
                Point cur_target(this->berth_pos.x, this->berth_pos.y);
                std::pair<Berth*, Point> t = allocator.alloc_robot_berth(this, BerthList);
                this->target_berth = t.first;
                this->berth_pos = t.second;
                this->target_berth->space[this->berth_pos.x][this->berth_pos.y] = true;
                cur->space[cur_target.x][cur_target.y] = false;
            }

        }

        if(this->path.size() <= 1)return -1;

        this->path_index++;
        Point cur = this->path[this->path_index], last = this->path[this->path_index - 1];
        if(cur.x == last.x && cur.y == last.y + 1)return 0;
        else if(cur.x == last.x && cur.y == last.y - 1)return 1;
        else if(cur.x == last.x - 1 && cur.y == last.y)return 2;
        else if(cur.x == last.x + 1 && cur.y == last.y)return 3;
        else {
            info("generated path incontinuous!");
            return -1;
        }
    }
    else {
        info("no target berth\n");
        return -1;
    }
}

void Robot::act(int Blocks[Width][Width], std::set<Cargo*> &CargoSet, std::vector<Berth*> &BerthList, Robot *RobotList) {


    int dir = -1;
    if(this->is_carring_cargo == false) {
        dir = this->move_to_cargo(Blocks, CargoSet, RobotList);
    }
    else {
        dir = this->move_to_berth(Blocks, BerthList, RobotList);
    }
    if(dir != -1) {
        cout << "move " << this->id << " " << dir << endl;
    }


}

