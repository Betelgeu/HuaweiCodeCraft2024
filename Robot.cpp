#include "Robot.h"

Robot::Robot() {}

Robot::Robot(int startX, int startY) {
    this->x = startX, this->y = startY;
}


bool Robot::is_available(int Blocks[Width][Width], Point dest, Robot* RobotList, Robot *robot) {
    Point start(this->x, this->y);
    Search search;
    std::vector<Point> path = search.Astar_robot_without_collision(Blocks, start, dest, RobotList, this->id);

    if(path.size() == 0)return false;
    else return true;
}

int Robot::generate_path(int Blocks[Width][Width], Point dest, Robot *RobotList){
    Point start(this->x, this->y);
    Search search;
//    std::vector<Point> path;
    std::vector<Point> path = search.Astar_robot_without_collision(Blocks, start, dest, RobotList, this->id);

    if(path.size() == 0 || this->path.size() != 0) {
        info("robot generate path error!");
        this->searched_fail_time++;
        return -1;
    }

    this->path = path;
    this->path_index = 0;

    return 0;
}

int Robot::move_to_cargo(int Blocks[Width][Width], std::set<Cargo*> &CargoSet, Robot *RobotList) {
    // 没有路径，尝试选择货物 alloc分配时会计算路径，保证可达
    if(this->searched_fail_time <= 20) {
        if(this->target_cargo == nullptr || isValid(this->target_cargo->x, this->target_cargo->y, Blocks) == false) {
            Allocator allocator;
            this->target_cargo = allocator.alloc_robot_cargo(this, CargoSet, Blocks, RobotList);
        }
    }


    // 如果成功选上 or 已有货物
    if(this->target_cargo != nullptr) {
        if(this->path.size() <= 1)return -1;

        Point cur = this->path[path_index + 1], last = this->path[path_index];
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
    //没有目标，则分配目标泊位，同时寻路
    if(this->target_berth == nullptr && this->searched_fail_time <= 20) {
        Allocator allocator;
        std::pair<Berth*, Point> t = allocator.alloc_robot_berth(this, BerthList, Blocks, RobotList);
        this->target_berth = t.first;
        this->berth_pos = Point(t.second.x, t.second.y);
    }


    //有目标
    if(this->target_berth != nullptr) {
        if(this->path.size() <= 1)return -1;

        Point cur = this->path[this->path_index + 1], last = this->path[this->path_index];
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

    if(this->is_running) {
        if(this->is_carring_cargo == false) {
            int dir = this->move_to_cargo(Blocks, CargoSet, RobotList);
            if(dir != -1)cout << "move " << this->id << " " << dir << endl;

            if(this->path_index == this->path.size() - 1) {//到货物 取货
                this->path.clear();
                this->path_index = 0;
                std::cout << "get " << this->id << std::endl;
            }
        }
        if(this->is_carring_cargo == true) {
            int dir = this->move_to_berth(Blocks, BerthList, RobotList);
            if(dir != -1)cout << "move " << this->id << " " << dir << endl;

            if(this->path_index == this->path.size() - 1) {//到泊位 卸货
                this->target_cargo = nullptr;
                this->target_berth = nullptr;
                this->berth_pos = Point(-1, -1);

                this->path.clear();
                this->path_index = -1;
                std::cout << "pull " << this->id << std::endl;
            }
        }
    }
    else {
        //
    }

}

