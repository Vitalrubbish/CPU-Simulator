#ifndef REGISTER_H
#define REGISTER_H


class Register {
    unsigned int reg[32]{};
    int recorder[32]{};
    int old_recorder[32]{};
public:

    Register() {
        for (int& it : recorder) {
            it = -1;
        }
        for (int& it : old_recorder) {
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

    int GetOldRecorder(const unsigned int &index) const {
        return old_recorder[index];
    }

    void PutRecorder(const unsigned int &index, const int &val) {
        old_recorder[index] = recorder[index];
        recorder[index] = val;
    }

    void ModifyRecorder(const unsigned int& index, const int& reco, const unsigned int& val);

    void clear() {
        for (int & it : recorder) {
            it = -1;
        }
        for (int& it : old_recorder) {
            it = -1;
        }
    }

    bool Issue();

    void CommitEntry();

    bool Clear();
};
#endif //REGISTER_H
