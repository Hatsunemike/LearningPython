/**
 * Stream Reader and Outputer for some class.
 * return value all use `int`:
 *  positive number: means the varient it read.(usually 1.)
 *  0: means the varien isn't assigned successfully.
 *  EOF: means reading error or attaching the file's end.
 */

#include "mnum.h"
#include <iostream>

namespace mystream {
    /**
     * int
     */
    int read_int(FILE* f, int &n);
    int write_int(FILE* f, const int &n);

    /**
     * mpz_int
     */
    int read_mpz(FILE* f, m_type& n);
    int write_mpz(FILE* f, const m_type& n);
}