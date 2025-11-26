#include <stdio.h>
#include "EllipticCurve.h"
#include "bigprime.h"
#include "test.h"
#include <string>
#include <map>

#include "diyutility.h"

using std::cout;
using std::endl;
using namespace diy;

int main() {
    const m_type p = 7;
    EllipticCurve e(-2, -3, p);
    cout << "points number: " << e.countPointsBrutely() << endl;
    
    Point P(mnum(5, p), mnum(0, p));
    cout << "level: " << e.findLevelBrutely(P) << endl;
    return 0;
}