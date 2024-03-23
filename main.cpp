#include "Berth.h"
#include "Boat.h"
#include "Robot.h"
#include "Cargo.h"
#include "tool.h"

using namespace std;

int money, FrameId;

char ChMap[Width][Width];//输入的字符地图
int Blocks[Width][Width];//网格图，0表示可通行，1表示障碍

Robot Robots[RobotNum];
Boat Boats[BoatNum];

vector<Berth*> BerthList;
set<Cargo*> CargoSet;
queue<Cargo*> CargoQueue;
map<pair<int, int> , Cargo*> CargoMap;
map<pair<int, int> , Berth*> BerthMap;
map<int, int> frame2K;

int Allocator::block_count = 0;
int Allocator::map_case = -1;
int Boat::capacity = 0;
//防止astar调用过多
int Search::count_a = 0;
int Search::count_b = 0;
int Search::count = 0;
int Allocator::total_cargo_num = 0;
int Allocator::total_cargo_dist = 0;
int Robot::totalPickedCargoNum = 0;
int Robot::totalPickedCargoValue = 0;

void Init()
{
    // 地图信息
    for (int i = 0; i < Width; i++) {
        for (int j = 0; j < Width; j++) {
            cin >> ChMap[i][j];//[x][y]
            //初始化障碍图
            if (ChMap[i][j] == '.' || ChMap[i][j] == 'B' || ChMap[i][j] == 'A') {
                Blocks[i][j]=0;//无障碍
            }
            else {
                if(ChMap[i][j] == '#')Allocator::block_count++;;
                Blocks[i][j]=1;//有障碍
            }
        }
    }
    if(Allocator::block_count < 200)Allocator::map_case = 2;
    else Allocator::map_case = 1;
    Allocator::map_case = 1;

    // 机器人初始化
    for(int i = 0; i < RobotNum; i++)
        Robots[i].id = i;


    // 泊位信息
    BerthList.resize(BerthNum);
    for (int i = 0; i < BerthNum; i++){
        int id, x, y, transport_time, loading_speed;
        cin >> id >> x >> y >> transport_time >> loading_speed;
        BerthList[id] = new Berth(id, x, y, transport_time, loading_speed);
        BerthMap[make_pair(x, y)] = BerthList[id];
    }


    // 船只信息
    for (int i = 0; i < BoatNum; i++)Boats[i].id = i;
    cin >> Boat::capacity;
    // OK
    string info_end;
    cin >> info_end;
    cout << "OK" << endl;;
    fflush(stdout);
}

void Input()
{
    cin >> FrameId >> money;

    int K;
    cin >> K;
    if(K != 0)frame2K[FrameId] = K;

//    if(FrameId == 9000) {
//
//    }

    //处理失效的货物
    auto it = frame2K.begin();
    while (it != frame2K.end()) {
        int frame = it->first;
        int K = it->second;
        if (frame <= FrameId - 1000) {
            for (int i = 0; i < K; i++) {
                Cargo* cargo = CargoQueue.front();
                CargoQueue.pop();
                CargoSet.erase(cargo);
                pair<int, int> pa = make_pair(cargo->x, cargo->y);
                CargoMap.erase(pa);
                if (Robots[cargo->selected].is_carring_cargo) { // 被选择这个货的机器人取到了就不删除
                    continue;
                }
                else {
                    delete cargo;
                }
            }
            it = frame2K.erase(it); // 删除满足条件的元素，并返回下一个有效的迭代器
        } else {
            ++it; // 继续迭代
        }
    }


    // 新生成的货物信息
    vector<Cargo*> newCargoList;
    for (int i = 0; i < K; i++)
    {
        int x, y, val;
        cin >> x >> y >> val;
        auto *c = new Cargo(x, y, val);

        CargoSet.insert(c);
        CargoQueue.push(c);
        newCargoList.push_back(c);
        pair<int, int> pa = make_pair(x, y);
        CargoMap[pa] = c;
    }

    //输入机器人信息

    for (int i = 0; i < RobotNum; i++)
    {
        cin >> Robots[i].is_carring_cargo >> Robots[i].x >> Robots[i].y >> Robots[i].is_running;
        if(Robots[i].path.size()  && Robots[i].path_index + 1 < Robots[i].path.size()) {
            if(Robots[i].x == Robots[i].path[Robots[i].path_index + 1].x && Robots[i].y == Robots[i].path[Robots[i].path_index + 1].y)
                Robots[i].path_index++;
        }
    }

    // 泊位信息初始化
    for(int i = 0; i < BerthNum; i++)
        BerthList[i]->selected_ship_num = 0;

    // 船信息
    for (int i = 0; i < BoatNum; i++) {
        int state, target;
        cin >> state >> target;
        Boats[i].state = state;
        Boats[i].target = target;
        if(target != -1)BerthList[target]->selected_ship_num++;
    }

    string OK;
    cin >> OK;
}

void InitRobotState() {
    //初始化，找出哪些机器人是被困的
    for(int i = 0; i < RobotNum; i++) {
        Point berth_point(BerthList[i]->x, BerthList[i]->y);
        bool try_find = Robots[i].is_available(Blocks, berth_point);
        if(try_find == false) {
            BerthList[i]->select_failed_robots.insert(&Robots[i]);
            bool trapped = true;
            for(int j = 0; j < BerthNum; j++) {
                if(j == i)continue;
                Point berth(BerthList[j]->x, BerthList[j]->y);
                if(Robots[i].is_available(Blocks, berth)) {
                    trapped = false;
                    break;
                }
                else BerthList[j]->select_failed_robots.insert(&Robots[i]);
            }
            if(trapped)Robots[i].dead = true;
        }
    }
}

int main() {
    Init();
    for (int frame = 1; frame <= 15000; frame++)
    {

//        cout << "program_frame: " << frame << endl;
        Input();

        if(frame == 1)InitRobotState();
        Search::count_a = 0;
        Search::count_b = 0;
        Search::count = 0;

        for(int i = 0; i < RobotNum; i++) {
            Robots[i].act(Blocks, CargoSet, BerthList, Robots, CargoMap, BerthMap);
        }

        //船只行动
        for(int i = 0; i < BoatNum; i++) {
            Boats[i].act(BerthList, FrameId);
        }

        cout << "OK" << endl;
        fflush(stdout);
    }

//    cout << Allocator::total_cargo_dist / Allocator::total_cargo_num << endl;
	return 0;
}