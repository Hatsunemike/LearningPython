#include "EllipticCurve.h"
#include "ElGamel.h"

#include <cstdlib>

std::vector<u_char> serialize_int(const int& n) {
    // fprintf(stderr, "n: %02x\n", n);

    std::vector<u_char> v;
    v.push_back((n>>24)&0xff);
    v.push_back((n>>16)&0xff);
    v.push_back((n>>8)&0xff);
    v.push_back(n&0xff);

    // fprintf(stderr, "v: ");
    // for(int i=0;i<4;++i) {
    //     fprintf(stderr, "%02x", v[i]);
    // }
    // fprintf(stderr, ".\n");
    return v;
}

int deserialize_int(int& n,const std::vector<u_char>& v) {
    if(v.size() < 4) {
        std::cerr << "Error: deserialize_int: the length is less than 4."  << std::endl;
        return 0;
    }
    n = (int(v[0])<<24)|(int(v[1])<<16)|(int(v[2])<<8)|int(v[3]);
    return 4;
}

// #include "diyutility.h"

std::vector<u_char> serialize_mpz(const m_type& n) {
    // fprintf(stderr, "n: ");
    // diy::print_hex(n);
    // fprintf(stderr, ".\n");

    std::vector<u_char> ret;
    size_t size;
    u_char* chs = (u_char*)mpz_export(nullptr, &size, 1, 1, 0, 0, n.backend().data());
    // insert sign byte
    u_char ss;
    if(n.sign() >= 0) ss = 0;
    else ss = 0xff;
    // fprintf(stderr, "ss: %02x\n", ss);
    ret.push_back(ss);
    // insert len
    ret.insert(ret.begin(), (u_char)size);
    // insert data
    ret.insert(ret.end(), chs, chs+size);

    // fprintf(stderr, "ret: ");
    // size_t ret_size = ret.size();
    // for(size_t i=0;i<ret_size;++i) {
    //     fprintf(stderr, "%02x", ret[i]);
    //     if(i>0 && i%4 == 0) fprintf(stderr, " ");
    // }
    // fprintf(stderr, ".\n");
    // std::cin.get();

    delete[] chs;
    return ret;
}

size_t deserialize_mpz(m_type& n, const std::vector<u_char>& data) {
    size_t now = 0;
    if(data.size() < now+1) {
        std::cerr << "Error: deserialize_mpz: no length." << std::endl;
        return 0;
    }
    data.at(0);
    u_char len = data[0];
    ++now;

    if(data.size() < now+len+1) {
        std::cerr << "Error: deserialize_mpz: data is shorter than designated by length field."
            << std::endl;
        return 0;
    }
    u_char sign = data[now];
    int f = 1;
    if(sign == 0xff) f = -1;
    n = 0;
    for(int i=1;i<=len;++i) {
        n = (n<<8) | data[now+i];
    }
    n *= f;
    now += len+1;

    return now;
}