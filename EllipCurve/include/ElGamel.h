#include "EllipticCurve.h"
#include <boost/random.hpp>
#include <utility>
#include <fstream>

#ifndef EL_GAMEL_H
#define EL_GAMEL_H

class ElGamelSK{
public:
    ECC_Context eCtx;
    m_type x;
    int k; // HashToX exponent

    std::vector<u_char> serialize() const;
    size_t deserialize(const std::vector<u_char>& data);

    int writeTo(FILE* f) const;
    int readFrom(FILE* f);
};

class ElGamelPK{
public:
    ECC_Context eCtx;
    m_type Gx;
    m_type Gy;
    m_type Yx;
    m_type Yy;
    m_type maxu; // the maximum u that sender can use.
    int k; // HashToX exponent

    std::vector<u_char> serialize() const;
    size_t deserialize(const std::vector<u_char>& data);

    int writeTo(FILE* f) const;
    int readFrom(FILE* f);
};

/**
 * The file/memory functions
 */

// vector<u_char> - file transformation
bool read_u_char_vec (FILE* f, std::vector<u_char>& v);
bool write_u_char_vec(FILE* f, const std::vector<u_char>& v);

size_t ElGamel_SK2File(FILE* f, const ElGamelSK& sk);
size_t ElGamel_PK2File(FILE* f, const ElGamelPK& pk);

bool ElGamel_File2SK(FILE* f, ElGamelSK& sk);
bool ElGamel_File2PK(FILE* f, ElGamelPK& pk);

class ElGamelCtx{
public:
    ECC_Context eCtx;
    m_type Gx; // generator x
    m_type Gy; // generator y
    m_type level; // ord(G)
    int k; // HashToX exponent
};

ElGamelCtx ElGamelCtx_secp256k1();

/**
 * ElGamelEncMsg: cipherText C=(C1, C2) in memory.
 */
class ElGamelEncMsg : public std::pair<Point, Point> {
public:
    ElGamelEncMsg(const std::pair<Point, Point>& a);
    ElGamelEncMsg& operator=(const std::pair<Point, Point>& a);

    std::vector<u_char> serialize();
    size_t deserialize(const std::vector<u_char>& data, const m_type& p);

    int writeTo(FILE* f) const;
    int readFrom(FILE* f, const m_type& p);
};

#define EL_GAMEL_DEFAULT_LEVEL 100000

m_type readFromArray(const u_char* arr, size_t len);
void readFromArray(const u_char* arr, size_t len, m_type& res);

class ElGamelGenerator{
private:
    boost::random::mt19937 rng; // random number generator
    EllipticCurve* e;
    Point Y;
    Point G;
    m_type level;
    int k;
    m_type x;
public:
    ElGamelGenerator();
    ElGamelGenerator(const ElGamelCtx& ctx);
    ~ElGamelGenerator();

    bool findGenBrutely();
    bool genKey(ElGamelSK& sk, ElGamelPK& pk);

    bool hasGen();
};

class ElGamelEncryptor{
    EllipticCurve* e;
    const m_type p;
    boost::random::mt19937 rng;
    boost::random::uniform_int_distribution<m_type> dist;
    int embed_k;
    Point* G;
    Point* Y;

    m_type Mx, My;
public:
    ElGamelEncryptor(const ElGamelPK& pk);
    ~ElGamelEncryptor();

    ElGamelEncMsg* Encrypt(const m_type& msg);
    bool EncryptFile(FILE* pf, FILE* cf);
};

class ElGamelDecryptor{
    EllipticCurve *e;
    const m_type p;
    const m_type x;
    const int embed_k;
    Point *C1;
    Point *C2;
public:
    ElGamelDecryptor(const ElGamelSK& sk);
    ~ElGamelDecryptor();

    m_type Decrypt(const ElGamelEncMsg& Msg);
    bool DecryptFile(FILE* cf, FILE* pf);
};

#endif