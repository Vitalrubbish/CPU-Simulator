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
extern unsigned int pc;
extern ROB rob;
extern RS rs;
extern LSB lsb;
extern ALU alu;
extern Predictor predictor;



inline Instruction Fetch() {
    unsigned int code = memo.GetInstructionCode(pc);
    Instruction ins{code, pc};
    return ins;
}

inline void Issue() {
    Instruction ins = Fetch();
    ROBEntry rob_entry{ins};
    RSEntry rs_entry{ins};
    LSBEntry lsb_entry{ins};
    if (ins.LSType()) {
        if (rob.full() || lsb.full()) {
            return false;
        }
        int place = rob.addEntry(rob_entry);
        regs.PutRecorder(ins.rd, place);
        lsb.AddEntry(lsb_entry);
    } else {
        if (rob.full() || rs.full()) {
            return false;
        }
        int place = rob.addEntry(rob_entry);
        regs.PutRecorder(ins.rd, place);
        rs.AddEntry(rs_entry);
    }
    Predictor::MovePc(ins);
    return true;
}

inline void Exec() {

}
#endif //EXECUTE_H
