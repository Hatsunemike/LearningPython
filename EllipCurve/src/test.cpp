#include "test.h"
#include "EllipticCurve.h"

using std::list;
using std::cout;
using std::cerr;
using std::endl;

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

bool testMnumPow() {
    const int p = 7;
    mnum b(1, p);
    mnum res;
    for (int i=2;i<p;++i) {
        b.setX(i);
        res = mnum(1, p);
        for(int j=0;j<p;++j) {
            mnum bres = b.pow(j);
            cout << bres.getX() << endl;
            if (bres != res) {
                cerr << "\nError: pow(" << i << ", " << j << ") = " << res.getX()
                    << ", mnum.pow(" << i << ", " << j << ") = " << bres.getX() << endl;
                return false;
            }
            res *= b;
        }
        puts("");
    }
    return true;
}

bool testInv() {
    const int p = 1e9+7;
    for(int i=1; i<1000; ++i) {
        mnum a(i, p);
        auto res = a.getInv() * a;
        if(res.getX() != 1){
            cerr << "error: a=" << a.getX() << " inv=" << a.getInv().getX() << endl;
            return false;
        }
    }
    return true;
}

int getSqrt(int x, int p) {
    if(x==0) return 0;
    if(x==1) return 1;
    for(int i=2;i<p;++i) {
        if(i*i%p == x) {
            return i;
        }
    }
    return 0;
}

bool testCipolla() {
    m_type p = 1;
    p <<= 89; p -= 1;

    int cnt = 0;
    double start = clock(); start /= CLOCKS_PER_SEC;
    for(int i=1; i<=1000000; ++i) {
        mnum a = mnum(i, p);
        mnum b;
        if(Cipolla(a, b)) {
            ++cnt;
            if(b*b != a) {
                cerr << "error: " << b.getX() << "*" << b.getX() << "!=" << a.getX() << endl;
            }
            if (cnt%20000 == 0) {
                cout << "epoch: found " << cnt << " numbers."
                << " time use: " << double(clock())/CLOCKS_PER_SEC-start << " s." << endl;
            }
        }
    }
    cout << "cnt=" << cnt << endl;
    return true;
}

bool testEmbedMessage() {
    EllipticCurve e(3, 0, 4177);
    m_type y;
    bool flag = e.getYbyX(65235, y);
    if(flag) {
        cout << y << endl;
    } else {
        printf("Halo!\n");
    }
    return flag;
}

#include <cstdlib>

bool testFindGen() {
    int p = 31;
    printf("p=%d\n", p);
    EllipticCurve e(1, 1, p);
    int cnt = 0;
    for(int x=0; x<p; x++) {
        m_type y;
        if(e.getYbyX(x, y)) {
            Point P(mnum(x, p), mnum(y, p));
            cout << "level(" << x << ", " << y << ")=" << e.findLevel(P) << endl;
            P.y.setX(-y);
            cout << "level(" << x << ", " << p-y << ")=" << e.findLevel(P) << endl;
            ++cnt;
        }
    }
    printf("cnt = %d\n", cnt);
    return true;
}

bool testMulPoints() {
    int p = 7;
    EllipticCurve e(-2, -3, p);
    Point g(mnum(3, p), mnum(2, p));
    Point res = g;
    bool flag = true;
    for(int i=1;i<=10;++i) {
        Point ans = e.mulPoint(g, i);
        if(res != ans) {
            cout << "error: g^{" << i << "}=(" 
                << res.x.getX() << ", " << res.y.getX() << "), but result: ("
                << ans.x.getX() << ", " << ans.y.getX() << ")" << endl;
            flag = false;
        }
        res = e.addPoints(res, g);
    }
    return flag;
}