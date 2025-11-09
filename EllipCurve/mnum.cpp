#include "mnum.h"

void mnum::exgcd(m_type a, m_type b, m_type& x, m_type& y) {
    if(!b) {
        x = 1;
        y = 0;
        return;
    }
    exgcd(b, a%b, y, x);
    y -= a/b;
}

m_type mnum::getP() const {
    return this->p;
}

m_type mnum::getX() const {
    return x;
}

void mnum::setX(const m_type& _x) {
    x = (_x % p + p) % p;
}

mnum mnum::pow(m_type k) const {
    if(k==0) {
        return mnum(1, this->p);
    }
    else if(k==1) {
        return *this;
    }
    int maxx = sizeof(m_type)*8;
    mnum res = *this;
    mnum ans(1, this->getP());
    for (int i=0;i<maxx;++i) {
        if(k&1) {
            ans *= res;
        }
        if(!k) break;
        k >>= 1;
        res = res*res;
    }
    return ans;
}

mnum::mnum(const m_type& _x, const m_type& _p) {
    p = _p;
    x = (_x % p + p) % p;
}
mnum::mnum(const mnum& other) : x(other.x), p(other.p) {}

mnum& mnum::operator=(const mnum& b) {
    this->x = b.x;
    this->p = b.p;
    return *this;
}

mnum mnum::getInv() const {
    m_type xx,yy;
    mnum::exgcd(this->x,this->p,xx,yy);
    return mnum(xx, this->p);
}

mnum operator+(const mnum& a, const mnum& b) {
    if (a.p != b.p) {
        return mnum(0, a.p);
    }
    mnum res;
    res.x = (a.x + b.x) % a.p;
    res.p = a.p;
    return res;
}
mnum operator-(const mnum& a, const mnum& b) {
    if (a.p != b.p) {
        return mnum(0, a.p);
    }
    mnum res;
    res.x = (a.x - b.x + a.p) % a.p;
    res.p = a.p;
    return res;
}
mnum operator*(const mnum& a, const mnum& b) {
    if (a.p != b.p) {
        return mnum(1, a.p);
    }
    mnum res;
    res.x = (a.x * b.x) % a.p;
    res.p = a.p;
    return res;
}
mnum operator/(const mnum& a, const mnum& b) {
    if (a.p != b.p) {
        return mnum(1, a.p);
    }
    mnum res;
    res.x = a.x * b.getInv().x % a.p;
    res.p = a.p;
    return res;
}

bool mnum::operator==(const mnum& b) const {
    return this->x == b.x && this->p == b.p;
}

mnum mnum::operator+=(const mnum& b) {
    if (this->p != b.getP()) {
        this->x = 0;
        return *this;
    }
    this->x = (this->x + b.getX()) % this->p;
    return *this;
}

mnum mnum::operator*=(const mnum& b) {
    if (this->p != b.getP()) {
        this->x = 1;
        return *this;
    }
    this->x = (this->x * b.getX()) % this->p;
    return *this;
}