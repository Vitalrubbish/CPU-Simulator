#include "../include/Register.h"

unsigned int Register::GetValue(const unsigned int& index) const {
    return reg[index];
}

void Register::Refresh(const unsigned int &index, const int &reco, const unsigned int &val) {
    if (recorder[index] == reco) {
        recorder[index] = -1;
    }
    reg[index] = val;
}
