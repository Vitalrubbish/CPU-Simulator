#ifndef EXECUTE_H
#define EXECUTE_H
#include "include/Memory.h"
#include "include/Register.h"
#include "include/ALU.h"
#include "include/Execute.h"
#include "include/RS.h"
#include "include/ROB.h"
#include "include/LSB.h"

extern int clk;
extern unsigned int pc;
extern ROB rob;
extern RS rs;
extern LSB lsb;
extern ALU alu;
extern Memory memo;
extern Register regs;

inline Instruction Fetch() {
    unsigned int code = memo.GetInstructionCode(pc);
    return Instruction{code};
}

inline void Issue(const Instruction& ins) {
    ROBEntry entry{ins, ins.rd};
    bool success = rob.addEntry(entry);
}
#endif //EXECUTE_H
