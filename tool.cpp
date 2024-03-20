#include "tool.h"
#include "Cargo.h"
#include "Robot.h"

void info(const std::string msg) {
    return;
    const std::string file_name = "/Users/wuxiaojia/Documents/huawei/arch/log.txt";
    std::ofstream file;
    file.open(file_name, std::ios::app);

    if (file.is_open()) {
        file << msg;  // 输出到文件
        file.close();
    } else {
        std::cerr << "Error opening file: " << file_name << std::endl;
    }
}

bool isValid(int x, int y, int maze[Width][Width]) {
    return x >= 0 && x < Width && y >= 0 && y < Width && maze[x][y] == 0;
}

bool isDestination(int x, int y, const Node& dest) {
    return x == dest.x && y == dest.y;
}

int calculateHValue(int x, int y, const Node& dest) {
//    return std::sqrt((x - dest.x) * (x - dest.x) + (y - dest.y) * (y - dest.y));
    return abs(x - dest.x) + abs(y - dest.y);
}

struct cmp{
    bool operator ()(const Node* a, const Node* b)
    {
        return a->h + a->g > b->h + b->g;//将value的值由小到大排列，形成Node的小根堆
    }
};

std::vector<Point> Search::Astar(int maze[Width][Width], Point Start, Point Dest) {
    count_a++;
    count++;

    Node start(Start.x, Start.y), dest(Dest.x, Dest.y);
    if (!isValid(start.x, start.y, maze) || !isValid(dest.x, dest.y, maze)) {
        //没找到路径
        info("Start or Destination is an obstacle\n");
        return std::vector<Point>();
    }

    start.g = 0;
    start.h = calculateHValue(start.x, start.y, dest);

    std::priority_queue<Node*, std::vector<Node*>, cmp> openList;
    vector<vector<bool>> closed(Width, vector<bool>(Width, false));
    openList.push(&start);
    while (!openList.empty()) {
        Node *curNode = openList.top();
        openList.pop();

        int x = curNode->x;
        int y = curNode->y;
        if(closed[x][y])continue;
        closed[x][y] = true;

        if (isDestination(x, y, dest)) {
//            cout << "openlist.size: " << openList.size() << endl;
            std::vector<Point> path;
            Node* node = curNode;
            while (node != nullptr) {
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

        for (int i = 0; i < 4; i++) {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (isValid(newX, newY, maze) && closed[newX][newY] == false) {
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

std::vector<Point> Search::Astar_robot_without_collision(int maze[Width][Width], Point s, Point d, Robot *RobotList, int robot_id) {
    count_b++;
    count++;

    Node start(s.x, s.y), dest(d.x, d.y);
    if (!isValid(start.x, start.y, maze) || !isValid(dest.x, dest.y, maze)) {
        //没找到路径
        info("Start or Destination is an obstacle\n");
        return std::vector<Point>();
    }

    start.g = 0;
    start.h = calculateHValue(start.x, start.y, dest);

    std::priority_queue<Node*, std::vector<Node*>, cmp> openList;
    vector<vector<bool>> closed(Width, vector<bool>(Width, false));
    openList.push(&start);
    vector<vector<int>> a(Width, vector<int>(Width, 1));
    while (!openList.empty()) {
        Node *curNode = openList.top();
        openList.pop();

        int x = curNode->x;
        int y = curNode->y;
        if(closed[x][y])continue;
        closed[x][y] = true;
//        info("open cur: " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(curNode->g + curNode->h) + "\n");

        if (isDestination(x, y, dest)) {
            std::vector<Point> path;
            Node* node = curNode;
            vector<int> test;
            while (node != nullptr) {
                Point n(node->x, node->y);
                path.emplace_back(n);
                test.push_back(node->g);
                node = node->parent;
            }
            test.push_back(0);
//            for(auto x : test)cout << x << " ";
//            cout << endl;
            reverse(path.begin(), path.end());
            return path;
        }

        // 上下左右4个方向
        std::vector<int> dx = {-1, 0, 1, 0};
        std::vector<int> dy = {0, 1, 0, -1};

        for (int i = 0; i < 4; i++) {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (isValid(newX, newY, maze) && closed[newX][newY] == false) {
                Node *child = new Node(newX, newY);
                // 启发函数
                child->g = curNode->g + 1;

                // 碰撞避免
                int g = child->g;
                bool valid_child = true;
                for(int i = 0; i < RobotNum && i != robot_id; i++) {
                    if(RobotList[i].path.size() - RobotList[i].path_index > g) {
                        if(RobotList[i].path[RobotList[i].path_index + g].x == newX && RobotList[i].path[RobotList[i].path_index + g].y == newY) {
                            valid_child = false;
                            break;
                        }
                    }
                }
                if(valid_child == false)continue;

                //将子节点添加到openList
                child->h = calculateHValue(newX, newY, dest);
                child->parent = curNode;
                openList.push(child);
            }
        }
    }

    return std::vector<Point>();
}



Cargo* Allocator::alloc_robot_cargo(Robot *robot, std::set<Cargo*> &CargoSet) {
    Cargo *cargo_max_value = nullptr;
    double max_value = 0;
    for(auto cargo : CargoSet) {
        if(cargo->selected == false) {
            int dist = abs(cargo->x - robot->x) + abs(cargo->y - robot->y);
            double value = double(cargo->val) / dist;
            if(value > max_value) {
                max_value = value;
                cargo_max_value = cargo;
            }
        }
    }
    return cargo_max_value;
}

std::pair<Berth*, Point> Allocator::alloc_robot_berth(Robot *robot, std::vector<Berth*> &BerthList) {
    // 放回放回点的坐标
    for(int i = 0; i < BerthNum; i++) {
        Berth *berth = BerthList[i];
        if(berth->is_full == false) {
            for(int x = 0; x < 4; x++) {
                for(int y = 0; y < 4; y++) {
                    if(berth->space[x][y] == false) {
                        Point target(x, y);
                        return {berth, target};
                    }
                }
            }
        }
    }
    Point target(-1, -1);
    return {nullptr, target};
}

