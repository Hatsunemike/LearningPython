# `EllipCurve` Demo

-------

A demo implementing some ECC cryptology.

## Current Features

1. **`ElGamel` asymmetric cryptography in ECC.**

    [Algorithm Details](./docs/ElGamel.md).

2. **Inner Curves**
    1. `secp256k1`: $E_p(a,b)$, $G$, $ord(G)$

## Developing Environment
- Ubuntu Desktop 24.04
- gcc 13.3.0
- Cmake 3.28.3
    - GNU make 4.3
- Boost 1.83.0
    - multiprecision
    - unordered
    - random
- OpenSSL 3.0.13
- argparse (for command line utility `elgamel` )