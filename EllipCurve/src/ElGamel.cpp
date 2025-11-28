#include "ElGamel.h"
#include "bigprime.h"
#include "serial.h"
#include <chrono>

using std::cerr;
using std::endl;

/* pri-key , pub-key serilize and deserialize functions. */

std::vector<u_char> ElGamelSK::serialize() const {
    std::vector<u_char> ret;
    // serialize eCtx
    auto t = eCtx.serialize();
    ret.insert(ret.end(), t.begin(), t.end());

    // serialize x
    t = serialize_mpz(x);
    ret.insert(ret.end(), t.begin(), t.end());

    // serialize k
    t = serialize_int(k);
    ret.insert(ret.end(), t.begin(), t.end());

    return ret;
}

size_t ElGamelSK::deserialize(const std::vector<u_char>& data) {
    size_t now = 0;
    size_t rd_bytes;

    rd_bytes = eCtx.deserialize(data);
    if(rd_bytes == 0) {
        std::cerr << "Error: ElGamelSK.deserialize: eCtx.deserialize failed" << std::endl;
        return 0;
    }
    now += rd_bytes;

    rd_bytes = deserialize_mpz(x, std::vector<u_char>(data.begin()+now, data.end()));
    if(rd_bytes == 0) {
        std::cerr << "Error: ElGamelSK.deserialize: private key x deserialize failed."
            << std::endl;
        return 0;
    }
    now += rd_bytes;

    deserialize_int(k, std::vector<u_char>(data.begin()+now, data.end()));
    now += 4;

    return now;
}

std::vector<u_char> ElGamelPK::serialize() const {
    std::vector<u_char> ret;
    std::vector<u_char> t;

    t = eCtx.serialize();
    ret.insert(ret.end(), t.begin(), t.end());

    t = serialize_mpz(Gx);
    ret.insert(ret.end(), t.begin(), t.end());

    t = serialize_mpz(Gy);
    ret.insert(ret.end(), t.begin(), t.end());

    t = serialize_mpz(Yx);
    ret.insert(ret.end(), t.begin(), t.end());

    t = serialize_mpz(Yy);
    ret.insert(ret.end(), t.begin(), t.end());

    t = serialize_mpz(maxu);
    ret.insert(ret.end(), t.begin(), t.end());

    t = serialize_int(k);
    ret.insert(ret.end(), t.begin(), t.end());
    
    return ret;
}

size_t ElGamelPK::deserialize(const std::vector<u_char>& data) {
    size_t now = 0;
    size_t add = 0;
    std::vector<u_char> tmp;

    add = eCtx.deserialize(data);
    if(add == 0) goto err_process;
    now += add;

    tmp.assign(data.begin()+now, data.end());
    add = deserialize_mpz(Gx,tmp);
    if(add == 0) goto err_process;
    now += add;

    tmp.assign(data.begin()+now, data.end());
    add = deserialize_mpz(Gy,tmp);
    if(add == 0) goto err_process;
    now += add;

    tmp.assign(data.begin()+now, data.end());
    add = deserialize_mpz(Yx,tmp);
    if(add == 0) goto err_process;
    now += add;

    tmp.assign(data.begin()+now, data.end());
    add = deserialize_mpz(Yy,tmp);
    if(add == 0) goto err_process;
    now += add;

    tmp.assign(data.begin()+now, data.end());
    add = deserialize_mpz(maxu,tmp);
    if(add == 0) goto err_process;
    now += add;

    tmp.assign(data.begin()+now, data.end());
    add = deserialize_int(k,tmp);
    if(add == 0) goto err_process;
    now += add;

    return now;

err_process:
    std::cerr << "Error: ElGamelPK: deserialize failed." << endl;
    return 0;
}

/**
 * file/memory transmitting functions.
 */

const size_t KF_BUFFER_SIZE = 2048;

#include <vector>
#include <cstdio>
#include <algorithm> // std::min

size_t ElGamel_SK2File(FILE* f, const ElGamelSK& sk) {
    const std::vector<u_char>& ss = sk.serialize();
    size_t ss_len = ss.size();

    size_t total_written = 0;
    u_char buf[KF_BUFFER_SIZE];

    for (size_t offset = 0; offset < ss_len; offset += KF_BUFFER_SIZE) {
        size_t chunk_size = std::min(KF_BUFFER_SIZE, ss_len - offset);
        std::copy(ss.begin() + offset, ss.begin() + offset + chunk_size, buf);
        size_t written = fwrite(buf, sizeof(u_char), chunk_size, f);
        if (written != chunk_size) {
            perror("fwrite failed");
            return total_written;
        }
        total_written += written;
    }
    
    return total_written;
}

