#include "test.h"

using std::list;

void Oula(char* isPrime, const int maxn) {
    list<int> primes;
    memset(isPrime, 1, (maxn+1)*sizeof(char));
    isPrime[0] = isPrime[1] = 0;
    for(int i=2;i<=maxn;++i) {
        if(isPrime[i]) primes.push_back(i);
        for(auto it = primes.begin(); it != primes.end(); ++it) {
            int j = i*(*it);
            if(j>maxn) break;
            isPrime[j] = 0;
            if(i % (*it) == 0) break;
        }
    }
}

bool testMillarRabin() {
    const int maxn = 10000000;
    char* isPrime = new char[maxn+10];

    double start = clock();
    Oula(isPrime, maxn);
    double now = clock();
    printf("oula times: %f ms\n", now-start);
    start = now;
    bool flag = true;
    for(int i=1;i<=maxn;++i) {
        if(MillarRabin(i) ^ isPrime[i]) {
            flag = false;
            printf("Millar(%d) = %d, isPrime[%d] = %d\n", i, MillarRabin(i), i, isPrime[i]);
        }
    }
    puts("");
    now = clock();
    printf("millar times: %f ms\n", now - start);
    delete[] isPrime;
    return flag;
}