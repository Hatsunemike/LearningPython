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
bool testAddPoints();
bool testMulPoints();

/* ElGamel Cryptology Test */

bool testElGamel();
bool testSerialization();
bool testK2F_F2K();
bool testFileEncDec();

/* OpenSSL EVP API Test */
int CalcFileDigest(const char* filedir, const char* algo_name, u_char* md5ans, u_int* md5len);
bool testEVPFileDigest();
#endif