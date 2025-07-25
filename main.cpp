#include <iostream>
#include "include/Memory.h"
#include "include/Register.h"
#include "include/ALU.h"
#include "include/InputManager.h"
#include "include/Execute.h"
#include "include/RS.h"
#include "include/ROB.h"
#include "include/LSB.h"
#include "include/Predictor.h"
#include <unistd.h>


int clk = 0;
unsigned int pc = 0;
Memory memo{};
Register regs{};
ROB rob{};
RS rs{};
LSB lsb{};
ALU alu{};
Predictor predictor{};
bool logout = false;

int main() {
    //freopen("../testcases/bulgarian.data", "r", stdin);
    //freopen("../logrus.txt", "w", stdout);
    ManageInput();
    while (!logout) {
        ++clk;
        //std::cout << "clk = " << std::dec << clk << std::hex << " pc = " << pc << '\n';
        //std::cout << std::hex << pc << '\n';
        Issue();
        Exec();
        Broadcast();
        logout = Commit();
    }
    std::cout << std::dec << regs.GetValue(10) % 256 << '\n';
}