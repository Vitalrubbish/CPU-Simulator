#ifndef PREDICTOR_H
#define PREDICTOR_H
#include "Instruction.h"
#include "Register.h"

extern unsigned int pc;

class Predictor {
public:

    Predictor() = default;

    static void MovePc(const Instruction& ins) {
        switch (ins.type) {
            case InstructionType::JAL: {
                int offset = static_cast<int>(ins.imm << 12) >> 12;
                pc = pc + offset;
                break;
            }
            case InstructionType::JALR: {
                pc = regs.GetValue(ins.rs1) + ins.imm;
                break;
            }
            case InstructionType::BEQ: {
                int offset = static_cast<int>(ins.imm << 20) >> 20;
                pc = pc + offset;
                break;
            }
            case InstructionType::BGE: {
                int offset = static_cast<int>(ins.imm << 20) >> 20;
                pc = pc + offset;
                break;
            }
            case InstructionType::BGEU: {
                int offset = static_cast<int>(ins.imm << 20) >> 20;
                pc = pc + offset;
                break;
            }
            case InstructionType::BLT: {
                int offset = static_cast<int>(ins.imm << 20) >> 20;
                pc = pc + offset;
                break;
            }
            case InstructionType::BLTU: {
                int offset = static_cast<int>(ins.imm << 20) >> 20;
                pc = pc + offset;
                break;
            }
            case InstructionType::BNE: {
                int offset = static_cast<int>(ins.imm << 20) >> 20;
                pc = pc + offset;
                break;
            }
            default: {
                pc += 4;
            }
        }
    }
};
#endif //PREDICTOR_H
