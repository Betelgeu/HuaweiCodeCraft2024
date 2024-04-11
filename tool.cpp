#include "tool.h"
#include "Cargo.h"
#include "Robot.h"
#include "Boat.h"

using namespace std;

void info(const std::string msg)
{
    return;
    const std::string file_name = "/Users/wuxiaojia/Documents/huawei/arch/log.txt";
    std::ofstream file;
    file.open(file_name, std::ios::app);

    if (file.is_open())
    {
        file << msg; // 输出到文件
        file.close();
    }
    else
    {
        std::cerr << "Error opening file: " << file_name << std::endl;
    }
}

bool isValid(int x, int y, int maze[WIDTH][WIDTH])
{
    return x >= 0 && x < WIDTH && y >= 0 && y < WIDTH && maze[x][y] == 0;
}

bool isDestination(int x, int y, const Node &dest)
{
    return x == dest.x && y == dest.y;
}

int calculateHValue(int x, int y, const Node &dest)
{
    return abs(x - dest.x) + abs(y - dest.y);
}

struct cmp
{
    bool operator()(const Node *a, const Node *b)
    {
        // 将value的值由小到大排列，形成Node的小根堆
        return a->h + a->g > b->h + b->g;
    }
};

std::vector<Point> Search::Astar(int maze[WIDTH][WIDTH], Point Start, Point Dest)
{
    count_a++;
    count++;

    Node start(Start.x, Start.y), dest(Dest.x, Dest.y);
    if (!isValid(start.x, start.y, maze) || !isValid(dest.x, dest.y, maze))
    {
        // 没找到路径
        info("Start or Destination is an obstacle\n");
        return std::vector<Point>();
    }

    start.g = 0;
    start.h = calculateHValue(start.x, start.y, dest);

    std::priority_queue<Node *, std::vector<Node *>, cmp> openList;
    vector<vector<bool>> closed(WIDTH, vector<bool>(WIDTH, false));
    openList.push(&start);
    while (!openList.empty())
    {
        Node *curNode = openList.top();
        openList.pop();

        int x = curNode->x;
        int y = curNode->y;
        if (closed[x][y])
            continue;
        closed[x][y] = true;

        if (isDestination(x, y, dest))
        {
            std::vector<Point> path;
            Node *node = curNode;
            while (node != nullptr)
            {
                Point n(node->x, node->y);
                path.emplace_back(n);
                node = node->parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // 上下左右4个方向
        std::vector<int> dx = {-1, 0, 1, 0};
        std::vector<int> dy = {0, 1, 0, -1};

        for (int i = 0; i < 4; i++)
        {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (isValid(newX, newY, maze) && closed[newX][newY] == false)
            {
                Node *child = new Node(newX, newY);
                // 启发函数
                child->g = curNode->g + 1;
                child->h = calculateHValue(newX, newY, dest);
                child->parent = curNode;
                openList.push(child);
            }
        }
    }

    return std::vector<Point>();
}

std::vector<Point> Search::Astar_robot_without_collision(int maze[WIDTH][WIDTH], Point s, Point d, Robot *RobotList, int robot_id)
{
    count_b++;
    count++;

    Node start(s.x, s.y), dest(d.x, d.y);
    if (!isValid(start.x, start.y, maze) || !isValid(dest.x, dest.y, maze))
    {
        // 没找到路径
        info("Start or Destination is an obstacle\n");
        return std::vector<Point>();
    }

    start.g = 0;
    start.h = calculateHValue(start.x, start.y, dest);

    std::priority_queue<Node *, std::vector<Node *>, cmp> openList;
    vector<vector<bool>> closed(WIDTH, vector<bool>(WIDTH, false));
    openList.push(&start);
    vector<vector<int>> a(WIDTH, vector<int>(WIDTH, 1));
    while (!openList.empty())
    {
        Node *curNode = openList.top();
        openList.pop();

        int x = curNode->x;
        int y = curNode->y;

        if (closed[x][y])
            continue;
        closed[x][y] = true;

        if (isDestination(x, y, dest))
        {
            std::vector<Point> path;
            Node *node = curNode;
            while (node != nullptr)
            {
                Point n(node->x, node->y);
                path.emplace_back(n);
                node = node->parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // 上下左右4个方向
        std::vector<int> dx = {-1, 0, 1, 0};
        std::vector<int> dy = {0, 1, 0, -1};

        for (int i = 0; i < 4; i++)
        {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (isValid(newX, newY, maze) && closed[newX][newY] == false)
            {
                Node *child = new Node(newX, newY);
                // 启发函数
                child->g = curNode->g + 1;

                // 碰撞避免
                int g = child->g;
                bool valid_child = true;
                for (int j = 0; j < ROBOT_NUM; j++)
                {
                    if (j == robot_id)
                        continue;

                    if (RobotList[j].path.size())
                    {
                        if (RobotList[j].path.size() - RobotList[j].path_index > g)
                        {
                            // 生成的点和这个时刻已经生成路径的机器人相撞
                            if (RobotList[j].path[RobotList[j].path_index + g].x == newX &&
                                RobotList[j].path[RobotList[j].path_index + g].y == newY)
                            {
                                valid_child = false;
                                break;
                            }
                            if (RobotList[j].path[RobotList[j].path_index + g + 1].x == newX &&
                                RobotList[j].path[RobotList[j].path_index + g + 1].y == newY)
                            {
                                // 生成的点和机器人下一个路径相撞(对撞)
                                valid_child = false;
                                break;
                            }
                        }
                        else if (RobotList[j].path.size() - RobotList[j].path_index > g - 2)
                        {
                            // 对生成的点的时刻已经走完的点，假设它停在了终点
                            if (RobotList[j].path[RobotList[j].path.size() - 1].x == newX &&
                                RobotList[j].path[RobotList[j].path.size() - 1].y == newY)
                            {
                                valid_child = false;
                                break;
                            }
                        }
                    }
                    if (RobotList[j].path.size() <= 1 || RobotList[j].path_index == RobotList[j].path.size() - 1 || RobotList[j].is_running == false)
                    {
                        // 对于没有路径的机器人，假设它会停在原地
                        if (newX == RobotList[j].x &&
                            newY == RobotList[j].y)
                        {
                            valid_child = false;
                            break;
                        }
                    }
                }
                if (valid_child == false)
                    continue;

                // 将子节点添加到openList
                child->h = calculateHValue(newX, newY, dest);
                child->parent = curNode;
                openList.push(child);
            }
        }
    }

    return std::vector<Point>();
}

Cargo *Allocator::alloc_robot_cargo(Robot *robot, std::set<Cargo *> &CargoSet, int maze[WIDTH][WIDTH], Robot *RobotList)
{
    // 从未被选择的路径中，选一个可达的路径
    Cargo *cargo_max_value = nullptr;
    double max_value = 0;
    for (auto cargo : CargoSet)
    {
        if (cargo->selected == -1 && cargo->select_failed_robots.count(robot) == 0)
        {
            int dist = abs(cargo->x - robot->x) + abs(cargo->y - robot->y);
            // ***调参1:cargo对于robot的价值函数***
            double value = double(cargo->val) / (dist * 2);
            if (value > max_value)
            {
                max_value = value;
                cargo_max_value = cargo;
            }
        }
    }

    if (cargo_max_value != nullptr)
    {
        Point cargo_point(cargo_max_value->x, cargo_max_value->y);
        int cargo_available = robot->generate_path(maze, cargo_point, RobotList);
        if (cargo_available == -1)
        {
            cargo_max_value->select_failed_robots.insert(robot);
            return nullptr;
        }
        else
        {
            cargo_max_value->selected = robot->id;
            return cargo_max_value;
        }
    }
    else
        return nullptr;
}

std::pair<Berth *, Point> Allocator::alloc_robot_berth(Robot *robot, std::vector<Berth *> &BerthList, int maze[WIDTH][WIDTH], Robot *RobotList)
{
    // 返回要放的泊位和放回点的坐标
    Berth *target_berth = nullptr;
    Point target_berth_pos;
    target_berth_pos.x = 3, target_berth_pos.y = 0;
    Point null_pos(-1, -1);
    // 遍历寻找最大价值的货物
    double max_value = 0;
    for (int i = 0; i < BERTH_NUM; i++)
    {
        Berth *berth = BerthList[i];
        if (berth->select_failed_robots.count(robot) == 0)
        {
            // ***调参2: berth对robot的价值***
            // 这里价值 = 1 / 预估最小路径长度
            int dist = abs(robot->x - berth->x) + abs(robot->y - berth->y);
            double value = double(1) / dist;

            if (value > max_value)
            {
                target_berth = berth;
                max_value = value;
            }
        }
    }

    if (target_berth != nullptr)
    {
        Point target(target_berth->x + target_berth_pos.x, target_berth->y + target_berth_pos.y);
        int berth_available = robot->generate_path(maze, target, RobotList);
        if (berth_available == -1)
        {
            target_berth->select_failed_robots.insert(robot);
            return {nullptr, null_pos};
        }
        else
        {
            return {target_berth, target_berth_pos};
        }
    }
    else
        return {nullptr, null_pos};
}

int calculateRemainingSum(std::queue<int> q, int n)
{
    // 队列少于n个元素，剩余大小总和为0
    if (q.size() == 0 || q.size() < n)
        return 0;
    // 将前n个元素出队
    for (int i = 0; i < n; ++i)
        q.pop();

    int remainingSum = 0;
    // 处理队列中剩余元素
    while (!q.empty())
    {
        remainingSum += q.front();
        q.pop();
    }
    return remainingSum;
}

Berth *Allocator::alloc_boat_berth(Boat *boat, std::vector<Berth *> &BerthList)
{
    int maxLeftValue = 0;
    Berth *best_berth = nullptr;
    for (auto *berth : BerthList)
    {
        // ***调参3: berth对于boat的价值***
        // 这里也可以读berth的其他各种参数来计算价值
        int left_value = calculateRemainingSum(berth->cargo_values, berth->selected_ship_num * Boat::capacity);
        if (left_value > maxLeftValue)
            best_berth = berth;
    }
    if (best_berth == nullptr)
        return BerthList[boat->id * 2];
    else
        return best_berth;
}
