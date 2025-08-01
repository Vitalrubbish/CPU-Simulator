#ifndef PREDICTOR_H
#define PREDICTOR_H
#include <utility>
#include "Instruction.h"
#include "Register.h"
#include <cstdint>

extern unsigned int pc;
const int len = 1024;
const int history_length = 64;
const int MINW = -8;
const int MAXW = 7;

/*class Predictor {
    uint64_t history[len]{};
    int weight[len][history_length]{};
    int theta = 137;
    int bias[len]{};

    int GetSum(unsigned int index) const {
        int sum = bias[index];
        for (int i = 0; i < history_length; i++) {
            if (history[index] & (1 << i)) {
                sum += weight[index][i];
            } else {
                sum -= weight[index][i];
            }
        }
        return sum;
    }

public:

    Predictor() {
        for (auto& it: history) {
            it = 0x0f1e2d3c4b5a6978;
        }
    }

    bool predict(unsigned int index) const {
        if (GetSum(index % len) > 0) {
            return true;
        }
        return false;
    }

    void update(bool result, bool predict_result, unsigned int index_) {
        uint32_t index = index_ % len;
        if (result) {
            history[index] = (history[index] << 1) + 1;
        } else {
            history[index] = history[index] << 1;
        }
        int sum = GetSum(index);
        if (result != predict_result || abs(sum) < theta) {
            if (result) {
                bias[index] = std::min(bias[index] + 1, MAXW);
            } else {
                bias[index] = std::max(bias[index] - 1, MINW);
            }
            for (int i = 0; i < history_length; i++) {
                bool bit = history[index] & (1 << i);
                if (bit) {
                    if (result) {
                        weight[index][i] = std::min(weight[index][i] + 1, MAXW);
                    } else {
                        weight[index][i] = std::max(weight[index][i] - 1, MINW);
                    }
                } else {
                    if (result) {
                        weight[index][i] = std::max(weight[index][i] - 1, MINW);
                    } else {
                        weight[index][i] = std::min(weight[index][i] + 1, MAXW);
                    }
                }
            }
        }
    }
};*/

const int pht_size = 1024;
const int selector_size = 1024;

class SatCounter {
    int val = 1;
public:
    SatCounter() = default;

    void update(bool flag) {
        if (flag) {
            val = std::min(3, val + 1);
        } else {
            val = std::max(0, val - 1);
        }
    }

    bool decide() const {
        return val > 1;
    } // 0,1 for local predictor, 2,3 for global predictor
};

class BasicPredictor {
    SatCounter pht[pht_size];
    uint32_t history = 0x0f1e2d3c4b5a6978;
public:
    bool last_result{};

    BasicPredictor() = default;

    bool predict(uint32_t pc) {
        uint32_t index = (pc ^ history) % pht_size;
        last_result = pht[index].decide();
        return last_result;
    }

    void update(uint32_t pc, bool taken) {
        uint32_t index = (pc ^ history) % pht_size;
        pht[index].update(taken);
        history = (history << 1) + taken;
    }
};

class Predictor {
    BasicPredictor globalPredictor;
    BasicPredictor localPredictor[selector_size];
    SatCounter selector[selector_size];
public:
    Predictor() = default;

    bool predict(uint32_t pc) {
        uint32_t index = pc % selector_size;
        if (selector[index].decide()) {
            return globalPredictor.predict(pc);
        }
        return localPredictor[index].predict(pc);
    }

    void update(uint32_t pc, bool taken) {
        uint32_t index = pc % selector_size;
        globalPredictor.update(pc, taken);
        localPredictor[index].update(pc, taken);
        if (globalPredictor.last_result != localPredictor[index].last_result) {
            if (globalPredictor.last_result == taken) {
                selector[index].update(true);
            } else {
                selector[index].update(false);
            }
        }
    }
};
#endif //PREDICTOR_H
