#ifndef EXECUTOR_H
#define EXECUTOR_H
#include "Memory.h"
#include "Instruction.h"
#include "Register.h"
#include "RS.h"


extern Memory memo;

class ALU {
public:

    ALU() = default;

    static unsigned int Calculate(const RSEntry& entry);

    static unsigned int ExecuteLS(const Instruction& ins);
};
#endif //EXECUTOR_H
