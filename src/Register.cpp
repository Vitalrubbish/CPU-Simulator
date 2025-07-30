#include "../include/Register.h"
#include "../include/CDB.h"

extern CDB cdb;

unsigned int Register::GetValue(const unsigned int& index) const {
    return reg[index];
}

void Register::ModifyRecorder(const unsigned int &index, const int &reco, const unsigned int &val) {
    if (recorder[index] == reco) {
        old_recorder[index] = recorder[index];
        recorder[index] = -1;
    }
    reg[index] = val;
}

bool Register::Issue() {
    CDBEntry req = cdb.ReceiveRequirement(Hardware::Register, TransferType::PutRecorder);
    if (req.type != TransferType::NONE) {
        PutRecorder(req.dest, req.index);
        cdb.RemoveRequirement(req);
        // std::cout << "Register - Issue Instruction\n";
        return true;
    }
    return false;
}


void Register::CommitEntry() {
    CDBEntry req = cdb.ReceiveRequirement(Hardware::Register, TransferType::ModifyRecorder);
    if (req.type != TransferType::NONE) {
        ModifyRecorder(req.dest, req.index, req.value);
        cdb.RemoveRequirement(req);
        // std::cout << "Register - Commit Instruction\n";
    }
    req = cdb.ReceiveRequirement(Hardware::Register, TransferType::Clear);
    if (req.type != TransferType::NONE) {
        clear();
        // std::cout << "Register - Clear\n";
        cdb.RemoveRequirement(req);
    }
}

bool Register::Clear() {
    CDBEntry req = cdb.ReceiveRequirement(Hardware::Register, TransferType::Clear);
    if (req.type != TransferType::NONE) {
        clear();
        // std::cout << "Register - Clear\n";
        cdb.RemoveRequirement(req);
        return true;
    }
    return false;
}
