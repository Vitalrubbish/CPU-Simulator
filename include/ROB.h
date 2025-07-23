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

    ROBEntry(const Instruction& ins, const unsigned int& dest): ins(ins), dest(dest), state(State::EXECUTE) {}
};

class ROB {
    ROBEntry entry[ROB_size + 1]{};
    int head = 0;
    int tail = 0;
public:
    ROB() = default;

    [[nodiscard]] int size() const {
        return (tail - head + ROB_size + 1) % (ROB_size + 1);
    }

    bool addEntry(const ROBEntry& e) {
        if (size() < ROB_size) {
            entry[tail] = e;
            tail = (tail + 1) % (ROB_size + 1);
            return true;
        }
        return false;
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
