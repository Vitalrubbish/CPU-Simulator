#include <iostream>
#include <iomanip>
#include "include/Memory.h"
#include "include/Register.h"
#include "include/ALU.h"
#include "include/InputManager.h"
#include "include/Execute.h"
#include "include/RS.h"
#include "include/ROB.h"
#include "include/LSB.h"
#include "include/Predictor.h"
#include "include/CDB.h"
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
CDB cdb{};
unsigned int branch_count = 0;
unsigned int wrong_branch_count = 0;
bool logout = false;
bool cl = false;

int main() {
    freopen("../testcases/tak.data", "r", stdin);
    // freopen("../logrus.txt", "w", stdout);

    ManageInput();
    while (!logout) {
        ++clk;
        // std::cout << "clk = " << std::dec << clk << std::hex << " pc = " << pc << '\n';
        // std::cout << std::hex << pc << '\n';

        RS_run();
        LSB_run();
        Register_run();
        logout = ROB_run();


        /*if (clk % 1000 == 0) {
            sleep(1);
        }*/
        RefreshStage();

    }
    std::cout << std::dec << regs.GetValue(10) % 256 << '\n';

    // std::cout << std::setprecision(4) << 1 - static_cast<double>(wrong_branch_count) / branch_count << '\n';

    // std::cout << "Total clk count: " << clk << '\n';
}