#include "test.h"

using std::cout;
using std::endl;

bool testElGamel() {
    ElGamelGenerator g;
    ElGamelSK sk;
    ElGamelPK pk;

    const char* str_msg = "3245676532457892346578264385726348956829374658927346895638974";
    m_type msg = 0;
    int len = strlen(str_msg);

    for(int i=0;i<len;++i) {
        msg = (msg*10) + (str_msg[i] - '0');
    }

    g.genKey(sk, pk);

    const m_type t = 1;
    const m_type p = (t<<256) - (t<<32) - (t<<9) - (t<<8) - (t<<7) - (t<<6) - (t<<4) - 1;
    EllipticCurve e(0, 7, p);
    Point G(mnum(pk.Gx, p), mnum(pk.Gy, p));
    // cout << "x: " << sk.x << endl;
    Point Y1 = e.mulPoint(G, sk.x);

    if((Y1.x.getX() != pk.Yx) || (Y1.y.getX() != pk.Yy)) {
        cout << "key failed." << endl;
        cout << "Y(" << pk.Yx << "," << pk.Yy << ")." << endl;
        cout << "Y1(" << Y1.x.getX() << "," << Y1.y.getX() << ")." << endl;
    }

    auto enc_msg = ElGamelEncrypt(pk, msg);
    auto res_msg = ElGamelDecrypt(sk, enc_msg);
    cout << "msg:     " << msg     << endl;
    cout << "res_msg: " << res_msg << endl;
    return true;
}