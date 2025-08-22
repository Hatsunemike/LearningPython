#include <string>
#include <list>

using std::string;

typedef unsigned int uint;
typedef uint vType;

typedef uint ERRType;
const ERRType NOERR = 0;
const ERRType ERR_IndexOut = 1;

class Rotor {
protected:
    uint num;
    vType* f;
    vType* f_1;
public:
    uint offset;
    Rotor(uint _num = 26, bool ranGen = true);
    ~Rotor();
    vType Encrypt(vType c);
    vType Decrypt(vType c);
    void rotate();
    void __print(const string tab = "");
};
typedef Rotor* PRotor;

class RotorGroup{
private:
    std:: list<PRotor> rotorList;
public:
    ~RotorGroup();
    void newRotor(uint _num = 26, bool ranGen = true);
    ERRType removeRotor(uint index);
    vType Encrypt(vType c);
    vType Decrypt(vType c);
    void rotateGroup();
    void __print(const string tab = "");
};

const uint chr_cnt = 26;
class Enigma {
private:
    RotorGroup rotors;
    uint plugBoard[chr_cnt];
    
    void initPlugBoard(bool ranGen = true);
public:
    Enigma(uint rotor_cnt = 3);
    vType En_Decrypt(vType c);
    void rotateRotorGroup();
    void __print(const string tab = "");
};