#ifndef MNUM_H
#define MNUM_H

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/gmp.hpp>

typedef boost::multiprecision::mpz_int m_type;

class mnum {
    m_type x;
    m_type p;
public:
    static void exgcd(m_type a, m_type b, m_type& x, m_type& y);
    m_type getP() const;
    m_type getX() const;
    void setX(const m_type& _x);

    mnum pow(m_type k) const;

    mnum(const m_type& _x = long(0), const m_type& _p = long(1e9+7));
    mnum(const mnum& other);
    mnum& operator=(const mnum& b);
    mnum& operator=(const m_type& b);

    mnum getInv() const;
    bool isQuadRes() const;

    friend mnum operator+(const mnum& a, const mnum& b);
    friend mnum operator+(const m_type& a, const mnum& b);
    friend mnum operator+(const mnum& a, const m_type& b);
    friend mnum operator-(const mnum& a, const mnum& b);
    friend mnum operator-(const m_type& a, const mnum& b);
    friend mnum operator-(const mnum& a, const m_type& b);
    friend mnum operator*(const mnum& a, const mnum& b);
    friend mnum operator*(const m_type& a, const mnum& b);
    friend mnum operator*(const mnum& a, const m_type& b);
    friend mnum operator/(const mnum& a, const mnum& b);
    friend mnum operator/(const m_type& a, const mnum& b);
    friend mnum operator/(const mnum& a, const m_type& b);

    bool operator==(const mnum& b) const;
    bool operator!=(const mnum& b) const;
    mnum& operator+=(const mnum& b);
    mnum& operator+=(const m_type& b);
    mnum& operator*=(const mnum& b);
    mnum& operator*=(const m_type& b);

    mnum operator-() const;
};

// practical functions

/** Cipolla Algorithm.
 *   Solve the equation $x^2 \equiv n \mod p$
 *   return true if there is a solution, and x will store it.
 *   if $n \equiv 0 \mod p$ or there isn't a solution, return false and x won't change.
*/
bool Cipolla(const mnum& n, mnum& x);

#endif