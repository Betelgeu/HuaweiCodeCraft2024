#include "Berth.h"
#include "Boat.h"
#include "Robot.h"
#include "Cargo.h"
#include "tool.h"

using namespace std;

int BoatCapacity, money, FrameId;

char ChMap[Width][Width];//输入的字符地图
int Blocks[Width][Width];//网格图，0表示可通行，1表示障碍

Robot Robots[RobotNum];
Boat Boats[BoatNum];
int BerthFrame[BoatNum];

vector<Berth*> BerthList;
set<Cargo*> CargoSet;
queue<Cargo*> CargoQueue;
map<int, int> frame2K;
vector<double> Berth_weight;   //new
Allocator allo;

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
                Blocks[i][j]=1;//有障碍
            }
        }
    }

    //机器人初始化
    for(int i = 0; i < RobotNum; i++)
        Robots[i].id = i;

    // 港口信息
    BerthList.resize(BerthNum);
    for (int i = 0; i < BerthNum; i++){
        int id;
        cin >> id;
        int x, y, transport_time, loading_speed;
        cin >> x >> y >> transport_time >> loading_speed;
        BerthList[id] = new Berth(x, y, transport_time, loading_speed);
    }
    // 船只容量
    cin >> BoatCapacity;
    // OK
    string info_end;
    cin >> info_end;
    cout << "OK" << endl;;
    fflush(stdout);
}

void Input()
{
    cin >> FrameId >> money;
    cout << "cinFrame: " << FrameId << endl;
    int K;
    cin >> K;
    if(K != 0)frame2K[FrameId] = K;

//    if(Robots[0].path_index == Robots[0].path.size() - 2) {
//
//    }

    if(FrameId == 206) {

    }


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
                if (!cargo->selected) {
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
    }

    //输入机器人信息

    for (int i = 0; i < RobotNum; i++)
    {
        cin >> Robots[i].is_carring_cargo >> Robots[i].x >> Robots[i].y >> Robots[i].is_running;
        if(Robots[i].path.size() != 0) {
            if(Robots[i].x == Robots[i].path[Robots[i].path_index + 1].x && Robots[i].y == Robots[i].path[Robots[i].path_index + 1].y)
                Robots[i].path_index++;
        }
    }

    // 船信息
    for (int i = 0; i < BoatNum; i++) {
        cin >> Boats[i].state >> Boats[i].pos;
    }

    string OK;
    cin >> OK;
}

int Search::count_a = 0;
int Search::count_b = 0;
int Search::count = 0;
int main() {
    Init();
    for (int frame = 1; frame <= 15000; frame++)
    {
        Search::count = 0;
        cout << "program_frame: " << frame << endl;
        Input();

//        Robots[0].act(Blocks, CargoSet, BerthList, Robots);
        for(int i = 0; i < RobotNum; i ++) {
            Robots[i].act(Blocks, CargoSet, BerthList, Robots);
        }
        Berth_weight = allo.Berth_w(BerthList);
        for(int i = 0; i < BoatNum; i ++) {
            Boats[i].action(BerthList, CargoSet, Berth_weight, frame, BerthFrame, BoatCapacity, i);
        }
//        for(int i = 0; i < RobotNum; i++) {
//            if(Robots[i].path.size() != 0)Robots[i].path_index++;
//        }

//        Allocator alloc;
//        vector<double> Ber_weight = alloc.Berth_w(BerthList);
//        for (int i = 0; i < BoatNum; i++) {
//            Boats[i].action(BerthList, Ber_weight, frame, BerthFrame,BoatCapacity, i);
//        }

//        cout << Search::count_a << " " << Search::count_b << endl;
        cout << "OK" << endl;
        fflush(stdout);
    }
	return 0;
}