#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "../utility/utility.h"
enum class InstructionType {
    LUI, AUIPC, JAL, JALR, BEQ, BNE, BLT, BGE, BLTU, BGEU, LB, LH, LW,
    LBU, LHU, SB, SH, SW, ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI,
    SRAI, ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND
};

struct Instruction {
    InstructionType type{};
    unsigned int rs1 = 0, rs2 = 0, rd = 0;
    unsigned int imm = 0;

    Instruction() = default;

    explicit Instruction(const unsigned int &code);

};
#endif //INSTRUCTION_H
