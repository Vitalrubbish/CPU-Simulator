#include "../include/RS.h"
#include "../include/ALU.h"
#include "../include/CDB.h"

extern CDB cdb;

void RS::Issue() {
    CDBEntry req = cdb.ReceiveRequirement(Hardware::RS, TransferType::AddEntry);
    if (req.type != TransferType::NONE) {
        RSEntry e{req.ins, req.index};
        AddEntry(e);
        cdb.RemoveRequirement(req);
    }
}

void RS::ExecuteEntry() {
    if (!empty()) {
        RSEntry rs_entry = GetFirstPreparedEntry();
        if (rs_entry.type != InstructionType::NONE) {
            unsigned int result = ALU::Calculate(rs_entry);
            unsigned int pc_value;
            if (rs_entry.type == InstructionType::JALR) {
                pc_value = rs_entry.v1 + rs_entry.imm;
            } else if (rs_entry.type == InstructionType::BEQ || rs_entry.type == InstructionType::BLT ||
                rs_entry.type == InstructionType::BGE || rs_entry.type == InstructionType::BNE ||
                rs_entry.type == InstructionType::BGEU || rs_entry.type == InstructionType::BLTU) {
                if (result == 1) {
                    int offset = static_cast<int>(rs_entry.imm << 20) >> 20;
                    pc_value = rs_entry.index + offset;
                } else {
                    pc_value = rs_entry.index + 4;
                }
            }
            CDBEntry entry{Hardware::RS, Hardware::ROB, TransferType::ModifyAfterExecute,
                rs_entry.recorder, result, pc_value, 0};
            cdb.AddRequirement(entry);
            DeleteEntry(rs_entry);
            //std::cout << "Execute Instruction: " << std::hex << rs_entry.index << '\n';
        }
    }
}

void RS::Broadcast() {
    CDBEntry req = cdb.ReceiveRequirement(Hardware::RS, TransferType::ModifyRecorder);
    if (req.type != TransferType::NONE) {
        ModifyRecorder(req.index, req.value);
        cdb.RemoveRequirement(req);
    }
}


void RS::CommitEntry() {
    CDBEntry req = cdb.ReceiveRequirement(Hardware::RS, TransferType::ModifyRecorder);
    if (req.type != TransferType::NONE) {
        ModifyRecorder(req.index, req.value);
        cdb.RemoveRequirement(req);
    }
    req = cdb.ReceiveRequirement(Hardware::RS, TransferType::Clear);
    if (req.type != TransferType::NONE) {
        clear();
        cdb.RemoveRequirement(req);
    }
}