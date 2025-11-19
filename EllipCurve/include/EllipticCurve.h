#ifndef ELLIPTICCURVE_H
#define ELLIPTICCURVE_H

#include "mnum.h"
#include <boost/functional/hash.hpp>
#include <boost/unordered_set.hpp>

class Point {
public:
    mnum x;
    mnum y;

    Point(const mnum& _x, const mnum& _y);
    Point(const Point& p);

    Point& operator=(const Point& b);
    bool operator==(const Point& b) const;
    bool operator!=(const Point& b) const;
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

class EllipticCurve {
private:
    mnum a;
    mnum b;
public:
    mnum getA();
    mnum getB();
    m_type getP();

    // This constructor isn't recommended.
    EllipticCurve(const mnum& _a, const mnum& _b);
    // Use this as possible. 
    EllipticCurve(const m_type& _a, const m_type& _b, const m_type& _p);

    bool checkPoint(const Point& P) const;
    Point addPoints(const Point& P, const Point& Q) const;
    Point mulPoint(const Point& P, const m_type& k) const;
    bool getYbyX(const mnum& x, mnum &y) const;
    bool getYbyX(const m_type& x, m_type& y) const;
    
    bool embedMessage(const m_type& msg, const int& k, m_type& res_x, m_type& res_y) const;
    m_type findLevel(const Point& p) const;
    bool findGen(const m_type& level, Point& ans) const;
};
#endif // ELLIPTICCURVE_H