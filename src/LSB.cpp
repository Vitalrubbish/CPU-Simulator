#include "../include/LSB.h"
#include "../include/CDB.h"
#include "../include/ALU.h"
#include "../include/ROB.h"

extern unsigned int clk;
extern ROB rob;
extern CDB cdb;


bool LSB::Issue() {
    CDBEntry req = cdb.ReceiveRequirement(Hardware::LSB, TransferType::AddEntry);
    if (req.type != TransferType::NONE) {
        LSBEntry e{req.ins, req.index};
        AddEntry(e);
        cdb.RemoveRequirement(req);
        // std::cout << "LSB - Issue Instruction: " << std::hex << req.ins.index << '\n';
        return true;
    }
    return false;
}

bool LSB::ExecuteEntry() {
    CDBEntry req = cdb.ReceiveRequirement(Hardware::LSB, TransferType::GetHead);
    if (req.type != TransferType::NONE) {
        cdb.RemoveRequirement(req);
    }
    if (!empty() && clk % 3 == 0) {
        LSBEntry lsb_entry = GetFirstEntry();
        if (lsb_entry.recorder == req.index && lsb_entry.IsExecutable()) {
            unsigned int value = ALU::ExecuteLS(lsb_entry);
            CDBEntry entry{Hardware::LSB, Hardware::ROB, TransferType::ModifyAfterExecute,
                lsb_entry.recorder, value, 0, 0};
            cdb.AddRequirement(entry);
            DeleteEntry(lsb_entry);
            // std::cout << "LSB - Execute Instruction: " << std::hex << lsb_entry.index << '\n';
            return true;
        }
    }
    return false;
}

void LSB::CommitEntry() {
    CDBEntry req = cdb.ReceiveRequirement(Hardware::LSB, TransferType::ModifyRecorder);
    if (req.type != TransferType::NONE) {
        ModifyRecorder(req.index, req.value);
        cdb.RemoveRequirement(req);
    }
}

bool LSB::Clear() {
    CDBEntry req = cdb.ReceiveRequirement(Hardware::LSB, TransferType::Clear);
    if (req.type != TransferType::NONE) {
        clear();
        cdb.RemoveRequirement(req);
        // std::cout << "LSB - Clear\n";
        return true;
    }
    return false;
}