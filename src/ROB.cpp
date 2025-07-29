#include "../include/ROB.h"
#include "../include/CDB.h"
#include "../include/Predictor.h"
#include "../include/Memory.h"


extern CDB cdb;
extern Memory memo;


void ROB::Issue() {
    Instruction ins{memo.GetInstructionCode(pc), pc};
    ROBEntry rob_entry{ins};
    if (full()) {
        return;
    }
    int place = addEntry(rob_entry);
    if (ins.LSType()) {
        CDBEntry lsb_req{Hardware::ROB, Hardware::LSB, TransferType::AddEntry,
            ins, place};
        cdb.AddRequirement(lsb_req);
        if (ins.rd != 0) {
            CDBEntry regs_req{Hardware::ROB, Hardware::Register, TransferType::PutRecorder,
                place, 0, 0, ins.rd};
            cdb.AddRequirement(regs_req);
        }
    } else {
        CDBEntry rs_req{Hardware::ROB, Hardware::RS, TransferType::AddEntry,
            ins, place};
        cdb.AddRequirement(rs_req);
        if (ins.rd != 0) {
            CDBEntry regs_req{Hardware::ROB, Hardware::Register, TransferType::PutRecorder,
                place, 0, 0, ins.rd};
            cdb.AddRequirement(regs_req);
        }
    }
    // std::cout << "Issue Instruction: " << std::hex << ins.index << '\n';
    Predictor::MovePc(ins);
}


void ROB::ExecuteEntry() {
    CDBEntry req = cdb.ReceiveRequirement(Hardware::ROB, TransferType::ModifyAfterExecute);
    if (req.type != TransferType::NONE) {
        entry[req.index].value = req.value;
        entry[req.index].pc_value = req.pc_value;
        entry[req.index].state = State::EXECUTE;
        cdb.RemoveRequirement(req);
    }
}

void ROB::Broadcast() {
    int head = GetHead();
    for (int i = 0; i < ROB_size; i++) {
        int index = (head + i) % (ROB_size + 1);
        if (entry[index].type == DestType::REG && entry[index].state == State::EXECUTE) {
            entry[index].state = State::BROADCAST;
            CDBEntry rs_req{Hardware::ROB, Hardware::RS, TransferType::ModifyRecorder,
                index, entry[index].value, 0, 0};
            CDBEntry lsb_req{Hardware::ROB, Hardware::LSB, TransferType::ModifyRecorder,
                index, entry[index].value, 0, 0};
            cdb.AddRequirement(rs_req);
            cdb.AddRequirement(lsb_req);
            // std::cout << "Broadcast Instruction Successfully! Instruction code: " << std::hex << entry[index].ins.index << '\n';
            break;
        }
        if (entry[index].type != DestType::REG && entry[index].state == State::EXECUTE) {
            entry[index].state = State::BROADCAST;
            // std::cout << "Broadcast Instruction Successfully! Instruction code: " << std::hex << entry[index].ins.index << '\n';
            break;
        }
    }
}

bool ROB::CommitEntry() {
    int next = (head + 1) % (ROB_size + 1);
    if (!empty() && entry[head].ins.code == 0x0ff00513) {
        return true;
    }
    // std::cout << std::hex << entry[head].ins.index << '\n';
    if (!empty() && entry[head].state == State::BROADCAST) {
        entry[head].state = State::COMMIT;
        // std::cout << "Commit Instruction: " << std::hex << entry[head].ins.index << '\n';
        if (entry[head].type == DestType::REG) {
            CDBEntry rs_req{Hardware::ROB, Hardware::RS, TransferType::ModifyRecorder,
                head, entry[head].value, 0, 0};
            CDBEntry lsb_req{Hardware::ROB, Hardware::LSB, TransferType::ModifyRecorder,
                head, entry[head].value, 0, 0};
            CDBEntry regs_req{Hardware::ROB, Hardware::Register, TransferType::ModifyRecorder,
            head, entry[head].value, 0, entry[head].dest};
            cdb.AddRequirement(rs_req);
            cdb.AddRequirement(lsb_req);
            cdb.AddRequirement(regs_req);
        }
        if (entry[head].ins.BranchType()) {
            if (size() == 1 || size() > 1 && entry[next].ins.index != entry[head].pc_value) {
                unsigned int reversed_pc = entry[head].pc_value;
                CDBEntry rs_req{Hardware::ROB, Hardware::RS, TransferType::Clear,
                    0, 0, 0, 0};
                CDBEntry lsb_req{Hardware::ROB, Hardware::LSB, TransferType::Clear,
                    0, 0, 0, 0};
                CDBEntry regs_req{Hardware::ROB, Hardware::Register, TransferType::Clear,
                    0, 0, 0, 0};
                cdb.AddRequirement(rs_req);
                cdb.AddRequirement(lsb_req);
                cdb.AddRequirement(regs_req);
                clear();
                pc = reversed_pc;
                return false;
            }
        }
        deleteHead();
    }
    return false;
}


