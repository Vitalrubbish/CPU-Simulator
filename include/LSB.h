#ifndef LSB_H
#define LSB_H
#include "Instruction.h"
const int LSB_size = 5;

struct LSBEntry {
    bool busy = false;
    InstructionType type{};
    int v1 = -1, v2 = -1;
    int q1 = -1, q2 = -1;
    int dest = -1;
    int a = -1;
};

class LSB {
    LSBEntry entry[LSB_size]{};
    int tail = 0;
public:
    LSB() = default;

    [[nodiscard]] int size() const {
        return tail;
    }

    bool AddEntry(const LSBEntry& e) {
        if (size() < LSB_size) {
            entry[tail++] = e;
            return true;
        }
        return false;
    }

    void clear() {
        tail = 0;
    }
};
#endif //LSB_H
