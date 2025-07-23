#include <iostream>
#include "include/Memory.h"
#include "include/Register.h"
#include "include/ALU.h"
#include "include/InputManager.h"
#include "include/Execute.h"
#include "include/RS.h"
#include "include/ROB.h"
#include "include/LSB.h"


int clk = 0;
unsigned int pc = 0;
Memory memo{};
Register regs{};
ROB rob{};
RS rs{};
LSB lsb{};
ALU alu{};

int main() {
    ManageInput();
    while (true) {
        ++clk;
        Issue();
    }
}