#include "EllipticCurve.h"

Point::Point(const mnum& _x, const mnum& _y) : x(_x), y(_y) {}

Point& Point::operator=(const Point& b) {
    this->x = b.x;
    this->y = b.y;
    return *this;
}

EllipticCurve::EllipticCurve(const mnum& _a, const mnum& _b) : a(_a), b(_b) {}

bool EllipticCurve::checkPoint(const Point& P) const {
    mnum left = P.y * P.y;
    mnum right = P.x * P.x * P.x + a * P.x + b;
    return left == right;
}

Point EllipticCurve::addPoints(const Point& P, const Point& Q) const {
    if (P.x == Q.x && P.y == Q.y) {
        mnum lambda = (mnum(3) * P.x * P.x + a) / (mnum(2) * P.y);
        mnum xR = lambda * lambda - mnum(2) * P.x;
        mnum yR = lambda * (P.x - xR) - P.y;
        return Point(xR, yR);
    } else {
        mnum lambda = (Q.y - P.y) / (Q.x - P.x);
        mnum xR = lambda * lambda - P.x - Q.x;
        mnum yR = lambda * (P.x - xR) - P.y;
        return Point(xR, yR);
    }
}

