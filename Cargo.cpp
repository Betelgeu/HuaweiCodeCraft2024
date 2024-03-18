#include "Cargo.h"

Cargo::Cargo() {}
Cargo::Cargo(int start_x, int start_y, int val) {
    this->x = start_x;
    this->y = start_y;
    this->val = val;
    this->selected = false;
    this->exist_time = cargoLeftTime;
}
void Cargo::time_decay() {
    this->exist_time--;
}

void removeCargo(vector<Cargo*> &CargoList) {
    CargoList.erase(std::remove_if(CargoList.begin(), CargoList.end(), [](Cargo* c){ return c->exist_time <= 0; }), CargoList.end());
}