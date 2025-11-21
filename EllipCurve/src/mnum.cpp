#include "mnum.h"
#include <random>
#include <iostream>

void mnum::exgcd(m_type a, m_type b, m_type& x, m_type& y) {
    if(!b) {
        x = 1;
        y = 0;
        return;
    }
    exgcd(b, a%b, y, x);
    y -= (a/b)*x;
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

mnum& mnum::operator=(const m_type& b) {
    this->x = b;
    this->p = int(1e9+7);
    return *this;
}

mnum mnum::getInv() const {
    m_type xx,yy;
    mnum::exgcd(this->x,this->p,xx,yy);
    xx = (xx%p+p)%p;    // min natural number root ([0, p-1])
    return mnum(xx, this->p);
}

bool mnum::isQuadRes() const {
    if (pow((p-1)/2).getX() == 1)return true;
    return false;
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

mnum operator+(const m_type& a, const mnum& b) {
    m_type aa = (a%b.p+b.p)%b.p;
    return mnum((aa + b.x) % b.p, b.p);
}

mnum operator+(const mnum& a, const m_type& b) {
    m_type bb = (b%a.p+a.p)%a.p;
    return mnum(a.x + bb, a.p);
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

mnum operator-(const m_type& a, const mnum& b) {
    m_type aa = (a%b.p+b.p)%b.p;
    return mnum((aa - b.x + b.p) % b.p, b.p);
}

mnum operator-(const mnum& a, const m_type& b) {
    m_type bb = (b%a.p+a.p)%a.p;
    return mnum(a.x - bb, a.p);
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

mnum operator*(const m_type& a, const mnum& b) {
    m_type aa = (a%b.p+b.p)%b.p;
    return mnum(aa*b.x, b.p);
}

mnum operator*(const mnum& a, const m_type& b) {
    m_type bb = (b%a.p+a.p)%a.p;
    return mnum(a.x*bb, a.p);
}

mnum operator/(const mnum& a, const mnum& b) {
    if (a.p != b.p) {
        return mnum(1, a.p);
    }
    return a*b.getInv();
}

mnum operator/(const m_type& a, const mnum& b) {
    m_type aa = (a%b.p+b.p)%b.p;
    return aa*b.getInv();
}

mnum operator/(const mnum& a, const m_type& b) {
    mnum mb(b, a.p);
    return a*mb.getInv();
}

bool mnum::operator==(const mnum& b) const {
    return this->x == b.x && this->p == b.p;
}

bool mnum::operator!=(const mnum &b) const {
    return !(*this == b);
}

mnum& mnum::operator+=(const mnum& b) {
    if (this->p != b.getP()) {
        this->x = 0;
        return *this;
    }
    this->x = (this->x + b.getX()) % this->p;
    return *this;
}

mnum& mnum::operator+=(const m_type& b) {
    m_type bb = (b%p+p)%p;
    x = (x+bb)%p;
    return *this;
}

mnum& mnum::operator*=(const mnum& b) {
    if (this->p != b.getP()) {
        this->x = 1;
        return *this;
    }
    this->x = (this->x * b.getX()) % this->p;
    return *this;
}

mnum& mnum::operator*=(const m_type& b) {
    m_type bb = (b%p+p)%p;
    x = (x*bb)%p;
    return *this;
}

mnum mnum::operator-() const {
    return mnum(p-x, p);
}

// practical functions

struct complex{
    mnum real, imag;
    complex() {}
    complex(const mnum& _real, const mnum& _imag) : real(_real), imag(_imag){}
    complex(const complex& b): real(b.real), imag(b.imag) {}
    complex& operator=(const complex& b) {
        real = b.real;
        imag = b.imag;
        return *this;
    }
    complex mul(const complex& b, const mnum& i2) const {
        return complex(real*b.real + i2*imag*b.imag, real*b.imag+imag*b.real);
    }
    complex pow(const mnum& i2, m_type k) const {
        m_type p = real.getP();
        if (k==0) {
            return complex(mnum(1, p), mnum(0, p));
        }
        if (k==1) return *this;
        complex x(*this);
        complex ans(mnum(1, p), mnum(0, p));
        for(; k>0; k>>=1) {
            if(k&1) ans = ans.mul(x, i2);
            x = x.mul(x, i2);
        }
        return ans;
    }
};

bool Cipolla(const mnum& n, mnum& x) {
    m_type p = n.getP();
    if (n.getX() == 0) { 
        // Here, though x^2 \equiv n has solution, 0 isn't in the groop G_p, so 0 isn't QR.
        return false;
    }
    if (p == 2) {
        x = n;
        return true;
    }
    if (!n.isQuadRes()) return false;
    mnum a(n); a.setX(0);
    mnum i2;
    bool flag = 0;
    const int maxx = 30;
    for(int i=1;i<=maxx;++i) {
        a = a + 1;
        i2 = a*a - n;
        if(i2.getX() == 0) {
            x = a;
            return true;
        }
        bool res = i2.isQuadRes();
        if (!res) {
            flag = 1;
            break;
        }
    }
    if (!flag) {
        std:: cerr << "warning:: Cipolla:: " << maxx << " times of random number cann't find a non-QR number." << std::endl;
        return false;
    }
    // calc (a+i)^((p+1)/2);
    complex res = complex(a, mnum(1, p)).pow(i2, (p+1)/2);
    x = res.real;
    return true;
}