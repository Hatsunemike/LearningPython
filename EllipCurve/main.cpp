#include <iostream>
#include "EllipticCurve.h"
#include "bigprime.h"
#include "test.h"

using std::cout;
using std::endl;

using boost::multiprecision::cpp_int;

int main() {
    testCipolla();
    return 0;
}