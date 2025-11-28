#include <stdio.h>
#include "EllipticCurve.h"
#include "bigprime.h"
#include "test.h"
#include <string>
#include <map>

#include "diyutility.h"

#include <openssl/bio.h>

using std::cout;
using std::endl;

void print_mpz(const m_type& data, const std::string name) {
    printf("%s: ", name.c_str());
    diy::print_hex(data);
    printf(";\n");
}

#include "serial.h"

int main() {
    testK2F_F2K();
    return 0;
}