#include "Robot.h"

Robot::Robot() {}

Robot::Robot(int startX, int startY)
{
    this->x = startX, this->y = startY;
}

bool Robot::is_available(int Blocks[WIDTH][WIDTH], Point dest)
{
    Point start(this->x, this->y);
    Search search;
    std::vector<Point> path = search.Astar(Blocks, start, dest);

    if (path.size() == 0)
        return false;
    else
        return true;
}

int Robot::generate_path(int Blocks[WIDTH][WIDTH], Point dest, Robot *RobotList)
{
    Point start(this->x, this->y);
    Search search;
    std::vector<Point> path = search.Astar_robot_without_collision(Blocks, start, dest, RobotList, this->id);

    if (path.size() == 0)
    {
        info("robot generate path error!");
        return -1;
    }

    this->path = path;
    this->path_index = 0;

    return 0;
}

int Robot::move_to_cargo(int Blocks[WIDTH][WIDTH], std::set<Cargo *> &CargoSet, Robot *RobotList)
{
    // 没有路径，尝试选择货物 alloc分配时会计算路径，保证可达
    if (this->target_cargo == nullptr || isValid(this->target_cargo->x, this->target_cargo->y, Blocks) == false)
    {
        Allocator allocator;
        this->target_cargo = allocator.alloc_robot_cargo(this, CargoSet, Blocks, RobotList);
    }

    if (this->target_cargo != nullptr && this->path.size() == 0)
    {
    }

    // 如果成功选上 or 已有货物
    if (this->target_cargo != nullptr)
    {
        if (this->path.size() <= 1 || this->path_index == this->path.size() - 1)
            return -1;

        Point cur = this->path[path_index + 1], last = this->path[path_index];
        if (cur.x == last.x && cur.y == last.y + 1)
            return 0;
        else if (cur.x == last.x && cur.y == last.y - 1)
            return 1;
        else if (cur.x == last.x - 1 && cur.y == last.y)
            return 2;
        else if (cur.x == last.x + 1 && cur.y == last.y)
            return 3;
        else
        {
            info("generated path incontinuous!");
            return -1;
        }
    }
    else
    {
        // 如果没有找到可达的货物，则返回一个特殊值表示未找到
        info("no target cargo\n");
        return -1;
    }
}

int Robot::move_to_berth(int Blocks[WIDTH][WIDTH], std::vector<Berth *> &BerthList, Robot *RobotList)
{
    // 没有目标，则分配目标泊位，同时寻路
    if (this->target_berth == nullptr)
    {
        Allocator allocator;
        std::pair<Berth *, Point> t = allocator.alloc_robot_berth(this, BerthList, Blocks, RobotList);
        this->target_berth = t.first;
        this->berth_pos = Point(t.second.x, t.second.y);
    }

    if (this->target_berth != nullptr)
    {
        // 分配有目标, allocator保证必然是可达的，也就必然有路线
        if (this->path.size() <= 1 || this->path_index == this->path.size() - 1)
            return -1;
        // 有路线，按路线走
        Point cur = this->path[this->path_index + 1], last = this->path[this->path_index];
        if (cur.x == last.x && cur.y == last.y + 1)
            return 0;
        else if (cur.x == last.x && cur.y == last.y - 1)
            return 1;
        else if (cur.x == last.x - 1 && cur.y == last.y)
            return 2;
        else if (cur.x == last.x + 1 && cur.y == last.y)
            return 3;
        else
        {
            info("generated path incontinuous!");
            return -1;
        }
    }
    else
    { // 没有分配到目标，下次继续分配
        info("no target berth\n");
        return -1;
    }
}

void Robot::act(int Blocks[WIDTH][WIDTH], std::set<Cargo *> &CargoSet, std::vector<Berth *> &BerthList, Robot *RobotList)
{
    // 只操作正在运行的机器人
    if (this->is_running && this->dead == false)
    {

        // 移动前动作
        if (this->path.size() && this->path_index == this->path.size() - 1)
        {
            this->path.clear();
            this->path_index = -1;
            if (this->is_carring_cargo == false)
            {
                std::cout << "get " << this->id << std::endl;
                this->is_carring_cargo = true;
            }
            else
            {
                this->target_berth->take_in_cargo(this->target_cargo->val);

                this->target_cargo = nullptr;
                this->target_berth = nullptr;
                this->berth_pos = Point(-1, -1);
                std::cout << "pull " << this->id << std::endl;
                this->is_carring_cargo = false;
            }
        }

        // 移动动作
        if (this->is_carring_cargo == false)
        {
            int dir = this->move_to_cargo(Blocks, CargoSet, RobotList);
            if (dir != -1)
                cout << "move " << this->id << " " << dir << endl;
        }
        else
        {
            int dir = this->move_to_berth(Blocks, BerthList, RobotList);
            if (dir != -1)
                cout << "move " << this->id << " " << dir << endl;
        }

        // 移动后动作
        if (this->path.size() && this->path_index == this->path.size() - 1)
        {
            this->path.clear();
            this->path_index = -1;
            if (this->is_carring_cargo == false)
            {
                std::cout << "get " << this->id << std::endl;
            }
            else
            {
                this->target_berth->take_in_cargo(this->target_cargo->val);

                this->target_cargo = nullptr;
                this->target_berth = nullptr;
                this->berth_pos = Point(-1, -1);
                std::cout << "pull " << this->id << std::endl;
            }
        }
    }
    /*
    // 将没在运行的机器人的路径置空，方面后续重新生成路径，从碰撞中恢复
        else {
            if(this->path_index != -1) {
                if(this->is_carring_cargo == false) {
                    this->target_cargo = nullptr;
                    this->path.clear();
                    this->path_index = -1;
                }
                else {
                    this->target_berth = nullptr;
                    this->path.clear();
                    this->path_index = -1;
                }
            }
        }
    */
}
