#include "bigprime.h"

bool MillarRabin(m_type p) {
    if(p<=100) {
        const int ps[25] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
        for(int i=0;i<25;++i) {
            if(ps[i] == p) {
                return 1;
            }
        }
        return 0;
    }
    m_type s = p-1;
    int cnt;
    for(cnt = 0; s>0; ++cnt) {
        if(s&1) break;
        s >>= 1;
    }
    m_type as[25] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
    for(int as_i=0; as_i<25; as_i++) {
        mnum a(as[as_i], p);
        a = a.pow(s);
        mnum b;
        for(int i=0;i<cnt;++i) {
            if(a.getX() == 1) {
                if(!i) break;
                else if(b.getX() == p-1) break;
                else return false;
            }
            b = a;
            a *= a;
        }
        if(a.getX() != 1)return false;
    }
    return true;
}

m_type GetBigPrime() {
    const int k = 30;
    m_type p = 1; p <<= k; p+=1;
    int cnt = 0;
    for(;!MillarRabin(p); p+=2) {
        ++cnt;
        if(cnt > 1000000) return 0;
    }
    return p;
}