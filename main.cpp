#include <iostream>
#include "include/Memory.h"
#include "include/Register.h"
#include "include/Executor.h"
#include "include/InputManager.h"

Memory memo{};
Register regs{};

int main() {
    ManageInput();
    unsigned int ret = Execute();
    std::cout << ret << '\n';
}