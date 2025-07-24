#ifndef EXECUTE_H
#define EXECUTE_H
#include "include/Memory.h"
#include "include/Register.h"
#include "include/ALU.h"
#include "include/Execute.h"
#include "include/RS.h"
#include "include/ROB.h"
#include "include/LSB.h"
#include "include/Predictor.h"

extern int clk;
extern RS rs;
extern LSB lsb;
extern ALU alu;
extern Predictor predictor;
extern ROB rob;


inline Instruction Fetch() {
    unsigned int code = memo.GetInstructionCode(pc);
    Instruction ins{code, pc};
    return ins;
}

inline void Issue() {
    Instruction ins = Fetch();
    ROBEntry rob_entry{ins};
    if (ins.LSType()) {
        if (rob.full() || lsb.full()) {
            return;
        }
        int place = rob.addEntry(rob_entry);
        LSBEntry lsb_entry{ins, place};
        if (ins.rd != 0) {
            regs.PutRecorder(ins.rd, place);
        }
        lsb.AddEntry(lsb_entry);
    } else {
        if (rob.full() || rs.full()) {
            return;
        }
        int place = rob.addEntry(rob_entry);
        RSEntry rs_entry{ins, place};
        if (ins.rd != 0) {
            regs.PutRecorder(ins.rd, place);
        }
        rs.AddEntry(rs_entry);
    }
    Predictor::MovePc(ins);
}

inline void Exec() {
    if (!rs.empty()) {
        RSEntry rs_entry = rs.GetFirstPreparedEntry();
        rob.entry[rs_entry.recorder].value = ALU::Calculate(rs_entry);
        if (rs_entry.type == InstructionType::JALR) {
            rob.entry[rs_entry.recorder].pc_value = rs_entry.v1 + rs_entry.imm;
        } else if (rob.entry[rs_entry.recorder].ins.BranchType()) {
            rob.entry[rs_entry.recorder].pc_value = rs_entry.index + static_cast<int>(rs_entry.imm);
        }
        rob.entry[rs_entry.recorder].state = State::EXECUTE;
        rs.DeleteEntry(rs_entry);
    }
    if (!lsb.empty()) {
        LSBEntry lsb_entry = lsb.GetFirstEntry();
        if (lsb_entry.recorder == rob.GetHead() && lsb_entry.IsExecutable()) {
            rob.entry[lsb_entry.recorder].value = ALU::ExecuteLS(rob.entry[lsb_entry.recorder].ins);
            rob.entry[lsb_entry.recorder].state = State::EXECUTE;
            lsb.DeleteEntry(lsb_entry);
        }
    }
}

inline void Broadcast() {
    for (int i = 0; i < ROB_size; i++) {
        int index = (rob.GetHead() + i) % (ROB_size + 1);
        if (rob.entry[index].type == DestType::REG && rob.entry[index].state == State::EXECUTE) {
            rob.entry[index].state = State::BROADCAST;
            rs.ModifyRecorder(index, rob.entry[index].value);
            lsb.ModifyRecorder(index, rob.entry[index].value);
            break;
        }
    }
}

inline bool Commit() {
    int head = rob.GetHead(), next = (head + 1) % (ROB_size + 1);
    if (!rob.empty() && rob.entry[head].ins.code == 0x0ff00513) {
        return true;
    }
    if (!rob.empty() || rob.entry[head].state == State::BROADCAST) {
        rob.entry[head].state = State::COMMIT;
        if (rob.entry[head].type == DestType::REG) {
            regs.Refresh(rob.entry[head].dest, rob.entry[head].value);
        }
        if (rob.entry[head].ins.BranchType()) {
            if (rob.size() > 1 && rob.entry[next].ins.index != rob.entry[head].pc_value) {
                unsigned int reversed_pc = rob.entry[head].pc_value;
                lsb.clear();
                rs.clear();
                rob.clear();
                regs.clear();
                pc = reversed_pc;
            }
        }
    }
    return false;
}
#endif //EXECUTE_H