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
extern ROB rob;


inline void Issue() {
    rob.Issue();
    rs.Issue();
    lsb.Issue();
    regs.Issue();
}

inline void Exec() {
    lsb.ExecuteEntry();
    rs.ExecuteEntry();
    rob.ExecuteEntry();
}

inline void Broadcast() {
    rob.Broadcast();
    rs.Broadcast();
    lsb.Broadcast();
}

inline bool Commit() {
    bool halt = rob.CommitEntry();
    rs.CommitEntry();
    regs.CommitEntry();
    lsb.CommitEntry();
    return halt;
}
#endif //EXECUTE_H