#ifndef RS_H
#define RS_H
#include "Instruction.h"
extern Register regs;

const int RS_size = 5;

struct RSEntry {
    bool busy = false;
    InstructionType type{};
    unsigned int v1 = -1, v2 = -1;
    int q1 = -1, q2 = -1;
    unsigned int dest = 0;
    unsigned int a = 0;

    RSEntry() = default;

    explicit RSEntry(const Instruction& ins) {
        q1 = regs.GetRecorder(ins.rs1);
        v1 = regs.GetValue(ins.rs1);
        q2 = regs.GetRecorder(ins.rs2);
        v2 = regs.GetValue(ins.rs2);
        a = ins.imm; //注意v1 v2 a的符号问题
        dest = ins.rd;
        type = ins.type;
    }
};

class RS {
    RSEntry entry[RS_size]{};
    int tail = 0;
public:
    RS() = default;

    [[nodiscard]] int size() const {
        return tail;
    }

    bool full() const {
        return size() == RS_size;
    }

    void AddEntry(const RSEntry& e) {
        entry[tail++] = e;
    }

    void clear() {
        tail = 0;
    }
};
#endif //RS_H
