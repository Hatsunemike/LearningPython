#include "streamio.h"

namespace mystream {
    int read_int(FILE* f, int& n) {
        u_char c[4];
        size_t rd_bytes = fread(c, sizeof(u_char), 4, f);
        if(rd_bytes < 4)return 0;
        n = (c[0]<<24)+(c[1]<<16)+(c[2]<<8)+c[3];
        return 1;
    }

    int write_int(FILE* f, const int &n) {
        u_char c[4];
        c[0] = (n>>24)&0xff;
        c[1] = (n>>16)&0xff;
        c[2] = (n>>8)&0xff;
        c[3] = n&0xff;
        size_t wt_bytes = fwrite(c, sizeof(u_char), 4, f);
        if(wt_bytes < 4) {
            if(wt_bytes == 0)return 0;
            else return -1;
        }
        return 1;
    }

    int read_mpz(FILE* f, m_type& n) {
        u_char len = 0;
        u_char sign;
        u_char rd_bytes = fread(&len, sizeof(u_char), 1, f);
        if(!rd_bytes) return 0;
        rd_bytes = fread(&sign, sizeof(u_char), 1, f);
        if(!rd_bytes || (sign != 0 && sign != 0xff)) return 0;
        u_char now;
        n = 0;
        for(u_char i=0; i<len; ++i) {
            rd_bytes = fread(&now, sizeof(u_char), 1, f);
            if(rd_bytes < 1) return -1;
            n = (n<<8)|now;
        }
        if(sign == 0xff) n = -n;
        return 1;
    }

    int write_mpz(FILE* f, const m_type& n) {
        size_t size;
        u_char* chs = (u_char*)mpz_export(nullptr, &size, 1, 1, 1, 0, n.backend().data());
        u_char sign = 0;
        if(n<0) sign = 0xff;
        u_char len = (u_char)size;
        size_t wt_bytes;
        wt_bytes = fwrite(&len, sizeof(u_char), 1, f);
        if(wt_bytes == 0) {
            delete[] chs;
            return 0;
        }
        wt_bytes = fwrite(&sign, sizeof(u_char), 1, f);
        if(wt_bytes == 0) {
            delete[] chs;
            return -1;
        }
        wt_bytes = fwrite(chs, sizeof(u_char), len, f);
        if(wt_bytes < len) {
            delete[] chs;
            return -1;
        }
        delete[] chs;
        return 1;
    }
}