size_t ElGamel_PK2File(FILE* f, const ElGamelPK& pk) {
    const std::vector<u_char>& ss = pk.serialize();
    size_t ss_len = ss.size();

    size_t total_written = 0;
    u_char buf[KF_BUFFER_SIZE];

    for (size_t offset = 0; offset < ss_len; offset += KF_BUFFER_SIZE) {
        size_t chunk_size = std::min(KF_BUFFER_SIZE, ss.size() - offset);
        std::copy(ss.begin() + offset, ss.begin() + offset + chunk_size, buf);
        
        size_t written = fwrite(buf, sizeof(u_char), chunk_size, f);
        if (written != chunk_size) {
            perror("fwrite failed");
            return total_written;
        }
        total_written += written;
    }
    
    return total_written;
}

bool ElGamel_File2SK(FILE* f, ElGamelSK& sk) {
    std::vector<u_char> data;
    u_char buf[KF_BUFFER_SIZE];
    size_t read_bytes;

    while ((read_bytes = fread(buf, sizeof(u_char), KF_BUFFER_SIZE, f)) > 0) {
        data.insert(data.end(), buf, buf + read_bytes);
    }

    if (ferror(f)) {
        perror("fread failed");
        return false;
    }

    size_t rd_bytes = sk.deserialize(data);
    if(rd_bytes == 0) {
        fprintf(stderr, "Error: ElGamel_File2SK: load pri-key failed.");
        return false;
    }

    return true;
}

bool ElGamel_File2PK(FILE* f, ElGamelPK& pk) {
    std::vector<u_char> data;
    u_char buf[KF_BUFFER_SIZE];
    size_t read_bytes;

    while ((read_bytes = fread(buf, sizeof(u_char), KF_BUFFER_SIZE, f)) > 0) {
        data.insert(data.end(), buf, buf + read_bytes);
    }

    if (ferror(f)) {
        perror("fread failed");
        return false;
    }

    size_t rd_bytes = pk.deserialize(data);
    if(rd_bytes == 0) {
        fprintf(stderr, "Error: ElGamel_File2PK: load pub-key failed.");
        return false;
    }

    return true;
}

/* Encryptor and Decryptor */

ElGamelEncMsg ElGamelEncrypt(const ElGamelPK& pk, const m_type& msg) {
    EllipticCurve e(pk.eCtx);
    const m_type p = pk.eCtx.p;
    ElGamelEncMsg envMsg = std::make_pair(Point(mnum(0), mnum(0)), Point(mnum(0), mnum(0)));

    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    boost::random::mt19937 rng(seed);

    boost::random::uniform_int_distribution<m_type> dist(2, pk.maxu);
    m_type k = dist(rng);

    Point G(mnum(pk.Gx, p), mnum(pk.Gy, p));
    Point Y(mnum(pk.Yx, p), mnum(pk.Yy, p));
    Point C1 = e.mulPoint(G, k);
    envMsg.first = C1;

    m_type Mx, My;
    bool flag = e.embedMessage(msg, Mx, My, pk.k);
    if(!flag) {
        cerr << "ElGamelEncrypt:: cann't embed the message: " << msg << endl;
        return envMsg;
    } else if(Mx >= p) {
        cerr << "ElGamelEncrypt:: The msg is too large to embed. e.k="
            << pk.k << ", msg=" << msg << ", p=" << p << "." << endl;
        return envMsg;
    }
    Point M(mnum(Mx, p), mnum(My, p));
    Point C2 = e.mulPoint(Y, k); C2 = e.addPoints(C2, M);
    envMsg.second = C2;

    return envMsg;
}

m_type ElGamelDecrypt(const ElGamelSK& sk, const ElGamelEncMsg& msg) {
    EllipticCurve e(sk.eCtx);
    const m_type p = sk.eCtx.p;

    Point C1 = msg.first;
    Point C2 = msg.second;

    Point T(mnum(0), mnum(0));
    T = e.mulPoint(-C1, sk.x);
    T = e.addPoints(C2, T);

    return e.getMessage(T.x.getX(), sk.k);
}

/* ElGamel Context related functions */

