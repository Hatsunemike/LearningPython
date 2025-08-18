#include <cstdio>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <list>
#include <random>
#include <string>
#include <iostream>

using std::string;
using std::cerr;
using std::endl;

typedef unsigned int uint;
typedef uint vType;

typedef uint ERRType;
const ERRType NOERR = 0;
const ERRType ERR_IndexOut = 1;

class Rotor{
protected:
    uint num; // 映射定义域大小
    vType* f; // 映射
    vType* f_1; // 反向映射集
public:
    uint offset; // 偏移量
    Rotor(uint _num = 26, bool ranGen = true): num(_num), offset(0) {
        f = new vType[num];
        for(int i=0;i<num;++i) f[i] = i;
        if(ranGen) {
            std:: mt19937 rng(std:: time(0));
        std:: shuffle(f, f+num, rng);
        }
        f_1 = new vType[num];
        for(int i=0;i<num;++i) {
            f_1[f[i]] = i;
        }
    }

    ~Rotor() {
        delete[] f;
        delete[] f_1;
    }

    vType Encrypt(vType c) {
        c %= num;
        return f[(c+offset)%num];
    }

    vType Decrypt(vType c) {
        c %= num;
        return (f_1[c]-offset+num)%num;
    }

    void rotate() {
        offset++;
        if(offset == num) offset = 0;
    }

    void __print(const string tab) {
        cerr << tab << "num: " << num << endl;
        cerr << tab << "f: ";
        for(int i=0;i<num;++i) {
            cerr << f[i] << " ";
        }
        cerr << endl;
        cerr << tab << "f_1: ";
        for(int i=0;i<num;++i) {
            cerr << f_1[i] << " ";
        }
        cerr << endl;
    }
};
typedef Rotor* PRotor;

class RotorGroup{
private:
    std:: list<PRotor> rotorList;
public:
    ~RotorGroup() {
        for(auto it = rotorList.begin(); it!=rotorList.end(); ++it) {
            delete *it;
        }
    }

    void newRotor(uint _num = 26, bool ranGen = true) {
        PRotor p = new Rotor(_num, ranGen);
        rotorList.push_back(p);
    }

    ERRType removeRotor(uint index) {
        auto now = rotorList.begin();
        uint size = rotorList.size();
        for(int i=0;i<size;++i) now++;
        if (now == rotorList.end()) {
            return ERR_IndexOut;
        }
        delete *now;
        rotorList.erase(now);
    }

    vType Encrypt(vType c) {
        auto it = rotorList.begin();
        for(; it!=rotorList.end(); ++it) {
            c = (*it)->Encrypt(c);
        }
        return c;
    }

    vType Decrypt(vType c) {
        auto it = rotorList.rbegin();
        for(; it!=rotorList.rend(); ++it) {
            c = (*it)->Decrypt(c);
        }
        return c;
    }

    void rotateGroup() {
        if(rotorList.size() == 0) return;
        auto it = rotorList.begin();
        (*it)->rotate();
        while((*it)->offset == 0) {
            ++it;
            if(it == rotorList.end())break;
            (*it)->rotate();
        }
    }

    void __print(const string tab = "") {
        int cnt=0;
        for(auto it = rotorList.begin(); it!=rotorList.end(); it++) {
            printf("rotor index: %d\n", cnt);
            printf("rotor object info: ");
            (*it)->__print(tab+"  ");
        }
    }
};

const uint chr_cnt = 26;
class Enigma{
private:
    RotorGroup rotors;
    uint plugBoard[chr_cnt];

    void initPlugBoard(bool ranGen = true) {
        for(uint i=0;i<chr_cnt;++i) {
            plugBoard[i] = i;
        }
        if(ranGen) {
            char vis[chr_cnt];
            memset(vis, 0, sizeof(vis));
            std:: mt19937 rng(time(0));
            std:: uniform_int_distribution<uint> dist(0,25);
            uint a,b,c;
            for(uint i=0;i<10;++i) {
                do{
                    a = dist(rng);
                }while(vis[a]);
                do{
                    b = dist(rng);
                }while(a==b || vis[b]);
                c = plugBoard[a];
                plugBoard[a] = plugBoard[b];
                plugBoard[b] = c;
                vis[a] = vis[b] = 1;
            }
        }
    }
public:
    Enigma(uint rotor_cnt = 3): rotors(){
        initPlugBoard();
        // for(int i=0;i<rotor_cnt;++i)rotors.newRotor(chr_cnt, true);
    }

    vType En_Decrypt(vType c) {
        c = rotors.Encrypt(c);
        c = plugBoard[c];
        c = rotors.Decrypt(c);
        return c;
    }

    void rotateRotorGroup() {
        rotors.rotateGroup();
    }

    void __print(const string tab = "") {
        cerr << tab << "plugBoard:";
        for(int i=0;i<chr_cnt;++i) {
            cerr << plugBoard[i] << " ";
        }
        cerr << endl;
        cerr << tab << "rotors object info:" << endl;
        rotors.__print(tab+"  ");
    }
};

int main()
{
    Enigma e;
    cerr << "=========== (begin)e info ========" << endl;
    e.__print();
    cerr << "=========== (end)e info ==========" << endl;
    char* s = "hellohappyworld";
    printf("original string: %s\n",s);
    uint len = strlen(s);
    for(int i=0;i<len;++i)s[i] -= 'a';
    char* e_s = new char[len];
    for(int i=0;i<len;++i) {
        e_s[i] = e.En_Decrypt(s[i]) + 'a';
        e.rotateRotorGroup();
    }
    printf("encypted string: %s\n", e_s);
    delete[] e_s;
    return 0;
}