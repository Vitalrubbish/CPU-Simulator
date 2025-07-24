#include "../include/ALU.h"

extern unsigned int qc;

unsigned int ALU::Calculate(const RSEntry& entry){
    regs.PutValue(0, 0);
    switch (entry.type) {
        case InstructionType::LUI: {
            return entry.imm;
        }
        case InstructionType::AUIPC: {
            return entry.imm + entry.index;
        }
        case InstructionType::BEQ: {
            if (entry.v1 == entry.v2) {
                return 1;
            }
            return 0;
        }
        case InstructionType::BGE: {
            if (static_cast<int>(entry.v1) >= static_cast<int>(entry.v2)) {
                return 1;
            }
            return 0;
        }
        case InstructionType::BGEU: {
            if (entry.v1 >= entry.v2) {
                return 1;
            }
            return 0;
        }
        case InstructionType::BLT: {
            if (static_cast<int>(entry.v1) < static_cast<int>(entry.v2)) {
                return 1;
            }
            return 0;
        }
        case InstructionType::BLTU: {
            if (entry.v1 < entry.v2) {
                return 1;
            }
            return 0;
        }
        case InstructionType::BNE: {
            if (entry.v1 != entry.v2) {
                return 1;
            }
            return 0;
        }
        case InstructionType::ADD: {
            return entry.v1 + entry.v2;
        }
        case InstructionType::SUB: {
            return entry.v1 - entry.v2;
        }
        case InstructionType::AND: {
            return entry.v1 & entry.v2;
        }
        case InstructionType::OR: {
            return entry.v1 | entry.v2;
        }
        case InstructionType::XOR: {
            return entry.v1 ^ entry.v2;
        }
        case InstructionType::SLL: {
            return entry.v1 << entry.v2;
        }
        case InstructionType::SRL: {
            return entry.v1 >> entry.v2;
        }
        case InstructionType::SRA: {
            return static_cast<int>(entry.v1) >> entry.v2;
        }
        case InstructionType::SLT: {
            if (static_cast<int>(entry.v1) < static_cast<int>(entry.v2)) {
                return 1;
            }
            return 0;
        }
        case InstructionType::SLTU: {
            if (entry.v1 < entry.v2) {
                return 1;
            }
            return 0;
        }
        case InstructionType::ADDI: {
            int imm = static_cast<int>(entry.imm << 20) >> 20;
            return entry.v1 + imm;
        }
        case InstructionType::ANDI: {
            return entry.v1 & entry.imm;
        }
        case InstructionType::ORI: {
            return entry.v1 | entry.imm;
        }
        case InstructionType::XORI: {
            return entry.v1 ^ entry.imm;
        }
        case InstructionType::SLLI: {
            return entry.v1 << entry.imm;
        }
        case InstructionType::SRLI: {
            return entry.v1 >> entry.imm;
        }
        case InstructionType::SRAI: {
            return static_cast<int>(entry.v1) >> entry.imm;
        }
        case InstructionType::SLTI: {
            if (static_cast<int>(entry.v1) < static_cast<int>(entry.imm)) {
                return 1;
            }
            return 0;
        }
        case InstructionType::SLTIU: {
            if (entry.v1 < entry.imm) {
                return 1;
            }
            return 0;
        }
        default: {
            return entry.index + 4;
        }
    }
}

unsigned int ALU::ExecuteLS(const Instruction &ins) {
    switch(ins.type) {
        case InstructionType::LB: {
            int imm = static_cast<int>(ins.imm << 20) >> 20;
            unsigned int index = regs.GetValue(ins.rs1) + imm;
            return memo.LoadByte(index, true);
        }
        case InstructionType::LBU: {
            int imm = static_cast<int>(ins.imm << 20) >> 20;
            unsigned int index = regs.GetValue(ins.rs1) + imm;
            return memo.LoadByte(index, false);
        }
        case InstructionType::LH: {
            int imm = static_cast<int>(ins.imm << 20) >> 20;
            unsigned int index = regs.GetValue(ins.rs1) + imm;
            return memo.LoadHalf(index, true);
        }
        case InstructionType::LHU: {
            int imm = static_cast<int>(ins.imm << 20) >> 20;
            unsigned int index = regs.GetValue(ins.rs1) + imm;
            return memo.LoadHalf(index, false);
        }
        case InstructionType::LW: {
            int imm = static_cast<int>(ins.imm << 20) >> 20;
            unsigned int index = regs.GetValue(ins.rs1) + imm;
            return memo.LoadWord(index);
        }
        case InstructionType::SB: {
            int imm = static_cast<int>(ins.imm << 20) >> 20;
            unsigned int index = regs.GetValue(ins.rs1) + imm;
            memo.StoreByte(index, regs.GetValue(ins.rs2));
            return 0;
        }
        case InstructionType::SH: {
            int imm = static_cast<int>(ins.imm << 20) >> 20;
            unsigned int index = regs.GetValue(ins.rs1) + imm;
            memo.StoreHalf(index, regs.GetValue(ins.rs2));
            return 0;
        }
        case InstructionType::SW: {
            int imm = static_cast<int>(ins.imm << 20) >> 20;
            unsigned int index = regs.GetValue(ins.rs1) + imm;
            memo.StoreWord(index, regs.GetValue(ins.rs2));
            return 0;
        }
        default: return 0;
    }
}
