#ifndef PREDICTOR_H
#define PREDICTOR_H
#include <utility>
#include "Instruction.h"
#include "Register.h"

extern unsigned int pc;
const int len = 1024;
const int history_length = 64;
const int MINW = -8;
const int MAXW = 7;

class Predictor {
    uint64_t history[len]{};
    int weight[len][history_length]{};
    int theta = 137;
    int bias[len]{};
public:

    Predictor() {
        for (auto& it: history) {
            it = 0x0f1e2d3c4b5a6978;
        }
    }

    void update(bool taken, unsigned int index) {
        if (taken) {
            history[index] = (history[index] << 1) + 1;
        } else {
            history[index] = history[index] << 1;
        }
    }

    int GetSum(unsigned int index) const {
        int sum = bias[index];
        for (int i = 0; i < history_length; i++) {
            if (history[index] & (1 << i)) {
                sum += weight[index][i];
            } else {
                sum -= weight[index][i];
            }
        }
        return sum;
    }

    bool predict(unsigned int index) const {
        if (GetSum(index) >= 0) {
            return true;
        }
        return false;
    }

    void train(bool result, bool predict_result, unsigned int index) {
        int sum = GetSum(index);
        if (result != predict_result || abs(sum) < theta) {
            if (result) {
                bias[index] = std::min(bias[index] + 1, MAXW);
            } else {
                bias[index] = std::max(bias[index] - 1, MINW);
            }
            for (int i = 0; i < history_length; i++) {
                bool bit = history[index] & (1 << i);
                if (bit) {
                    if (result) {
                        weight[index][i] = std::min(weight[index][i] + 1, MAXW);
                    } else {
                        weight[index][i] = std::max(weight[index][i] - 1, MINW);
                    }
                } else {
                    if (result) {
                        weight[index][i] = std::max(weight[index][i] - 1, MINW);
                    } else {
                        weight[index][i] = std::min(weight[index][i] + 1, MAXW);
                    }
                }
            }
        }
    }

    void MovePc(const Instruction& ins) const {
        bool predict_result = predict(ins.index % 1024);
        // bool predict_result = true;
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
#endif //PREDICTOR_H
