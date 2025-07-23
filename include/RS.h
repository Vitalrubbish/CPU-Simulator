#ifndef RS_H
#define RS_H
#include "Instruction.h"

const int RS_size = 5;

struct RSEntry {
    bool busy = false;
    InstructionType type{};
    int v1 = -1, v2 = -1;
    int q1 = -1, q2 = -1;
    int dest = -1;
    int a = -1;
};

class RS {
    RSEntry entry[RS_size]{};
    int tail = 0;
public:
    RS() = default;

    [[nodiscard]] int size() const {
        return tail;
    }

    bool AddEntry(const RSEntry& e) {
        if (size() < RS_size) {
            entry[tail++] = e;
            return true;
        }
        return false;
    }

    void clear() {
        tail = 0;
    }
};
#endif //RS_H
