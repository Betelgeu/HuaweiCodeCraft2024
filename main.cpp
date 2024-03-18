#include "Berth.h"
#include "Boat.h"
#include "Robot.h"
#include "Cargo.h"
#include "tool.h"

using namespace std;

int BoatCapacity, money, FrameId;
int CargoNum = 0;

char ChMap[Width][Width];//输入的字符地图
int Blocks[Width][Width];//网格图，0表示可通行，1表示障碍

Robot Robots[RobotNum];
Boat Boats[BoatNum];
Berth BerthList[BerthNum];

vector<Cargo*> CargoList;


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
    for (int i = 0; i < BerthNum; i++){
        int id;
        cin >> id;
        cin >> BerthList[id].x >> BerthList[id].y >> BerthList[id].transport_time >> BerthList[id].loading_speed;
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
    int K;
    cin >> K;
    CargoNum += K;
    info("FrameId: " + to_string(FrameId) + "\n");
    for (int i = 0; i < K; i++)
    {
        int x, y, val;
        cin >> x >> y >> val;
        auto *c = new Cargo(x, y, val);
        CargoList.push_back(c);
    }

    //输入机器人信息

    for (int i = 0; i < RobotNum; i++)
    {
        cin >> Robots[i].is_carring_cargo >> Robots[i].x >> Robots[i].y >> Robots[i].is_running;
    }

    // 船信息
    for (int i = 0; i < BoatNum; i++) {
        cin >> Boats[i].state >> Boats[i].pos;
    }

    string OK;
    cin >> OK;
}

int main() {
    Init();
    for (int frame = 1; frame <= 15000; frame++)
    {
        Input();

        for(int i = 0; i < RobotNum; i ++) {
            info("robot" + to_string(i) + "\n");
            info(to_string(Robots[i].x) + " " + to_string(Robots[i].y) + "\n");
            int dir = Robots[i].move_to_cargo(Blocks, CargoList);
            if(dir != -1) {
                cout << "move " << i << " " << dir << endl;
                info("move " + to_string(i) + " " + to_string(dir) + "\n");
            }
        }

        cout << "OK" << endl;
        fflush(stdout);
    }
	return 0;
}