#include "Enigma.h"
#include <iostream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;

int main()
{
    Enigma e;
    cerr << "=========== (begin)e info ========" << endl;
    e.__print();
    cerr << "=========== (end)e info ==========" << endl;
    string s = "hellohappyworld";
    cout << "original string: " << s << endl;
    uint len = s.size();
    for(int i=0;i<len;++i)s[i] -= 'a';
    char* e_s = new char[len];
    for(int i=0;i<len;++i) {
        e_s[i] = e.En_Decrypt(s[i]) + 'a';
        e.rotateRotorGroup();
    }
    cout << "encrypted string: " << e_s << endl;
    delete[] e_s;
    return 0;
}