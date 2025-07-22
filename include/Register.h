#ifndef REGISTER_H
#define REGISTER_H
class Register {
    unsigned int reg[32]{};
public:

    Register() = default;

    unsigned int GetValue(const unsigned int &index) {
        return reg[index];
    }

    int GetSignedValue(const unsigned int &index) {
        return static_cast<int>(reg[index]);
    }

    void PutValue(const unsigned int &index, const unsigned int& val) {
        reg[index] = val;
    }
};
#endif //REGISTER_H
