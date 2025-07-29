#include "../include/Register.h"
#include "../include/CDB.h"

extern CDB cdb;

unsigned int Register::GetValue(const unsigned int& index) const {
    return reg[index];
}

void Register::ModifyRecorder(const unsigned int &index, const int &reco, const unsigned int &val) {
    if (recorder[index] == reco) {
        recorder[index] = -1;
    }
    reg[index] = val;
}

void Register::Issue() {
    CDBEntry req = cdb.ReceiveRequirement(Hardware::Register, TransferType::PutRecorder);
    if (req.type != TransferType::NONE) {
        PutRecorder(req.dest, req.index);
        cdb.RemoveRequirement(req);
    }
}


void Register::CommitEntry() {
    CDBEntry req = cdb.ReceiveRequirement(Hardware::Register, TransferType::ModifyRecorder);
    if (req.type != TransferType::NONE) {
        ModifyRecorder(req.dest, req.index, req.value);
        cdb.RemoveRequirement(req);
    }
    req = cdb.ReceiveRequirement(Hardware::Register, TransferType::Clear);
    if (req.type != TransferType::NONE) {
        clear();
        cdb.RemoveRequirement(req);
    }
}

