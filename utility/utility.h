#ifndef UTILITY_H
#define UTILITY_H
#include <cstring>
#include <string>
#include <algorithm>

inline unsigned int SymbolTransfer(char sym) {
    unsigned int ret;
    switch(sym) {
        case '0': ret = 0; break;
        case '1': ret = 1; break;
        case '2': ret = 2; break;
        case '3': ret = 3; break;
        case '4': ret = 4; break;
        case '5': ret = 5; break;
        case '6': ret = 6; break;
        case '7': ret = 7; break;
        case '8': ret = 8; break;
        case '9': ret = 9; break;
        case 'A': ret = 10; break;
        case 'B': ret = 11; break;
        case 'C': ret = 12; break;
        case 'D': ret = 13; break;
        case 'E': ret = 14; break;
        case 'F': ret = 15; break;
        default: ret = 0;
    }
    return ret;
}

inline unsigned int HexToDec(const std::string& hex) {
    unsigned int ret = 0;
    for (const char it : hex) {
        ret = ret * 16 + SymbolTransfer(it);
    }
    return ret;
}

inline int GetBit(const unsigned int& x, int index) {
    if ((x & 1U << index) != 0) {
        return 1;
    }
    return 0;
}

inline unsigned int SetBit(unsigned int x, int index, int val) {
    x -= (GetBit(x, index) << index);
    x += (val << index);
    return x;
}
#endif //UTILITY_H
