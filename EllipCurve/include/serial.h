#include "mnum.h"
#include <vector>

/**
 * serialize_int
 * 
 * The generated data list:
 * `S(x) = <b1><b2><b3><b4>`, where `(b1<<24)|(b2<<16)|(b3<<8)|b4 = x.`
 */
std::vector<u_char> serialize_int(const int& x);
size_t deserialize_int(int& n, const std::vector<u_char>& data);

/**
 * serialize_mpz
 * 
 * The generated data list:
 * 
 * `S(n) = <len_n><sign_n><n_1>[<n_k>]`
 * 
 * `len_n` is the length of $n_k$.
 * `sign_n` is `0xff` when n is negative, and '0x00` when n is positive.
 * `n_k` meet the condition: $\sum \limits_{k=0}^{len_n - 1} n_k * 2^k = n$.
 */
std::vector<u_char> serialize_mpz(const m_type& n);
size_t deserialize_mpz(m_type& n, const std::vector<u_char>& data);