#ifndef MEMORY_H
#define MEMORY_H
#include "../utility/utility.h"
constexpr int MemSize = 5e4;
class Memory {
    unsigned int mem[MemSize]{};
    unsigned int cursor = 0;
public:

    Memory() = default;

    void MoveCursor(unsigned int goal) {
        cursor = goal;
    }

    unsigned int GetInstructionCode(unsigned int index) {
        return mem[index / 4];
    }

    void InitStoreWord(unsigned int val) {
        mem[cursor] = val;
        ++cursor;
    }

    unsigned int LoadWord(unsigned int index) {
        return mem[index / 4];
    }

    void StoreWord(unsigned int index, unsigned int val) {
        mem[index / 4] = val;
    }

    unsigned int LoadByte(unsigned int index, bool extend) {
        unsigned int ret = 0;
        int q = static_cast<int>(index / 4);
        int r = static_cast<int>(index % 4);
        if (extend && GetBit(mem[q], r * 8 + 7)) {
            for (int i = 1; i <= 24; i++) {
                ret = ret * 2 + 1;
            }
        }
        for (int i = r * 8 + 7; i >= r * 8; i--) {
            ret = ret * 2 + GetBit(mem[q], i);
        }
        return ret;
    }

    void StoreByte(unsigned int index, unsigned int val) {
        int q = static_cast<int>(index / 4);
        int r = static_cast<int>(index % 4);
        for (int i = r * 8 + 7; i >= r * 8; i--) {
            int bit = GetBit(val, i - r * 8);
            mem[q] = SetBit(mem[q], i, bit);
        }
    }

    unsigned int LoadHalf(unsigned int index, bool extend) {
        unsigned int ret = 0;
        int q = static_cast<int>(index / 4);
        int r = static_cast<int>(index % 4);
        if (extend && GetBit(mem[q], r * 8 + 15)) {
            for (int i = 1; i <= 16; i++) {
                ret = ret * 2 + 1;
            }
        }
        for (int i = r * 8 + 15; i >= r * 8; i--) {
            ret = ret * 2 + GetBit(mem[q], i);
        }
        return ret;
    }

    void StoreHalf(unsigned int index, unsigned int val) {
        int q = static_cast<int>(index / 4);
        int r = static_cast<int>(index % 4);
        for (int i = r * 8 + 15; i >= r * 8; i--) {
            int bit = GetBit(val, i - r * 8);
            mem[q] = SetBit(mem[q], i, bit);
        }
    }
};
#endif //MEMORY_H
