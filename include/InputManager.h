#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <iostream>
#include <string>
#include "../utility/utility.h"
#include "Memory.h"
inline void ManageInput() {
    std::string token;
    std::string recorder{};
    while (std::cin >> token) {
        if (token.length() == 9) {
            unsigned int index = HexToDec(token.substr(1, 8));
            memo.MoveCursor(index / 4);
        } else {
            recorder.insert(0, token);
            if (recorder.length() == 8) {
                unsigned int val = HexToDec(recorder);
                memo.InitStoreWord(val);
                recorder.clear();
            }
        }
    }
}
#endif //INPUTMANAGER_H
