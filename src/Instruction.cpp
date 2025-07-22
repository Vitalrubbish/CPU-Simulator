#include <iostream>
#include "../include/Instruction.h"

unsigned int GetOpCode(const unsigned int &code) {
    unsigned int ret = 0;
    for (int i = 6; i >= 0; i--) {
        ret = ret * 2 + GetBit(code, i);
    }
    return ret;
}

unsigned int GetFunc3(const unsigned int &code) {
    unsigned int ret = 0;
    for (int i = 14; i >= 12; i--) {
        ret = ret * 2 + GetBit(code, i);
    }
    return ret;
}

unsigned int GetFunc7(const unsigned int &code) {
    unsigned int ret = 0;
    for (int i = 31; i >= 25; i--) {
        ret = ret * 2 + GetBit(code, i);
    }
    return ret;
}

unsigned int GetRd(const unsigned int &code) {
    unsigned int ret = 0;
    for (int i = 11; i >= 7; i--) {
        ret = ret * 2 + GetBit(code, i);
    }
    return ret;
}

unsigned int GetRs1(const unsigned int &code) {
    unsigned int ret = 0;
    for (int i = 19; i >= 15; i--) {
        ret = ret * 2 + GetBit(code, i);
    }
    return ret;
}

unsigned int GetRs2(const unsigned int &code) {
    unsigned int ret = 0;
    for (int i = 24; i >= 20; i--) {
        ret = ret * 2 + GetBit(code, i);
    }
    return ret;
}

unsigned int GetImm1(const unsigned int &code) {
    unsigned int ret = 0;
    for (int i = 31; i >= 12; i--) {
        ret = ret * 2 + GetBit(code, i);
    }
    ret <<= 12;
    return ret;
}

unsigned int GetImm2(const unsigned int &code) {
    unsigned int ret = 0;
    ret = ret * 2 + GetBit(code, 31);
    for (int i = 19; i >= 12; i--) {
        ret = ret * 2 + GetBit(code, i);
    }
    ret = ret * 2 + GetBit(code, 20);
    for (int i = 30; i >= 21; i--) {
        ret = ret * 2 + GetBit(code, i);
    }
    ret <<= 1;
    return ret;
}

unsigned int GetImm3(const unsigned int &code) {
    unsigned int ret = 0;
    for (int i = 31; i >= 20; i--) {
        ret = ret * 2 + GetBit(code, i);
    }
    return ret;
}

unsigned int GetImm4(const unsigned int &code) {
    unsigned int ret = 0;
    ret = ret * 2 + GetBit(code, 31);
    ret = ret * 2 + GetBit(code, 7);
    for (int i = 30; i >= 25; i--) {
        ret = ret * 2 + GetBit(code, i);
    }
    for (int i = 11; i >= 8; i--) {
        ret = ret * 2 + GetBit(code, i);
    }
    ret <<= 1;
    return ret;
}

unsigned int GetImm5(const unsigned int &code) {
    unsigned int ret = 0;
    for (int i = 31; i >= 25; i--) {
        ret = ret * 2 + GetBit(code, i);
    }
    for (int i = 11; i >= 7; i--) {
        ret = ret * 2 + GetBit(code, i);
    }
    return ret;
}

Instruction::Instruction(const unsigned int &code) {
    unsigned int opCode = GetOpCode(code);
    unsigned int func3 = GetFunc3(code);
    unsigned int func7 = GetFunc7(code);
    switch (opCode) {
        case 0x37: {
            type = InstructionType::LUI;
            rd = GetRd(code);
            imm = GetImm1(code);
            break;
        }
        case 0x17: {
            type = InstructionType::AUIPC;
            rd = GetRd(code);
            imm = GetImm1(code);
            break;
        }
        case 0x6f: {
            type = InstructionType::JAL;
            rd = GetRd(code);
            imm = GetImm2(code);
            break;
        }
        case 0x67: {
            type = InstructionType::JALR;
            rd = GetRd(code);
            rs1 = GetRs1(code);
            imm = GetImm3(code);
            break;
        }
        case 0x63: {
            switch (func3) {
                case 0: type = InstructionType::BEQ; break;
                case 1: type = InstructionType::BNE; break;
                case 4: type = InstructionType::BLT; break;
                case 5: type = InstructionType::BGE; break;
                case 6: type = InstructionType::BLTU; break;
                case 7: type = InstructionType::BGEU; break;
                default:;
            }
            rs1 = GetRs1(code);
            rs2 = GetRs2(code);
            imm = GetImm4(code);
            break;
        }
        case 0x03: {
            switch (func3) {
                case 0: type = InstructionType::LB; break;
                case 1: type = InstructionType::LH; break;
                case 2: type = InstructionType::LW; break;
                case 4: type = InstructionType::LBU; break;
                case 5: type = InstructionType::LHU; break;
                default:;
            }
            rs1 = GetRs1(code);
            rd = GetRd(code);
            imm = GetImm3(code);
            break;
        }
        case 0x23: {
            switch (func3) {
                case 0: type = InstructionType::SB; break;
                case 1: type = InstructionType::SH; break;
                case 2: type = InstructionType::SW; break;
                default:;
            }
            rs1 = GetRs1(code);
            rs2 = GetRs2(code);
            imm = GetImm5(code);
            break;
        }
        case 0x13: {
            switch (func3) {
                case 0: type = InstructionType::ADDI; break;
                case 2: type = InstructionType::SLTI; break;
                case 3: type = InstructionType::SLTIU; break;
                case 4: type = InstructionType::XORI; break;
                case 6: type = InstructionType::ORI; break;
                case 7: type = InstructionType::ANDI; break;
                case 1: type = InstructionType::SLLI; break;
                case 5: {
                    switch (func7) {
                        case 0x00: type = InstructionType::SRLI; break;
                        case 0x20: type = InstructionType::SRAI; break;
                        default:;
                    }
                    break;
                }
                default:;
            }
            rs1 = GetRs1(code);
            rd = GetRd(code);
            imm = GetImm3(code);
            break;
        }
        case 0x33: {
            switch (func3) {
                case 0: {
                    switch (func7) {
                        case 0x00: type = InstructionType::ADD; break;
                        case 0x20: type = InstructionType::SUB; break;
                        default:;
                    }
                    break;
                }
                case 1: type = InstructionType::SLL; break;
                case 2: type = InstructionType::SLT; break;
                case 3: type = InstructionType::SLTU; break;
                case 4: type = InstructionType::XOR; break;
                case 5: {
                    switch (func7) {
                        case 0x00: type = InstructionType::SLL; break;
                        case 0x20: type = InstructionType::SRA; break;
                        default:;
                    }
                    break;
                }
                case 6: type = InstructionType::OR; break;
                case 7: type = InstructionType::AND; break;
                default:;
            }
            rs1 = GetRs1(code);
            rs2 = GetRs2(code);
            rd = GetRd(code);
            break;
        }
        default:;
    }
}