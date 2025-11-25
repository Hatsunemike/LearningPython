#include "EllipticCurve.h"
#include <iostream>

// class Point

template<typename T>
inline T min(const T& a, const T& b) {return a<b?a:b;}

Point::Point(const mnum& _x, const mnum& _y) : x(_x), y(_y) {}

Point::Point(const Point& p) {
    x = p.x;
    y = p.y;
}

Point& Point::operator=(const Point& b) {
    this->x = b.x;
    this->y = b.y;
    return *this;
}

bool Point::operator==(const Point& b) const {
    return x == b.x && y == b.y;
}

bool Point::operator!=(const Point& b) const {
    return !((*this) == b);
}

Point Point::operator-() const {
    return Point(x, -y);
}

// class ECC_Context

ECC_Context ECC_Context_new() {
    ECC_Context ctx;
    ctx.a = 1;
    ctx.b = 1;
    ctx.p = int(1e9+7);
    return ctx;
}

//class EllipticCurve

mnum EllipticCurve::getA() {return this->a;}
mnum EllipticCurve::getB() {return this->b;}
m_type EllipticCurve::getP() {return this->a.getP();}
ECC_Context EllipticCurve::getCTX() {
    ECC_Context ctx;
    ctx.a = a.getX();
    ctx.b = b.getX();
    ctx.p = a.getP();
    return ctx;
}

EllipticCurve::EllipticCurve(const mnum& _a, const mnum& _b) {
    using std::cerr;
    using std::endl;

    if (_a.getP() != _b.getP()) {
        cerr << "Error: EllipticCurve: a, b must have same module! (a = (" 
            << a.getX() << ", " << a.getP() << "), b = ("
            << b.getX() << ", " << b.getP() << ")" << endl;
        mnum bb = mnum(_b.getX(), _a.getP());
        this->a = _a;
        this->b = bb;
    } else if (_a.getP() < 10) {
        cerr << "Error: EllipticCurve: the module is too small! (a.p = " << a.getP() << ")" << endl;
        this->a = mnum(a.getX(), int(1e9+7));
        this->b = mnum(b.getX(), int(1e9+7));
    }
    else {
        this->a = _a;
        this->b = _b;
    }
}

EllipticCurve::EllipticCurve(const m_type& _a, const m_type& _b, const m_type& _p) {
    this->a = mnum(_a, _p);
    this->b = mnum(_b, _p);
}

EllipticCurve::EllipticCurve(const ECC_Context& ctx) {
    this->a = mnum(ctx.a, ctx.p);
    this->b = mnum(ctx.b, ctx.p);
}

bool EllipticCurve::checkPoint(const Point& P) const {
    if (P.x.getP() != P.y.getP()) {
        return false;
    }
    if (P.y.getX() == 0) {   // The y cann't be zero to make sure each two points can be added.
        return false;
    }
    mnum left = P.y * P.y;
    mnum right = P.x * P.x * P.x + a * P.x + b;
    return left == right;
}

Point EllipticCurve::addPoints(const Point& P, const Point& Q) const {
    // printf("(%lld, %lld) + (%lld, %lld) = ", 
    //     P.x.getX(), P.y.getX(),
    //     Q.x.getX(), Q.y.getX());
    if (P.x == Q.x && P.y == Q.y) {
        mnum lambda = (3 * P.x * P.x + a) / (2 * P.y);
        mnum xR = lambda * lambda - 2 * P.x;
        mnum yR = lambda * (P.x - xR) - P.y;
        // printf("(%lld, %lld)\n", xR.getX(), yR.getX());
        return Point(xR, yR);
    } else if (P.x.getP() == P.y.getP()){
        mnum lambda = (Q.y - P.y) / (Q.x - P.x);
        mnum xR = lambda * lambda - P.x - Q.x;
        mnum yR = lambda * (P.x - xR) - P.y;
        // printf("(%lld, %lld)\n", xR.getX(), yR.getX());
        return Point(xR, yR);
    } else {
        return P;
    }
}

Point EllipticCurve::mulPoint(const Point& P, const m_type& k) const {
    const Point mp = Point(-P.x, P.y);
    const Point O = addPoints(P, mp);
    if(k<0) return mulPoint(mp, -k);
    if(k==0) return O;
    if(k==1) return P;
    Point res = P;
    Point ans = res;
    bool flag = false;
    m_type now = k;
    for(; now>0; now = now >> 1) {
        if (now & 1) {
            if(!flag) {
                ans = res;
                flag = true;
            } else {
                ans = addPoints(ans, res);
            }
        }
        res = addPoints(res, res);
    }
    return ans;
}

bool EllipticCurve::getYbyX(const mnum &x, mnum &y) const {
    mnum y2 = x*x*x + a*x + b;
    bool flag = Cipolla(y2, y);
    return flag;
}

bool EllipticCurve::getYbyX(const m_type& x, m_type& y) const {
    mnum _x(x, a.getP());
    mnum _y(y, a.getP());
    bool flag = getYbyX(_x, _y);
    y = _y.getX();
    return flag;
}

bool EllipticCurve::embedMessage(const m_type& msg , m_type& res_x, m_type& res_y, const int& k) const {
    m_type m = msg*(k);
    m_type y;
    for(int j=0;j<k;++j) {
        if(getYbyX(m, y)) {
            res_x = m;
            res_y = y;
            return true;
        }
        m++;
    }
    return false;
}

m_type EllipticCurve::getMessage(const m_type& p_x, const int& k) {
    return p_x/k;
}

m_type EllipticCurve::findLevelBrutely(const Point& p) const {
    m_type cnt = 0;
    Point res = p;
    boost:: unordered_set<Point, std::hash<Point> > s;
    for(; checkPoint(res) && (s.find(res) == s.end()); res = addPoints(res, p)) {
        s.insert(res);
        ++cnt;
    }
    return cnt;
}

bool EllipticCurve::findGenBrutely(const m_type& level, Point& ans) const {
    std::cout << "findGenBrutely start: level:" << level << std::endl;
    mnum x(0, a.getP());
    mnum y;
    for (m_type n=1; n<a.getP(); ++n) {
        bool flag = getYbyX(x, y);
        if(flag) {
            Point p(x,y);
            Point res(p);
            m_type cnt = 0;
            for(; checkPoint(res); res = addPoints(res, res)) {
                ++cnt;
                if(cnt >= level) {
                    ans = p;
                    std::cout << "findGenBrutely: found a generator: ("
                        << p.x.getX() << "," << p.y.getX() << ")." << std::endl;
                    return true;
                }
            }
            p = Point(x, -y);
            cnt = 0;
            for(res = p; checkPoint(res); res = addPoints(res, res)) {
                ++cnt;
                if(cnt >= level) {
                    ans = p;
                    return true;
                }
            }
        }
        x = x + 1;
    }
    std::cout << "findGenBrutely: cann't find a generator." << std::endl;
    return false;
}

/* The implementation of some commonly used ECC in cryptography. */

secp256k1Curve::secp256k1Curve(const m_type& p) : EllipticCurve(1, 1, p) {}

secp256k1Curve::~secp256k1Curve() {}