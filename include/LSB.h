#ifndef LSB_H
#define LSB_H
#include "Instruction.h"
const int LSB_size = 5;

struct LSBEntry {
    bool busy = false;
    InstructionType type{};
    unsigned int v1 = 0, v2 = 0;
    int q1 = -1, q2 = -1;
    unsigned int dest = 0;
    unsigned int a = 0;

    LSBEntry() = default;

    explicit LSBEntry(const Instruction& ins) {
        q1 = regs.GetRecorder(ins.rs1);
        v1 = regs.GetValue(ins.rs1);
        q2 = regs.GetRecorder(ins.rs2);
        v2 = regs.GetValue(ins.rs2);
        a = ins.imm; //注意v1 v2 a的符号问题
        dest = ins.rd;
        type = ins.type;
    }
};

class LSB {
    LSBEntry entry[LSB_size]{};
    int tail = 0;
public:
    LSB() = default;

    [[nodiscard]] int size() const {
        return tail;
    }

    bool full() const {
        return size() == LSB_size;
    }

    void AddEntry(const LSBEntry& e) {
        entry[tail++] = e;
    }

    void clear() {
        tail = 0;
    }
};
#endif //LSB_H
