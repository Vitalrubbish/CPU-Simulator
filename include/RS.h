#ifndef RS_H
#define RS_H
#include "Instruction.h"
#include "Register.h"

const int RS_size = 5;

extern Register regs;

struct RSEntry {
    InstructionType type = InstructionType::NONE;
    unsigned int index = 0;
    unsigned int v1 = 0, v2 = 0;
    int q1 = -1, q2 = -1;
    unsigned int dest = 0;
    unsigned int imm = 0;
    int recorder = -1;

    RSEntry() = default;

    explicit RSEntry(const Instruction& ins, const int& place) {
        if (ins.rs1 != 0) {
            q1 = regs.GetRecorder(ins.rs1);
            v1 = regs.GetValue(ins.rs1);
            if (q1 == place) {
                q1 = regs.GetOldRecorder(ins.rs1);
            }
        }
        if (ins.rs2 != 0) {
            q2 = regs.GetRecorder(ins.rs2);
            v2 = regs.GetValue(ins.rs2);
            if (q2 == place) {
                q2 = regs.GetOldRecorder(ins.rs2);
            }
        }
        imm = ins.imm; //注意v1 v2 a的符号问题
        dest = ins.rd;
        type = ins.type;
        recorder = place;
        index = ins.index;
    }

    bool IsExecutable() const {
        return q1 == -1 && q2 == -1;
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

    bool empty() const {
        return size() == 0;
    }

    bool full() const {
        return size() == RS_size;
    }

    void AddEntry(const RSEntry& e) {
        entry[tail++] = e;
    }

    void DeleteEntry(const RSEntry& e) {
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

    RSEntry GetFirstPreparedEntry() {
        for (int i = 0; i < tail; i++) {
            if (entry[i].q1 == -1 && entry[i].q2 == -1) {
                return entry[i];
            }
        }
        return RSEntry{};
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
#endif //RS_H
