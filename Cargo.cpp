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