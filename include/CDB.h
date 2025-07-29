#ifndef CDB_H
#define CDB_H
#include <iostream>
#include "Instruction.h"

const int CDB_size = 100;
enum class Hardware {
    ALU, LSB, RS, ROB, Register, Memory
};

enum class TransferType {
    ModifyAfterExecute, ModifyRecorder, Clear, SendInstruction,
    AddEntry, PutRecorder, ModifyRecorderAndValue, NONE
};

struct CDBEntry {
    Hardware from{};
    Hardware to{};
    TransferType type = TransferType::NONE;
    Instruction ins{};
    int index{};
    unsigned int value{};
    unsigned int pc_value{};
    unsigned int dest{};

    CDBEntry() = default;

    CDBEntry(const Hardware& from, const Hardware& to, const TransferType &type,
        const int& index, const unsigned int& value, const unsigned int& pc_value, const unsigned int& dest) {
        this -> from = from;
        this -> to = to;
        this -> type = type;
        this -> index = index;
        this -> value = value;
        this -> pc_value = pc_value;
        this -> dest = dest;
    }

    CDBEntry(const Hardware& from, const Hardware& to, const TransferType& type,
        const Instruction& ins, const int& index) {
        this -> from = from;
        this -> to = to;
        this -> type = type;
        this -> ins = ins;
        this -> index = index;
    }

    bool operator== (const CDBEntry& e) const {
        return from == e.from && to == e.to && type == e.type &&
            index == e.index && value == e.value;
    }
};

class CDB {
    CDBEntry entry[CDB_size]{};
    CDBEntry nex_entry[CDB_size]{};
    int tail = 0;
    int nex_tail = 0;

public:
    CDB() = default;

    [[nodiscard]]int size() const {
        return tail;
    }

    void AddRequirement(const CDBEntry &e) {
        nex_entry[nex_tail++] = e;
    }

    void RemoveRequirement(const CDBEntry &e) {
        for (int i = 0; i < tail; i++) {
            if (e == entry[i]) {
                for (int j = i; j < tail - 1; j++) {
                    entry[j] = entry[j + 1];
                }
                --tail;
                break;
            }
        }
    }

    CDBEntry ReceiveRequirement(const Hardware& hardware, const TransferType& transferType) const {
        for (int i = 0; i < tail; i++) {
            if (entry[i].to == hardware && entry[i].type == transferType) {
                return entry[i];
            }
        }
        return CDBEntry{};
    }

    void Refresh() {
        for (int i = tail; i < tail + nex_tail; i++) {
            entry[i] = nex_entry[i - tail];
        }
        tail += nex_tail;
        nex_tail = 0;
    }
};
#endif //CDB_H
