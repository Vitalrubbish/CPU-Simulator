#include "../include/Memory.h"
#include "../include/CDB.h"

extern unsigned int pc;
extern CDB cdb;

void Memory::Issue() {
    Instruction ins{GetInstructionCode(pc), pc};
    CDBEntry req{Hardware::Memory, Hardware::ROB, TransferType::SendInstruction, ins, -1};
    cdb.AddRequirement(req);
}
