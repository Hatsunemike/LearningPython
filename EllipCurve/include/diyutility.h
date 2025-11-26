#ifndef HEADER_FILE_H
#define HEADER_FILE_H

#include "EllipticCurve.h"
#include <string>

using std::string;

namespace diy{
    int readint();
    int readint(const char* s);
    std::string str(const Point& P, const EllipticCurve& e);
    int demo_calcplus(int argc, char* argv[]);
    void e_1_1_23_new(EllipticCurve& e);
    void e__2__3_7_new(EllipticCurve& e, Point& G);
}

#endif // HEADER_FILE_H