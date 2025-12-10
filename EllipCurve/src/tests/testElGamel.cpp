#include "test.h"
#include "ElGamel.h"

using std::cout;
using std::endl;

bool testElGamel() {
    ElGamelGenerator g;
    ElGamelSK sk;
    ElGamelPK pk;

    const char* str_msg = "3245676532457892346578264385726348956829374658927346895638974";
    m_type msg(str_msg);

    bool flag = g.genKey(sk, pk);
    if(!flag) {
        std::cerr << "generate key failed." << endl;
        return false;
    }

    ElGamelEncryptor encryptor(pk);
    ElGamelDecryptor decryptor(sk);
    auto pEncMsg  = encryptor.Encrypt(msg);
    if(pEncMsg == NULL) {
        fprintf(stderr, "encrypt failed.\n");
        return false;
    }
    auto res_msg = decryptor.Decrypt(*pEncMsg);
    delete pEncMsg;
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

    sk.writeTo(skf);
    pk.writeTo(pkf);

    fclose(skf);
    fclose(pkf);

    skf = fopen("./elgamel_sk.key", "rb");
    pkf = fopen("./elgamel_pk.key", "rb");

    ElGamelSK sk2;
    ElGamelPK pk2;

    sk2.readFrom(skf);
    pk2.readFrom(pkf);

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

bool testFileEncDec() {
    ElGamelSK sk;
    ElGamelPK pk;
    ElGamelGenerator g;

    g.genKey(sk, pk);

    ElGamelEncryptor encryptor(pk);
    char pfname[] = "Makefile";
    char cfname[] = "Makefile.enc";
    char pf2name[] = "Makefile2.txt";
    FILE* pf = fopen(pfname, "rb");
    FILE* cf = fopen(cfname, "wb");

    bool flag = encryptor.EncryptFile(pf, cf);
    if(!flag) {
        std::cerr << "error: EncryptFile Failed." << endl;
        return false;
    }
    fclose(pf); fclose(cf);

    ElGamelDecryptor decryptor(sk);
    pf = fopen(pf2name, "wb");
    cf = fopen(cfname, "rb");
    flag = decryptor.DecryptFile(cf, pf);
    if(!flag) {
        std::cerr << "error: DecryptFile Failed." << endl;
        return false;
    }

    // compare two pf
    FILE* pf2 = fopen(pf2name, "rb");
    pf = fopen(pfname, "rb");
    u_char c1, c2;
    size_t rd_bytes;
    while((rd_bytes = fread(&c1, sizeof(u_char), 1, pf))>0) {
        rd_bytes = fread(&c2, sizeof(u_char), 1, pf2);
        if(rd_bytes<1) {
            std::cerr << "error: two plain file have different lengths." << endl;
            return false;
        }
        if(c1 != c2) {
            std::cerr << "error: two plain file have different bytes." << endl;
            return false;
        }
    }
    return true;
}