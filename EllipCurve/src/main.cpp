#include <stdio.h>
#include "EllipticCurve.h"
#include "bigprime.h"
#include "test.h"
#include <string>

#include <openssl/evp.h>
#include <openssl/err.h>

using std::string;

int CalcFileDigest(const char* filedir, const char* algo_name, u_char* md5ans, u_int* md5len) {
    const size_t BUFFER_SIZE = 512;
    FILE* file = NULL;
    OSSL_LIB_CTX* lib_ctx = NULL;
    EVP_MD* algo = NULL;
    EVP_MD_CTX* digest_ctx = NULL;
    size_t bytes_read;
    u_char* buffer = new u_char[BUFFER_SIZE];
    int result = 0;

    file = fopen(filedir, "rb");
    if (!file) {
        fprintf(stderr, "the file cann't open: %s\n", filedir);
        result = 1;
        goto cleanup;
    }

    lib_ctx = OSSL_LIB_CTX_new();
    if (lib_ctx == NULL) {
        fprintf(stderr, "Create EVP digest context failed.");
        result = 1;
        goto cleanup;
    }

    algo = EVP_MD_fetch(lib_ctx, algo_name, NULL);
    if(algo == NULL) {
        fprintf(stderr, "EVP_MD_fetch cann't find %s algorithm.\n", algo_name);
        result = 1;
        goto cleanup;
    }

    digest_ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(digest_ctx, algo, NULL);
    while((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        EVP_DigestUpdate(digest_ctx, buffer, bytes_read);
    }

    EVP_DigestFinal_ex(digest_ctx, md5ans, md5len);

cleanup:
    delete[] buffer;
    EVP_MD_free(algo);
    EVP_MD_CTX_free(digest_ctx);
    OSSL_LIB_CTX_free(lib_ctx);
    if(file) fclose(file);
    return result;
}

int main(int argc, char* argv[]) {
    string filedir = "./input.txt";
    string outfiledir = "./output.txt";
    int result = 0;
    u_int len = 0;
    if (argc > 1) {
        filedir = argv[1];
    }
    if (argc > 2) {
        outfiledir = argv[2];
    }

    u_char* md5ans = new u_char[EVP_MAX_MD_SIZE];
    FILE* output = fopen(outfiledir.c_str(), "w");

    if (CalcFileDigest(filedir.c_str(), "sha256", md5ans, &len) == 0) {
        for(u_int i=0;i<len;++i) {
            fprintf(output, "%02x", md5ans[i]);
        }
    } else {
        result = 1;
        goto cleanup;
    }

cleanup:
    fclose(output);
    delete[] md5ans;
    if(result > 0) {
        fprintf(stderr, "output failed.\n");
    }
    return 0;
}