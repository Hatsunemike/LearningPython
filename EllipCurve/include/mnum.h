#ifndef MNUM_H
#define MNUM_H

typedef long long m_type;

class mnum {
    m_type x;
    m_type p;
public:
    static void exgcd(m_type a, m_type b, m_type& x, m_type& y);
    m_type getP() const;
    m_type getX() const;
    void setX(const m_type& _x);

    mnum pow(m_type k) const;

    mnum(const m_type& _x = 0, const m_type& _p = 1e9+7);
    mnum(const mnum& other);
    mnum& operator=(const mnum& b);

    mnum getInv() const;

    friend mnum operator+(const mnum& a, const mnum& b);
    friend mnum operator-(const mnum& a, const mnum& b);
    friend mnum operator*(const mnum& a, const mnum& b);
    friend mnum operator/(const mnum& a, const mnum& b);

    bool operator==(const mnum& b) const;
    mnum operator+=(const mnum& b);
    mnum operator*=(const mnum& b);
};

#endif