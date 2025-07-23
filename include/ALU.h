#ifndef EXECUTOR_H
#define EXECUTOR_H
#include "Memory.h"
#include "Instruction.h"
#include "Register.h"

extern Register regs;
extern Memory memo;

class ALU {
public:

    ALU() = default;

    static void Execute(const Instruction& ins);
};
#endif //EXECUTOR_H
