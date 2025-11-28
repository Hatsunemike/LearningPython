#include "mnum.h"
#include <vector>

std::vector<u_char> serialize_int(const int& x);
size_t deserialize_int(int& n, const std::vector<u_char>& data);

std::vector<u_char> serialize_mpz(const m_type& n);
size_t deserialize_mpz(m_type& n, const std::vector<u_char>& data);