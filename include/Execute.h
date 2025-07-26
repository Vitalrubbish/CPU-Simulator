#ifndef EXECUTE_H
#define EXECUTE_H
#include "Memory.h"
#include "Register.h"
#include "ALU.h"
#include "Execute.h"
#include "RS.h"
#include "ROB.h"
#include "LSB.h"
#include "Predictor.h"

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
    //std::cout << "Issue Instruction Successfully! Instruction code: " << std::hex << ins.index << '\n';
    Predictor::MovePc(ins);
}

inline void Exec() {
    if (!rs.empty()) {
        //rob.PrintInfo();
        RSEntry rs_entry = rs.GetFirstPreparedEntry();
        if (rs_entry.type != InstructionType::NONE) {
            unsigned int result = ALU::Calculate(rs_entry);
            rob.entry[rs_entry.recorder].value = result;
            if (rs_entry.type == InstructionType::JALR) {
                rob.entry[rs_entry.recorder].pc_value = rs_entry.v1 + rs_entry.imm;
            } else if (rob.entry[rs_entry.recorder].ins.BranchType()) {
                if (result == 1) {
                    int offset = static_cast<int>(rs_entry.imm << 20) >> 20;
                    rob.entry[rs_entry.recorder].pc_value = rs_entry.index + offset;
                } else {
                    rob.entry[rs_entry.recorder].pc_value = rs_entry.index + 4;
                }
            }
            rob.entry[rs_entry.recorder].state = State::EXECUTE;
            rs.DeleteEntry(rs_entry);
            //std::cout << "Execute Instruction Successfully! Instruction code: " << std::hex << rs_entry.index << '\n';
        }
    }
    if (!lsb.empty() && clk % 3 == 0) {
        LSBEntry lsb_entry = lsb.GetFirstEntry();
        if (lsb_entry.recorder == rob.GetHead() && lsb_entry.IsExecutable()) {
            rob.entry[lsb_entry.recorder].value = ALU::ExecuteLS(rob.entry[lsb_entry.recorder].ins);
            rob.entry[lsb_entry.recorder].state = State::EXECUTE;
            lsb.DeleteEntry(lsb_entry);
            //std::cout << "Execute Instruction Successfully! Instruction code: " << std::hex << lsb_entry.index << '\n';
        }
    }
}

inline void Broadcast() {
    int head = rob.GetHead();
    for (int i = 0; i < ROB_size; i++) {
        int index = (head + i) % (ROB_size + 1);
        if (rob.entry[index].type == DestType::REG && rob.entry[index].state == State::EXECUTE) {
            rob.entry[index].state = State::BROADCAST;
            rs.ModifyRecorder(index, rob.entry[index].value);
            lsb.ModifyRecorder(index, rob.entry[index].value);
            //std::cout << "Broadcast Instruction Successfully! Instruction code: " << std::hex << rob.entry[index].ins.index << '\n';
            break;
        }
        if (rob.entry[index].type != DestType::REG && rob.entry[index].state == State::EXECUTE) {
            rob.entry[index].state = State::BROADCAST;
            //std::cout << "Broadcast Instruction Successfully! Instruction code: " << std::hex << rob.entry[index].ins.index << '\n';
            break;
        }
    }
}

inline bool Commit() {
    int head = rob.GetHead(), next = (head + 1) % (ROB_size + 1);
    if (!rob.empty() && rob.entry[head].ins.code == 0x0ff00513) {
        return true;
    }
    if (!rob.empty() && rob.entry[head].state == State::BROADCAST) {
        rob.entry[head].state = State::COMMIT;
        //std::cout << "Commit Instruction Successfully! Instruction code: " << rob.entry[head].ins.index << '\n';
        if (rob.entry[head].type == DestType::REG) {
            rs.ModifyRecorder(head, rob.entry[head].value);
            lsb.ModifyRecorder(head, rob.entry[head].value);
            regs.Refresh(rob.entry[head].dest, head, rob.entry[head].value);
        }
        if (rob.entry[head].ins.BranchType()) {
            if (rob.size() == 1 || rob.size() > 1 && rob.entry[next].ins.index != rob.entry[head].pc_value) {
                unsigned int reversed_pc = rob.entry[head].pc_value;
                lsb.clear();
                rs.clear();
                rob.clear();
                regs.clear();
                pc = reversed_pc;
                return false;
            }
        }
        rob.deleteHead();
    }
    return false;
}
#endif //EXECUTE_H