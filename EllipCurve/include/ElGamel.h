#include "EllipticCurve.h"
#include <boost/random.hpp>
#include <utility>

#ifndef EL_GAMEL_H
#define EL_GAMEL_H

class ElGamelSK{
public:
    ECC_Context eCtx;
    m_type x;
    int k; // HashToX exponent
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
};

class ElGamelCtx{
public:
    ECC_Context eCtx;
    m_type Gx; // generator x
    m_type Gy; // generator y
    m_type level; // ord(G)
    int k; // HashToX exponent
};

ElGamelCtx ElGamelCtx_secp256k1();

typedef std::pair<Point, Point> ElGamelEncMsg;

ElGamelEncMsg ElGamelEncrypt(const ElGamelPK& pk, const m_type& msg);
m_type ElGamelDecrypt(const ElGamelSK& sk, const ElGamelEncMsg& msg);

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
    void genKey(ElGamelSK& sk, ElGamelPK& pk);

    bool hasGen();
};

#endif