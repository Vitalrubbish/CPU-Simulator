#ifndef EXECUTOR_H
#define EXECUTOR_H
#include "Memory.h"
#include "Instruction.h"
#include "Register.h"

class ALU {
public:

    ALU() = default;

    static void Execute(const Instruction& ins);
};
#endif //EXECUTOR_H
