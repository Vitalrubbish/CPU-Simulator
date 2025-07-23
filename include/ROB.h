#ifndef ROB_H
#define ROB_H
#include "Instruction.h"
const int ROB_size = 5;

enum class State {
    EXECUTE, WRITE, COMMIT
};

struct ROBEntry {
    Instruction ins{};
    State state{};
    unsigned int dest = 0;
    unsigned int value = 0;

    ROBEntry() = default;

    explicit ROBEntry(const Instruction& ins): ins(ins), state(State::EXECUTE), dest(ins.rd) {}
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

    bool full() const {
        return size() == ROB_size;
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

    void cleanCommitEntry() {
        while (head != tail && entry[head].state == State::COMMIT) {
            head = (head + 1) % (ROB_size + 1);
        }
    }

    void deleteUnCommit() {
        while (head != tail && entry[head].state != State::COMMIT) {
            tail = (tail + ROB_size) % (ROB_size + 1);
        }
    }
};
#endif //ROB_H
