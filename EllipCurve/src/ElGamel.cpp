#include "ElGamel.h"
#include "bigprime.h"
#include "serial.h"
#include "streamio.h"
#include <chrono>

using std::cerr;
using std::endl;
namespace ms = mystream;

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

int ElGamelSK::writeTo(FILE* f) const {
    int ret;
    ret = eCtx.writeTo(f);
    if(ret < 1) return ret;
    ret = ms::write_mpz(f, x);
    if(ret < 1) return ret;
    ret = ms::write_int(f, k);
    if(ret < 1) return ret;
    return 1;
}

int ElGamelSK::readFrom(FILE* f) {
    int ret;
    ret = eCtx.readFrom(f);
    if(ret < 1) return ret;
    ret = ms::read_mpz(f, x);
    if(ret < 1) return ret;
    ret = ms::read_int(f, k);
    if(ret < 1) return ret;
    return 1;
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

int ElGamelPK::writeTo(FILE* f) const {
    int ret;
    ret = eCtx.writeTo(f);
    if(ret < 1) return ret;
    ret = ms::write_mpz(f, Gx);
    if(ret < 1) return ret;
    ret = ms::write_mpz(f, Gy);
    if(ret < 1) return ret;
    ret = ms::write_mpz(f, Yx);
    if(ret < 1) return ret;
    ret = ms::write_mpz(f, Yy);
    if(ret < 1) return ret;
    ret = ms::write_mpz(f, maxu);
    if(ret < 1) return ret;
    ret = ms::write_int(f, k);
    if(ret < 1) return ret;
    return 1;
}

int ElGamelPK::readFrom(FILE* f) {
    int ret;
    ret = eCtx.readFrom(f);
    if(ret < 1) return ret;
    ret = ms::read_mpz(f, Gx);
    if(ret < 1) return ret;
    ret = ms::read_mpz(f, Gy);
    if(ret < 1) return ret;
    ret = ms::read_mpz(f, Yx);
    if(ret < 1) return ret;
    ret = ms::read_mpz(f, Yy);
    if(ret < 1) return ret;
    ret = ms::read_mpz(f, maxu);
    if(ret < 1) return ret;
    ret = ms::read_int(f, k);
    if(ret < 1) return ret;
    return 1;
}

/**
 * file/memory transmitting functions.
 */

const size_t KF_BUFFER_SIZE = 2048;

#include <vector>
#include <cstdio>
#include <algorithm> // std::min

// vector<u_char> - file transformation

bool read_u_char_vec(FILE* f, std::vector<u_char>& v) {
    u_char buf[KF_BUFFER_SIZE];

    v.clear();
    size_t rd_bytes;
    while((rd_bytes = fread(buf, sizeof(u_char), KF_BUFFER_SIZE, f)) > 0) {
        v.insert(v.end(), buf, buf+rd_bytes);
    }

    if (ferror(f)) {
        perror("read_u_char_vec:: read file failed.");
        return false;
    }

    return true;
}

bool write_u_char_vec(FILE* f, const std::vector<u_char>& v) {
    size_t v_len = v.size();

    u_char buf[KF_BUFFER_SIZE];

    for (size_t offset = 0; offset < v_len; offset += KF_BUFFER_SIZE) {
        size_t chunk_size = std::min(KF_BUFFER_SIZE, v.size() - offset);
        std::copy(v.begin() + offset, v.begin() + offset + chunk_size, buf);
        
        size_t written = fwrite(buf, sizeof(u_char), chunk_size, f);
        if (written != chunk_size) {
            perror("fwrite failed");
            return false;
        }
    }
    
    return true;
}

size_t ElGamel_SK2File(FILE* f, const ElGamelSK& sk) {
    const std::vector<u_char>& ss = sk.serialize();
    return write_u_char_vec(f, ss);
}

size_t ElGamel_PK2File(FILE* f, const ElGamelPK& pk) {
    const std::vector<u_char>& ss = pk.serialize();
    return write_u_char_vec(f, ss);
}

bool ElGamel_File2SK(FILE* f, ElGamelSK& sk) {
    std::vector<u_char> data;
    bool flag = read_u_char_vec(f, data);
    if(!flag) {
        fprintf(stderr, "Error: ElGamel_File2SK: read file failed.");
        return false;
    }

    size_t rd_bytes = sk.deserialize(data);
    if(rd_bytes == 0) {
        fprintf(stderr, "Error: ElGamel_File2SK: deserialize pri-key failed.");
        return false;
    }

    return true;
}

bool ElGamel_File2PK(FILE* f, ElGamelPK& pk) {
    std::vector<u_char> data;
    bool flag = read_u_char_vec(f, data);
    if(!flag) {
        fprintf(stderr, "Error: ElGamel_File2PK: read file failed.");
    }

    size_t rd_bytes = pk.deserialize(data);
    if(rd_bytes == 0) {
        fprintf(stderr, "Error: ElGamel_File2PK: deserialize pub-key failed.");
        return false;
    }

    return true;
}

/**
 * class ElGamelEncMsg
 */

ElGamelEncMsg::ElGamelEncMsg(const std::pair<Point, Point>& a) : std::pair<Point, Point>(a) {}

ElGamelEncMsg& ElGamelEncMsg::operator=(const std::pair<Point, Point>& a) {
    this->first = a.first;
    this->second = a.second;
    return *this;
}

std::vector<u_char> ElGamelEncMsg::serialize() {
    std::vector<u_char> ret;
    std::vector<u_char> t;

    // Serialize first point (C1)
    t = serialize_mpz(this->first.x.getX());
    ret.insert(ret.end(), t.begin(), t.end());
    t = serialize_mpz(this->first.y.getX());
    ret.insert(ret.end(), t.begin(), t.end());

    // Serialize second point (C2)
    t = serialize_mpz(this->second.x.getX());
    ret.insert(ret.end(), t.begin(), t.end());
    t = serialize_mpz(this->second.y.getX());
    ret.insert(ret.end(), t.begin(), t.end());

    return ret;
}

size_t ElGamelEncMsg::deserialize(const std::vector<u_char>& data, const m_type& p) {
    size_t now = 0;
    size_t prt_bytes;
    std::vector<u_char> tmp;
    m_type x;

    // Deserialize first point (C1)
    tmp.assign(data.begin() + now, data.end());
    prt_bytes = deserialize_mpz(x, tmp);
    if (prt_bytes == 0) return 0;
    this->first.x = mnum(x, p);
    now += prt_bytes;

    tmp.assign(data.begin() + now, data.end());
    prt_bytes = deserialize_mpz(x, tmp);
    if (prt_bytes == 0) return 0;;
    this->first.y = mnum(x, p);
    now += prt_bytes;

    // Deserialize second point (C2)
    tmp.assign(data.begin() + now, data.end());
    prt_bytes = deserialize_mpz(x, tmp);
    if (prt_bytes == 0) return 0;;
    this->second.x = mnum(x, p);
    now += prt_bytes;

    tmp.assign(data.begin() + now, data.end());
    prt_bytes = deserialize_mpz(x, tmp);
    if (prt_bytes == 0) return 0;
    this->second.y = mnum(x, p);
    now += prt_bytes;

    return true;
}

int ElGamelEncMsg::writeTo(FILE* f) const {
    int ret;
    ret = ms::write_mpz(f, this->first.x.getX());
    if(ret <= 0) return ret;
    ms::write_mpz(f, this->first.y.getX());
    if(ret <= 0) return ret;

    ms::write_mpz(f, this->second.x.getX());
    if(ret <= 0) return ret;
    ms::write_mpz(f, this->second.y.getX());
    if(ret <= 0) return ret;
    return 1;
}

int ElGamelEncMsg::readFrom(FILE* f, const m_type& p) {
    int ret;
    m_type xx, xy, yx, yy;
    ret = ms::read_mpz(f, xx);
    if(ret <= 0)return ret;
    ret = ms::read_mpz(f, xy);
    if(ret <= 0)return ret;
    ret = ms::read_mpz(f, yx);
    if(ret <= 0)return ret;
    ret = ms::read_mpz(f, yy);
    if(ret <= 0)return ret;

    this->first = Point(mnum(xx, p), mnum(xy, p));
    this->second = Point(mnum(yx, p), mnum(yy, p));
    
    return 1;
}

/* Encryptor and Decryptor */

ElGamelEncryptor::ElGamelEncryptor(const ElGamelPK& pk)
:p(pk.eCtx.p) {
    e = new EllipticCurve(pk.eCtx);
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    rng = boost::random::mt19937(seed);
    dist = boost::random::uniform_int_distribution<m_type>(2, pk.maxu);
    embed_k = pk.k;
    G = new Point(mnum(pk.Gx, p), mnum(pk.Gy, p));
    Y = new Point(mnum(pk.Yx, p), mnum(pk.Yy, p));
}

ElGamelEncryptor::~ElGamelEncryptor() {
    delete e;
    delete G;
    delete Y;
}

ElGamelEncMsg* ElGamelEncryptor::Encrypt(const m_type& msg) {
    bool flag = e->embedMessage(msg, Mx, My, embed_k);
    if(!flag) {
        cerr << "ElGamelEncrypt:: cann't embed the message: " << msg << endl;
        return NULL;
    } else if (Mx >= p) {
        cerr << "ElGamelEncrypt:: The msg is too large to embed. e.k="
            << embed_k << ", msg=" << msg << ", p=" << p << "." << endl;
        return NULL;
    }
    Point M(mnum(Mx, p), mnum(My, p));
    m_type k = dist(rng);
    
    Point C1 = e->mulPoint(*G, k);

    Point C2 = e->mulPoint(*Y, k);
    C2 = e->addPoints(C2, M);

    ElGamelEncMsg* encMsg = new ElGamelEncMsg(std::make_pair(C1, C2));
    return encMsg;
}

bool ElGamelEncryptor::EncryptFile(FILE* pf, FILE* cf) {
    int p_mp_size = p.backend().data()->_mp_size;
    u_char max_len_m;
    if(p_mp_size>0) max_len_m = p_mp_size*GMP_LIMB_BITS/8;
    else max_len_m = (-p_mp_size)*GMP_LIMB_BITS/8;

    u_char* buf = new u_char[max_len_m];
    u_char rest = 0;
    u_char rd_bytes;
    while((rd_bytes = fread(buf+rest, sizeof(u_char), max_len_m-rest, pf)) > 0 || rest>0) {
        m_type m = 0;
        u_char len = rest+rd_bytes;
        for(u_char i=0; i<len; ++i) m = (m<<8)|buf[i];
        for(; len>0; --len) {
            bool flag =e->embedMessage(m, Mx, My, embed_k);
            if(flag && Mx<p) break;
            m >>= 8;
        }
        if(!len) {
            cerr << "ElGamelEncryptor::EncryptFile: cann't embed message." << endl;
            delete[] buf;
            return false;
        }

        // write cipher chip
        fwrite(&len, sizeof(u_char), 1, cf);
        Point M(mnum(Mx, p), mnum(My, p));
        m_type k = dist(rng);
        Point C1 = e->mulPoint(*G, k);
        Point C2 = e->mulPoint(*Y, k);
        C2 = e->addPoints(C2, M);
        ElGamelEncMsg encMsg = std::make_pair(C1, C2);
        encMsg.writeTo(cf);

        // move the rest to the front
        for(u_char i=0; i<rest+rd_bytes-len; ++i) buf[i] = buf[i+len];
        rest = rest+rd_bytes-len;
    }
    return true;
}

ElGamelDecryptor::ElGamelDecryptor(const ElGamelSK& sk)
: p(sk.eCtx.p),
  x(sk.x),
  embed_k(sk.k) {
    e = new EllipticCurve(sk.eCtx);
    C1 = NULL;
    C2 = NULL;
}

ElGamelDecryptor::~ElGamelDecryptor() {
    delete e;
    delete C1;
    delete C2;
}

m_type ElGamelDecryptor::Decrypt(const ElGamelEncMsg& msg) {
    if(C1==NULL) C1 = new Point(msg.first);
    else *C1 = msg.first;

    if(C2==NULL) C2 = new Point(msg.second);
    else *C2 = msg.second;
    
    Point T(e->mulPoint(-(*C1), x));
    T = e->addPoints(*C2, T);

    return e->getMessage(T.x.getX(), embed_k);
}

bool ElGamelDecryptor::DecryptFile(FILE* cf, FILE* pf) {
    ElGamelEncMsg encMsg = std::make_pair(
        Point(mnum(0), mnum(0)),
        Point(mnum(0), mnum(0))
    );
    u_char rd_bytes;
    u_char len_m;
    while((rd_bytes = fread(&len_m, sizeof(u_char), 1, cf))>0) {
        int flag = encMsg.readFrom(cf, p);
        if(flag<1) return false;
        m_type m = this->Decrypt(encMsg);
        for(size_t i=0; i<len_m; ++i) {
            u_char now = int((m>>(8*(len_m-i-1)))&0xff);
            fwrite(&now, sizeof(u_char), 1, pf);
        }
    }
    return true;
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

bool ElGamelGenerator::genKey(ElGamelSK& sk, ElGamelPK& pk) {
    if(!MillarRabin(e->getP())) {
        cerr << "ElGamelGenerator:: The modulus " << e->getP() << " is not a prime." << endl;
        return false;
    }

    if(!e->checkPoint(G)) {
        if(level == 0) level = EL_GAMEL_DEFAULT_LEVEL;
        bool flag = this->findGenBrutely();
        if(!flag) return false;
    }

    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    rng.seed(seed);

    m_type maxu = level - 1;
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

    return true;
}

bool ElGamelGenerator::hasGen() {
    return (this->x) > 0;
}