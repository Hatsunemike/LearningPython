#include "diyutility.h"
#include <cstring>
#include <string>

int diy::readint() {
    char ch = getchar();
    int x = 0, f = 1;
    while (!isdigit(ch)) {
        if (ch == '-') f = -1;
        ch = getchar();
    }
    while (isdigit(ch)) {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    return x * f;
}

int diy::readint(const char* s) {
    int len = strlen(s);
    int i=0;

    int x=0, f=1;
    for(;i<len && (s[i]>'9'||s[i]<'0');++i)if(s[i] == '-')f = -1;
    for(;i<len && (s[i]>='0'&&s[i]<='9');++i)x = (x<<1)+(x<<3)+s[i]-'0';
    return x*f;
}

string diy::str(const Point& P, const EllipticCurve& e) {
    if(!e.checkPoint(P)) return "O";
    return "("+P.x.getX().str()+","+P.y.getX().str()+")";
}

using std::cout;
using std::endl;

int diy::demo_calcplus(int argc, char* argv[]) {
        for(int i=0; i<argc; ++i) {
        cout << "arg[" << i << "]: " << argv[i] << endl;
    }
    m_type p = 7;
    EllipticCurve e = EllipticCurve(-2, -3, p);
    Point P(mnum(3, p), mnum(2, p));

    int a = 0, b = 0;
    if(argc >= 3) {
        a = readint(argv[1]); b = readint(argv[2]);
    } else if(argc == 2) {
        a = readint(argv[1]);
    }
    cout << "a: " << a << " b: " << b << endl;
    Point A = e.mulPoint(P, a);
    Point B = e.mulPoint(P, b);
    cout << "A: " << str(A, e) << " B: " << str(B, e) << endl;
    Point Q = e.addPoints(A, B);
    cout << "A+B: " << str(Q, e) << endl;
    return 0;
}

void diy::e_1_1_23_new(EllipticCurve& e) {
    const m_type p = 23;
    e = EllipticCurve(1, 1, p);
}

void diy::e__2__3_7_new(EllipticCurve& e, Point& G) {
    const m_type p = 7;
    e = EllipticCurve(-2, -3, p);

    G = Point(mnum(3, p), mnum(2, p));
}