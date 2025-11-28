#include "test.h"
#include "ElGamel.h"

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

bool testSerialization() {
    ElGamelGenerator g;
    ElGamelSK sk;
    ElGamelPK pk;

    g.genKey(sk, pk);

    auto sks = sk.serialize();
    ElGamelSK sk2;
    sk2.deserialize(sks);
    auto sks2 = sk2.serialize();

    if(sks != sks2) {
        cout << "pri-key serialize and deserialize test failed." << endl;
        return false;
    }

    auto pks = pk.serialize();
    ElGamelPK pk2;
    pk2.deserialize(pks);
    auto pks2 = pk2.serialize();

    if(pks != pks2) {
        cout << "pub-key serialize and deserialize test failed." << endl;
        return false;
    }

    cout << "serialization test passed." << endl;
    return true;
}

bool testK2F_F2K() {
    ElGamelGenerator g;
    ElGamelSK sk;
    ElGamelPK pk;
    g.genKey(sk, pk);

    FILE* skf = fopen("./elgamel_sk.key", "wb");
    FILE* pkf = fopen("./elgamel_pk.key", "wb");

    ElGamel_SK2File(skf, sk);
    ElGamel_PK2File(pkf, pk);

    fclose(skf);
    fclose(pkf);

    skf = fopen("./elgamel_sk.key", "rb");
    pkf = fopen("./elgamel_pk.key", "rb");

    ElGamelSK sk2;
    ElGamelPK pk2;

    ElGamel_File2SK(skf, sk2);
    ElGamel_File2PK(pkf, pk2);

    if(sk.serialize() != sk2.serialize() || pk.serialize() != pk2.serialize()) {
        fprintf(stderr, "test K2F_F2K failed.\n");
        perror("file error: ");
        return false;
    }

    fclose(skf);
    fclose(pkf);

    fprintf(stderr, "test K2F_F2K passed.\n");
    return true;
}