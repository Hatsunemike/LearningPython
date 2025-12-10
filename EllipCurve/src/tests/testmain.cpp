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
    diy::print_hex(data, stdout);
    printf(";\n");
}

void print_u_char_vec(const std::vector<u_char>& v) {
    size_t len = v.size();
    for(size_t i=0; i<len; ++i) {
        if(i>0 && i%4==0) printf(" ");
        if(i>0 && i%16==0) printf("\n");
        printf("%02x", v[i]);
    }
    puts("");
}

int main() {
    testFileEncDec();
    return 0;
}