ElGamelCtx ElGamelCtx_secp256k1() {
    const unsigned char* Gx_arr = new unsigned char[32]{0x79, 0xBE, 0x66, 0x7E, 
        0xF9, 0xDC, 0xBB, 0xAC, 
        0x55, 0xA0, 0x62, 0x95, 
        0xCE, 0x87, 0x0B, 0x07, 
        0x02, 0x9B, 0xFC, 0xDB, 
        0x2D, 0xCE, 0x28, 0xD9, 
        0x59, 0xF2, 0x81, 0x5B, 
        0x16, 0xF8, 0x17, 0x98
    };
    const unsigned char* Gy_arr = new unsigned char[32]{0x48, 0x3A, 0xDA, 0x77, 
        0x26, 0xA3, 0xC4, 0x65, 
        0x5D, 0xA4, 0xFB, 0xFC, 
        0x0E, 0x11, 0x08, 0xA8, 
        0xFD, 0x17, 0xB4, 0x48, 
        0xA6, 0x85, 0x54, 0x19, 
        0x9C, 0x47, 0xD0, 0x8F, 
        0xFB, 0x10, 0xD4, 0xB8
    };
    const unsigned char* level_arr = new unsigned char[32]{0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFE, 
        0xBA, 0xAE, 0xDC, 0xE6, 
        0xAF, 0x48, 0xA0, 0x3B, 
        0xBF, 0xD2, 0x5E, 0x8C, 
        0xD0, 0x36, 0x41, 0x41
    };

    ElGamelCtx ctx;
    ctx.eCtx.a = 0;
    ctx.eCtx.b = 7;
    const m_type t = 1;
    const m_type p = (t<<256) - (t<<32) - (t<<9) - (t<<8) - (t<<7) - (t<<6) - (t<<4) - 1;
    ctx.eCtx.p = p;

    ctx.Gx = readFromArray(Gx_arr, 32);
    ctx.Gy = readFromArray(Gy_arr, 32);
    ctx.level = readFromArray(level_arr, 32);
    ctx.k = 30;
    
    return ctx;
}

/* ElGamel pub-key and pri-key generator */

void readFromArray(const u_char* arr, size_t len, m_type& res) {
    res = 0;
    for(size_t i = 0; i<len; ++i) {
        res <<= 8;
        res += arr[i];
    }
}

m_type readFromArray(const u_char* arr, size_t len) {
    m_type res = 0;
    readFromArray(arr, len, res);
    return res;
}

ElGamelGenerator::ElGamelGenerator(): Y(mnum(0), mnum(0)), G(mnum(0), mnum(0)) {
    auto ctx = ElGamelCtx_secp256k1();

    this->e = new EllipticCurve(ctx.eCtx);
    k = ctx.k;
    const m_type p = ctx.eCtx.p;
    G = Point(mnum(ctx.Gx, p), mnum(ctx.Gy, p));
    level = ctx.level;
}

ElGamelGenerator::ElGamelGenerator(const ElGamelCtx& ctx): Y(mnum(0), mnum(0)), G(mnum(0), mnum(0)) {
    this->e = new EllipticCurve(ctx.eCtx);
    k = ctx.k;
    const m_type p = ctx.eCtx.p;
    G = Point(mnum(ctx.Gx, p), mnum(ctx.Gy, p));
    level = ctx.level;
}

ElGamelGenerator::~ElGamelGenerator() {
    if (this->e != NULL) {
        delete e;
        e = NULL;
    }
}

bool ElGamelGenerator::findGenBrutely() {
    bool flag = e->findGenBrutely(level, G);
    if(!flag) {
        cerr << "ElGamelGenerator:: genKey:: Cann't find generator with level" << level
            << "." << endl;
        return false;
    }
    return true;
}

void ElGamelGenerator::genKey(ElGamelSK& sk, ElGamelPK& pk) {
    if(!MillarRabin(e->getP())) {
        cerr << "ElGamelGenerator:: The modulus " << e->getP() << " is not a prime." << endl;
        return;
    }

    if(!e->checkPoint(G)) {
        if(level == 0) level = EL_GAMEL_DEFAULT_LEVEL;
        bool flag = this->findGenBrutely();
        if(!flag) return;
    }

    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    rng.seed(seed);

    m_type maxu = sqrt(level);
    boost::random::uniform_int_distribution<m_type> dist(2, maxu);
    x = dist(rng);
    Y = e->mulPoint(G, x);

    sk.eCtx = e->getCTX();
    sk.x = x;
    sk.k = k;

    pk.eCtx = e->getCTX();
    pk.Gx = G.x.getX();
    pk.Gy = G.y.getX();
    pk.Yx = Y.x.getX();
    pk.Yy = Y.y.getX();
    pk.k = k;
    pk.maxu = maxu;
}

bool ElGamelGenerator::hasGen() {
    return (this->x) > 0;
}