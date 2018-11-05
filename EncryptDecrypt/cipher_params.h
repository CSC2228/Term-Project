#include <openssl/evp.h>

#ifndef CIPHER_PARAMS_H_INCLUDED
#define CIPHER_PARAMS_H_INCLUDED

typedef struct cipher_params_t {
    unsigned char *key;
    unsigned char* iv;
    unsigned int encrypt;
    const EVP_CIPHER *cipher_type;
}cipher_params_t;

#endif // CIPHER_PARAMS_H_INCLUDED
