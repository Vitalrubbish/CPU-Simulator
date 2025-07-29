#ifndef ROB_H
#define ROB_H
#include "Instruction.h"
#include "Register.h"
#include <iostream>
const int ROB_size = 8;

extern Register regs;

enum class State {
    WAIT, EXECUTE, BROADCAST, COMMIT
};

enum class DestType {
    MEM, REG, BOOL
};

struct ROBEntry {
    Instruction ins{};
    State state{};
    DestType type{};
    unsigned int dest = 0;
    unsigned int value = 0;
    unsigned int pc_value = 0;

    ROBEntry() = default;

    explicit ROBEntry(const Instruction& ins): ins(ins), state(State::WAIT) {
        if (ins.type == InstructionType::SB || ins.type == InstructionType::SH || ins.type == InstructionType::SW) {
            type = DestType::MEM;
            int imm = static_cast<int>(ins.imm << 20) >> 20;
            dest = regs.GetValue(ins.rs1) + imm;
        } else if (!ins.BranchType()) {
            type = DestType::REG;
            dest = ins.rd;
        } else {
            type = DestType::BOOL;
        }
    }
};

class ROB {
    int head = 0;
    int tail = 0;
public:

    ROBEntry entry[ROB_size + 1]{};

    ROB() = default;

    [[nodiscard]] int size() const {
        return (tail - head + ROB_size + 1) % (ROB_size + 1);
    }

    void PrintInfo() const {
        std::cout << "head = " << head << " tail = " << tail << '\n';
    }

    bool empty() const {
        return size() == 0;
    }

    bool full() const {
        return size() == ROB_size;
    }

    int GetHead() const {
        return head;
    }

    int addEntry(const ROBEntry& e) {
        int ret = -1;
        if (size() < ROB_size) {
            ret = tail;
            entry[tail] = e;
            tail = (tail + 1) % (ROB_size + 1);
        }
        return ret;
    }

    void deleteHead() {
        head = (head + 1) % (ROB_size + 1);
    }

    void clear() {
        tail = head;
    }

    void Issue();

    void ExecuteEntry();

    void Broadcast();

    bool CommitEntry();
};
#endif //ROB_H
