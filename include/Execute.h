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

inline bool ROB_run() {
    rob.Issue();
    rob.ExecuteEntry();
    rob.Broadcast();
    return rob.CommitEntry();
}

inline void RS_run() {
    rs.Issue();
    rs.ExecuteEntry();
    rs.Broadcast();
    rs.CommitEntry();
}

inline void LSB_run() {
    lsb.Issue();
    lsb.ExecuteEntry();
    lsb.Broadcast();
    lsb.CommitEntry();
}

inline void Register_run() {
    regs.Issue();
    regs.CommitEntry();
}
#endif //EXECUTE_H