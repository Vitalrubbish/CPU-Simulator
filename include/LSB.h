#ifndef LSB_H
#define LSB_H
#include "Instruction.h"
#include "Register.h"
const int LSB_size = 5;

extern Register regs;

struct LSBEntry {
    InstructionType type = InstructionType::NONE;
    Instruction ins;
    unsigned int index = 0;
    unsigned int v1 = 0, v2 = 0;
    int q1 = -1, q2 = -1;
    unsigned int dest = 0;
    unsigned int imm = 0;
    int recorder = -1;

    LSBEntry() = default;

    explicit LSBEntry(const Instruction& ins, const int& place): ins(ins) {
        q1 = regs.GetRecorder(ins.rs1);
        v1 = regs.GetValue(ins.rs1);
        q2 = regs.GetRecorder(ins.rs2);
        v2 = regs.GetValue(ins.rs2);
        if (q1 == place) {
            q1 = regs.GetOldRecorder(ins.rs1);
        }
        if (q2 == place) {
            q2 = regs.GetOldRecorder(ins.rs2);
        }
        imm = ins.imm; //注意v1 v2 a的符号问题
        dest = ins.rd;
        index = ins.index;
        type = ins.type;
        recorder = place;
    }

    bool IsExecutable() const {
        return q1 == -1 && q2 == -1;
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

    bool empty() const {
        return size() == 0;
    }

    bool full() const {
        return size() == LSB_size;
    }

    void AddEntry(const LSBEntry& e) {
        entry[tail++] = e;
    }

    LSBEntry GetFirstEntry() const {
        return entry[0];
    }

    void DeleteEntry(const LSBEntry& e) {
        for (int i = 0; i < tail; i++) {
            if (entry[i].recorder == e.recorder) {
                for (int j = i; j < tail - 1; j++) {
                    entry[j] = entry[j + 1];
                }
                --tail;
                break;
            }
        }
    }

    void ModifyRecorder(int recorder, unsigned int val) {
        for (auto& it: entry) {
            if (it.q1 == recorder) {
                it.q1 = -1;
                it.v1 = val;
            }
            if (it.q2 == recorder) {
                it.q2 = -1;
                it.v2 = val;
            }
        }
    }

    void clear() {
        tail = 0;
    }

    bool Issue();

    bool ExecuteEntry();

    void CommitEntry();

    bool Clear();
};
#endif //LSB_H
