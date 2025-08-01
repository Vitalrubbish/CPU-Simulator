#include "../include/Memory.h"
#include "../include/CDB.h"

extern unsigned int pc;
extern CDB cdb;
int tick = 0;

void Memory::Decode() {
    if (tick == 0) {
        Instruction ins{GetInstructionCode(pc), pc};
        // std::cout << "Memory - Decode Instruction: " << std::hex << pc << '\n';
        CDBEntry req{Hardware::Memory, Hardware::ROB, TransferType::SendInstruction, ins, -1};
        cdb.AddRequirement(req);
        tick = 1;
    } else {
        tick = 0;
    }
}
