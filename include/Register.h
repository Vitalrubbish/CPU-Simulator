#ifndef REGISTER_H
#define REGISTER_H
class Register {
    unsigned int reg[32]{};
    int recorder[32]{};
public:

    Register() {
        for (int& it : recorder) {
            it = -1;
        }
    }

    unsigned int GetValue(const unsigned int &index) const {
        return reg[index];
    }

    int GetSignedValue(const unsigned int &index) const {
        return static_cast<int>(reg[index]);
    }

    void PutValue(const unsigned int &index, const unsigned int& val) {
        reg[index] = val;
    }
};
#endif //REGISTER_H
