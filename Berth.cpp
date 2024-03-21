#include "Berth.h"

Berth::Berth() {}
Berth::Berth(int x, int y, int transport_time, int loading_speed) {
    this->x = x;
    this->y = y;
    this->transport_time = transport_time;
    this->loading_speed = loading_speed;
    this->ship_select = false;
    this->CargoNum = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->selected[i][j].first.first = x + i;
            this->selected[i][j].first.second = y + j;
            this->selected[i][j].second = false;
        }
    }
}
