#include "tool.h"
#include "Cargo.h"

void info(const std::string msg) {
//    return;
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

double calculateHValue(int x, int y, const Node& dest) {
//    return std::sqrt((x - dest.x) * (x - dest.x) + (y - dest.y) * (y - dest.y));
    return abs(x - dest.x) + abs(y - dest.y);
}

struct cmp{
    bool operator ()(const Node* a, const Node* b)
    {
        return a->h + a->g > b->h + b->g;//将value的值由小到大排列，形成Node的小根堆
    }
};

std::vector<Node> Search::Astar(int maze[Width][Width], std::pair<int, int> Start, std::pair<int, int> Dest) {
    Node start(Start.first, Start.second), dest(Dest.first, Dest.second);
    if (!isValid(start.x, start.y, maze) || !isValid(dest.x, dest.y, maze)) {
        //没找到路径
        info("Start or Destination is an obstacle\n");
        return std::vector<Node>();
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
            std::vector<Node> path;
            Node* node = curNode;
            while (node != nullptr) {
                Node n(node->x, node->y);
                path.emplace_back(n);
                node = node->parent;
            }
            std::reverse(path.begin(), path.end());
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

    return std::vector<Node>();
}


