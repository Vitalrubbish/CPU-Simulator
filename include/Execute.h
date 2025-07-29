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
    if (rob.Issue()) {
        return false;
    }
    if (rob.ExecuteEntry()) {
        return false;
    }
    return rob.CommitEntry();
}

inline void RS_run() {
    if (rs.Clear()) {
        return;
    }
    if (rs.Issue()) {
        return;
    }
    if (rs.ExecuteEntry()) {
        return;
    }
    rs.CommitEntry();
}

inline void LSB_run() {
    if (lsb.Clear()) {
        return;
    }
    if (lsb.Issue()) {
        return;
    }
    if (lsb.ExecuteEntry()) {
        return;
    }
    lsb.CommitEntry();
}

inline void Register_run() {
    if (regs.Clear()) {
        return;
    }
    if (regs.Issue()) {
        return;
    }
    regs.CommitEntry();
}
#endif //EXECUTE_H