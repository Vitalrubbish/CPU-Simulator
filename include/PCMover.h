#ifndef PCMOVER_H
#define PCMOVER_H
#include "Predictor.h"

extern Predictor predictor;

class PCMover {
public:
    static void MovePc(const Instruction& ins) {
        bool predict_result = predictor.predict(static_cast<int>(ins.index));
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
                if (predict_result) {
                    pc = pc + offset;
                } else {
                    pc = pc + 4;
                }
                break;
            }
            case InstructionType::BGE: {
                int offset = static_cast<int>(ins.imm << 20) >> 20;
                if (predict_result) {
                    pc = pc + offset;
                } else {
                    pc = pc + 4;
                }
                break;
            }
            case InstructionType::BGEU: {
                int offset = static_cast<int>(ins.imm << 20) >> 20;
                if (predict_result) {
                    pc = pc + offset;
                } else {
                    pc = pc + 4;
                }
                break;
            }
            case InstructionType::BLT: {
                int offset = static_cast<int>(ins.imm << 20) >> 20;
                if (predict_result) {
                    pc = pc + offset;
                } else {
                    pc = pc + 4;
                }
                break;
            }
            case InstructionType::BLTU: {
                int offset = static_cast<int>(ins.imm << 20) >> 20;
                if (predict_result) {
                    pc = pc + offset;
                } else {
                    pc = pc + 4;
                }
                break;
            }
            case InstructionType::BNE: {
                int offset = static_cast<int>(ins.imm << 20) >> 20;
                if (predict_result) {
                    pc = pc + offset;
                } else {
                    pc = pc + 4;
                }
                break;
            }
            default: {
                pc += 4;
            }
        }
    }
};
#endif //PCMOVER_H
