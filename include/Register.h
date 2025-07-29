#ifndef REGISTER_H
#define REGISTER_H


class Register {
    unsigned int reg[32]{};
    unsigned int phy_reg[32]{};
    int recorder[32]{};
public:

    Register() {
        for (int& it : recorder) {
            it = -1;
        }
    }

    unsigned int GetValue(const unsigned int &index) const;

    int GetSignedValue(const unsigned int &index) const {
        return static_cast<int>(reg[index]);
    }

    void PutValue(const unsigned int &index, const unsigned int& val) {
        reg[index] = val;
    }

    int GetRecorder(const unsigned int &index) const {
        return recorder[index];
    }

    void PutRecorder(const unsigned int &index, const int &val) {
        recorder[index] = val;
    }

    void ModifyRecorder(const unsigned int& index, const int& reco, const unsigned int& val);

    void clear() {
        for (int & it : recorder) {
            it = -1;
        }
    }

    void Issue();

    void CommitEntry();
};
#endif //REGISTER_H
