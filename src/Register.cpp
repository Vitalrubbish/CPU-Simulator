#include "../include/Register.h"

unsigned int Register::GetValue(const unsigned int& index) const {
    return reg[index];
}

void Register::Refresh(const unsigned int &index, const unsigned int &val) {
    recorder[index] = -1;
    reg[index] = val;
}
