#ifndef ELLIPTICCURVE_H
#define ELLIPTICCURVE_H

#include "mnum.h"
#include <boost/functional/hash.hpp>
#include <boost/unordered_set.hpp>
#include <vector>

/* Point class is used to store points on the ECC. */

class Point {
public:
    mnum x;
    mnum y;

    Point(const mnum& _x, const mnum& _y);
    Point(const Point& p);

    Point& operator=(const Point& b);
    bool operator==(const Point& b) const;
    bool operator!=(const Point& b) const;
    Point operator-() const;
};

namespace std{
    template<>
    struct hash<Point> {
        size_t operator()(const Point&p) const {
            size_t seed = 0;
            boost:: hash_combine(seed, p.x.getX());
            boost:: hash_combine(seed, p.y.getX());
            return seed;
        }
    };
}

/** 
 * ECC Context stores all information that should be assigned manually.
 * 
 * It includes (a,b,p) which identify a curve, 
 * and the encrypt exponent $k$ to control the encryption.
 */

struct ECC_Context{
    m_type a;
    m_type b;
    m_type p;

    /**
     * serilaize format:
     * <len_p (1 Byte)> <p> <len_a (1 Byte)> <a> <len_b (1 Byte)> <b>
     * p, a, b use the `serialize_mpz` method.
     */
    std::vector<u_char> serialize() const;
    /**
     * deserialize return the bytes number it reads.
     * return 0, means error.
     */
    size_t deserialize(const std::vector<u_char>& data) ;
};

/**
 * serialize format:
 * <len_n (1 byte)> <sign byte> <mpz_export(n)>
 * `len_n` is a unsigned interger, declare the length of `n` = `len_n` bytes = `len_n` * 8 bits.
 * If `n` is positive, <sign byte> is `0x00`, otherwise `0xff`.
 */
std::vector<u_char> serialize_mpz(const m_type& n);
/**
 * deserialize return the bytes number it reads.
 * return 0, means error.
 */
size_t deserialize(m_type&n, const std::vector<u_char>& data);

ECC_Context ECC_Context_new();

class EllipticCurve {
private:
    mnum a;
    mnum b;
public:
    mnum getA();
    mnum getB();
    m_type getP();
    ECC_Context getCTX();

    // This constructor isn't recommended.
    EllipticCurve(const mnum& _a, const mnum& _b);
    // Use this as possible. 
    EllipticCurve(const m_type& _a, const m_type& _b, const m_type& _p);
    // Using Context to construct
    EllipticCurve(const ECC_Context &ctx);

    bool checkPoint(const Point& P) const;
    Point addPoints(const Point& P, const Point& Q) const;
    Point mulPoint(const Point& P, const m_type& k) const;
    bool getYbyX(const mnum& x, mnum &y) const;
    bool getYbyX(const m_type& x, m_type& y) const;
    
    bool embedMessage(const m_type& msg, m_type& res_x, m_type& res_y, const int& k = 30) const;
    m_type getMessage(const m_type& p_x, const int& k = 30);
    
    m_type findLevelBrutely(const Point& p) const;
    bool findGenBrutely(const m_type& level, Point& ans) const;
    m_type countPointsBrutely();
};

/* The implementation of some commonly used ECC in cryptography. */

class secp256k1Curve: public EllipticCurve {
public:
    secp256k1Curve(const m_type& p);
    ~secp256k1Curve();
};
#endif // ELLIPTICCURVE_H