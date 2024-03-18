#include "Berth.h"

Berth::Berth() {}
Berth::Berth(int x, int y, int transport_time, int loading_speed) {
    this->x = x;
    this->y = y;
    this->transport_time = transport_time;
    this->loading_speed = loading_speed;
}
void Berth::flush () {
    if(CargoNum == 8)is_full = true;
}