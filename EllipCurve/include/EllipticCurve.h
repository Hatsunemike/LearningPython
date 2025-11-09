#ifndef ELLIPTICCURVE_H
#define ELLIPTICCURVE_H

#include "mnum.h"

class Point {
public:
    mnum x;
    mnum y;

    Point(const mnum& _x, const mnum& _y);

    Point& operator=(const Point& b);
};

class EllipticCurve {
public:
    mnum a;
    mnum b;
    EllipticCurve(const mnum& _a, const mnum& _b);

    bool checkPoint(const Point& P) const;
    Point addPoints(const Point& P, const Point& Q) const;
    bool getYbyX(const mnum& x, mnum &y) const;
    
    Point embedMessage(const mnum& msg, const mnum& k = 30) const;
};
#endif // ELLIPTICCURVE_H