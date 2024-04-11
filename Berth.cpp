#include "Berth.h"
#include "Boat.h"

Berth::Berth() {}
Berth::Berth(int id, int x, int y, int time, int velocity)
{
    this->id = id;
    this->x = x;
    this->y = y;
    this->time = time;
    this->velocity = velocity;
}

void Berth::take_in_cargo(int cargo_value)
{
    this->cargo_values.push(cargo_value);
    this->totalValues += cargo_value;
}

void Berth::give_boat(Boat *boat)
{
    int boat_left_space = Boat::capacity - boat->loaded_cargo_num;
    int load_num = std::min(this->velocity, boat_left_space);
    for (int i = 0; i < load_num && this->cargo_values.size(); i++)
    {
        // 一帧时间只能装velocity个货
        int v = this->cargo_values.front();
        this->cargo_values.pop();
        this->totalValues -= v;
        boat->loaded_cargo_num++;
    }
}