#ifndef TEST_H
#define TEST_H

#include <ctime>
#include <cstdio>
#include <cstring>
#include <list>
#include "bigprime.h"
#include "ElGamel.h"

void Oula(char* isPrime, const int maxn);
bool testMillarRabin();
bool testMnumPow();
bool testInv();
bool testCipolla();
bool testEmbedMessage();
bool testFindGen();
bool testMulPoints();

/* ElGamel Cryptology Test */

bool testElGamel();

/* OpenSSL EVP API Test */
int CalcFileDigest(const char* filedir, const char* algo_name, u_char* md5ans, u_int* md5len);
bool testEVPFileDigest();
#endif