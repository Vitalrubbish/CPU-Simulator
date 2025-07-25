#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "../utility/utility.h"
enum class InstructionType {
    LUI, AUIPC, JAL, JALR, BEQ, BNE, BLT, BGE, BLTU, BGEU, LB, LH, LW,
    LBU, LHU, SB, SH, SW, ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI,
    SRAI, ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND, NONE
};

struct Instruction {
    unsigned int code = 0;
    unsigned int index = 0;
    InstructionType type{};
    unsigned int rs1 = 0, rs2 = 0, rd = 0;
    unsigned int imm = 0;

    Instruction() = default;

    Instruction(const unsigned int &code, const unsigned int &index);

    bool LSType() const {
        return type == InstructionType::LB || type == InstructionType::LH || type == InstructionType::LW ||
            type == InstructionType::SB || type == InstructionType::SH || type == InstructionType::SW ||
            type == InstructionType::LBU || type == InstructionType::LHU;
    }

    bool BranchType() const {
        return type == InstructionType::JALR || type == InstructionType::BEQ || type == InstructionType::BLT ||
            type == InstructionType::BGE || type == InstructionType::BNE || type == InstructionType::BGEU ||
            type == InstructionType::BLTU;
    }
};
#endif //INSTRUCTION_H
