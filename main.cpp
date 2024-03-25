#include "Berth.h"
#include "Boat.h"
#include "Robot.h"
#include "Cargo.h"
#include "tool.h"

using namespace std;

int money, FrameId;

// 输入的字符地图
char ChMap[WIDTH][WIDTH];
// 网格图，0表示可通行，1表示障碍
int Blocks[WIDTH][WIDTH];

Robot Robots[ROBOT_NUM];
Boat Boats[BOAT_NUM];

vector<Berth *> BerthList;
set<Cargo *> CargoSet;
queue<Cargo *> CargoQueue;
map<int, int> frame2K;

int Boat::capacity = 0;
void Init()
{
    // 地图信息
    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            cin >> ChMap[i][j];
            // 初始化障碍图
            if (ChMap[i][j] == '.' || ChMap[i][j] == 'B' || ChMap[i][j] == 'A')
            {
                Blocks[i][j] = 0; // 无障碍
            }
            else
            {
                Blocks[i][j] = 1; // 有障碍
            }
        }
    }

    // 机器人初始化
    for (int i = 0; i < ROBOT_NUM; i++)
        Robots[i].id = i;

    // 泊位信息
    BerthList.resize(BERTH_NUM);
    for (int i = 0; i < BERTH_NUM; i++)
    {
        int id, x, y, transport_time, loading_speed;
        cin >> id >> x >> y >> transport_time >> loading_speed;
        BerthList[id] = new Berth(id, x, y, transport_time, loading_speed);
    }

    // 船只信息
    for (int i = 0; i < BOAT_NUM; i++)
        Boats[i].id = i;
    cin >> Boat::capacity;
    // OK
    string info_end;
    cin >> info_end;
    cout << "OK" << endl;
    ;
    fflush(stdout);
}

void Input()
{
    cin >> FrameId >> money;

    int K;
    cin >> K;
    if (K != 0)
        frame2K[FrameId] = K;

    // 处理失效的货物
    auto it = frame2K.begin();
    while (it != frame2K.end())
    {
        int frame = it->first;
        int K = it->second;
        if (frame <= FrameId - CARGO_LEFT_TIME)
        {
            for (int i = 0; i < K; i++)
            {
                Cargo *cargo = CargoQueue.front();
                CargoQueue.pop();
                CargoSet.erase(cargo);
                if (Robots[cargo->selected].is_carring_cargo)
                {
                    // 被选择这个货的机器人取到了就不删除
                    continue;
                }
                else
                {
                    delete cargo;
                }
            }
            it = frame2K.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // 新生成的货物信息
    vector<Cargo *> newCargoList;
    for (int i = 0; i < K; i++)
    {
        int x, y, val;
        cin >> x >> y >> val;
        auto *c = new Cargo(x, y, val);

        CargoSet.insert(c);
        CargoQueue.push(c);
        newCargoList.push_back(c);
    }

    // 输入机器人信息

    for (int i = 0; i < ROBOT_NUM; i++)
    {
        cin >> Robots[i].is_carring_cargo >> Robots[i].x >> Robots[i].y >> Robots[i].is_running;
        if (Robots[i].path.size() != 0 && Robots[i].path_index < Robots[i].path.size() - 1)
        {
            if (Robots[i].x == Robots[i].path[Robots[i].path_index + 1].x && Robots[i].y == Robots[i].path[Robots[i].path_index + 1].y)
                Robots[i].path_index++;
        }
    }

    // 泊位信息初始化
    for (int i = 0; i < BERTH_NUM; i++)
        BerthList[i]->selected_ship_num = 0;

    // 船信息
    for (int i = 0; i < BOAT_NUM; i++)
    {
        int state, target;
        cin >> state >> target;
        Boats[i].state = state;
        Boats[i].target = target;
        if (target != -1)
            BerthList[target]->selected_ship_num++;
    }

    string OK;
    cin >> OK;
}

void InitRobotState()
{
    // 初始化，找出哪些机器人是被困的
    for (int i = 0; i < ROBOT_NUM; i++)
    {
        Point berth_point(BerthList[i]->x, BerthList[i]->y);
        bool try_find = Robots[i].is_available(Blocks, berth_point);
        if (try_find == false)
        {
            BerthList[i]->select_failed_robots.insert(&Robots[i]);
            bool trapped = true;
            for (int j = 0; j < BERTH_NUM; j++)
            {
                if (j == i)
                    continue;
                Point berth(BerthList[j]->x, BerthList[j]->y);
                if (Robots[i].is_available(Blocks, berth))
                {
                    trapped = false;
                    break;
                }
                else
                    BerthList[j]->select_failed_robots.insert(&Robots[i]);
            }
            if (trapped)
                Robots[i].dead = true;
        }
    }
}

// 防止astar调用过多
int Search::count_a = 0;
int Search::count_b = 0;
int Search::count = 0;

int main()
{
    Init();
    for (int frame = 1; frame <= 15000; frame++)
    {
        Input();

        // 寻路次数初始化
        Search::count = 0;
        // 机器人初始化
        if (frame == 1)
            InitRobotState();

        // 机器人行动
        for (int i = 0; i < ROBOT_NUM; i++)
        {
            Robots[i].act(Blocks, CargoSet, BerthList, Robots);
        }

        // 船只行动
        for (int i = 0; i < BOAT_NUM; i++)
        {
            Boats[i].act(BerthList);
        }

        cout << "OK" << endl;
        fflush(stdout);
    }
    return 0;
}