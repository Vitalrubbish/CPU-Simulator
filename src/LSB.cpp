#include "../include/LSB.h"
#include "../include/CDB.h"
#include "../include/ALU.h"
#include "../include/ROB.h"

extern unsigned int clk;
extern ROB rob;
extern CDB cdb;

void LSB::Issue() {
    CDBEntry req = cdb.ReceiveRequirement(Hardware::LSB, TransferType::AddEntry);
    if (req.type != TransferType::NONE) {
        LSBEntry e{req.ins, req.index};
        AddEntry(e);
        cdb.RemoveRequirement(req);
    }
}

void LSB::ExecuteEntry() {
    if (!empty() && clk % 3 == 0) {
        LSBEntry lsb_entry = GetFirstEntry();
        if (lsb_entry.recorder == rob.GetHead() && lsb_entry.IsExecutable()) {
            unsigned int value = ALU::ExecuteLS(lsb_entry.ins);
            CDBEntry entry{Hardware::LSB, Hardware::ROB, TransferType::ModifyAfterExecute,
                lsb_entry.recorder, value, 0, 0};
            cdb.AddRequirement(entry);
            DeleteEntry(lsb_entry);
            // std::cout << "Execute Instruction: " << std::hex << lsb_entry.index << '\n';
        }
    }
}

void LSB::Broadcast() {
    CDBEntry req = cdb.ReceiveRequirement(Hardware::LSB, TransferType::ModifyRecorder);
    if (req.type == TransferType::ModifyRecorder) {
        ModifyRecorder(req.index, req.value);
        cdb.RemoveRequirement(req);
    }
}

void LSB::CommitEntry() {
    CDBEntry req = cdb.ReceiveRequirement(Hardware::LSB, TransferType::ModifyRecorder);
    if (req.type != TransferType::NONE) {
        ModifyRecorder(req.index, req.value);
        cdb.RemoveRequirement(req);
    }
    req = cdb.ReceiveRequirement(Hardware::LSB, TransferType::Clear);
    if (req.type != TransferType::NONE) {
        clear();
        cdb.RemoveRequirement(req);
    }
}