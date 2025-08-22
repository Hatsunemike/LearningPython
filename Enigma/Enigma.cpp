#include "Enigma.h"
#include <cstring>
#include <ctime>
#include <algorithm>
#include <random>
#include <iostream>

using std::string;
using std::cerr;
using std::cout;
using std::endl;

std:: mt19937 rng(std:: time(0));

// class Rotor

Rotor::Rotor(uint _num, bool ranGen): num(_num), offset(0) {
    f = new vType[num];
    for(int i=0;i<num;++i) f[i] = i;
    if(ranGen) {
        std:: shuffle(f, f+num, rng);
    }
    f_1 = new vType[num];
    for(int i=0;i<num;++i) {
        f_1[f[i]] = i;
    }
}

Rotor::~Rotor() {
    delete[] f;
    delete[] f_1;
}

vType Rotor::Encrypt(vType c) {
    c %= num;
    return f[(c+offset)%num];
}

vType Rotor::Decrypt(vType c) {
    c %= num;
    return (f_1[c]-offset+num)%num;
}

void Rotor::rotate() {
    offset++;
    if(offset == num) offset = 0;
}

void Rotor::__print(const string tab) {
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

// class RotorGroup

RotorGroup::~RotorGroup() {
    for(auto it = rotorList.begin(); it!=rotorList.end(); ++it) {
        delete *it;
    }
}

void RotorGroup::newRotor(uint _num, bool ranGen) {
    PRotor p = new Rotor(_num, ranGen);
    rotorList.push_back(p);
}

ERRType RotorGroup::removeRotor(uint index) {
    auto now = rotorList.begin();
    uint size = rotorList.size();
    for(int i=0;i<size;++i) now++;
    if (now == rotorList.end()) {
        return ERR_IndexOut;
    }
    delete *now;
    rotorList.erase(now);
}

vType RotorGroup::Encrypt(vType c) {
    auto it = rotorList.begin();
    for(; it!=rotorList.end(); ++it) {
        c = (*it)->Encrypt(c);
    }
    return c;
}

vType RotorGroup::Decrypt(vType c) {
    auto it = rotorList.rbegin();
    for(; it!=rotorList.rend(); ++it) {
        c = (*it)->Decrypt(c);
    }
    return c;
}

void RotorGroup::rotateGroup() {
    if(rotorList.size() == 0) return;
    auto it = rotorList.begin();
    (*it)->rotate();
    while((*it)->offset == 0) {
        ++it;
        if(it == rotorList.end())break;
        (*it)->rotate();
    }
}

void RotorGroup::__print(const string tab) {
    int cnt=0;
    for(auto it = rotorList.begin(); it!=rotorList.end(); it++) {
        cerr << tab << "rotor index: " << cnt << endl;
        cerr << tab << "rotor object info: " << endl;
        (*it)->__print(tab+"  ");
    }
}

// class Enigma

void Enigma::initPlugBoard(bool ranGen) {
    for(uint i=0;i<chr_cnt;++i) {
        plugBoard[i] = i;
    }
    if(ranGen) {
        char vis[chr_cnt];
        memset(vis, 0, sizeof(vis));
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

Enigma::Enigma(uint rotor_cnt): rotors(){
    initPlugBoard();
    for(int i=0;i<rotor_cnt;++i)rotors.newRotor(chr_cnt, true);
}

vType Enigma::En_Decrypt(vType c) {
    c = rotors.Encrypt(c);
    c = plugBoard[c];
    c = rotors.Decrypt(c);
    return c;
}

void Enigma::rotateRotorGroup() {
    rotors.rotateGroup();
}

void Enigma::__print(const string tab) {
    cerr << tab << "plugBoard:";
    for(int i=0;i<chr_cnt;++i) {
        cerr << plugBoard[i] << " ";
    }
    cerr << endl;
    cerr << tab << "rotors object info:" << endl;
    rotors.__print(tab+"  ");
}