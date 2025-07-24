#include "../include/Register.h"

unsigned int Register::GetValue(const unsigned int& index) const {
    return reg[index];
}

void Register::Refresh(const unsigned int &index, const unsigned int &val) {
    for (int i = 0; i < 32; i++) {
        if (recorder[i] == index) {
            recorder[i] = -1;
            reg[i] = val;
        }
    }
}
