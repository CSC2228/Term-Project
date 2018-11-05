#include "cipher_params.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef ENCRYPT_DECRYPT_H_INCLUDED
#define ENCRYPT_DECRYPT_H_INCLUDED

void file_encrypt_decrypt(cipher_params_t params, FILE *ifp, FILE *ofp);

#endif // ENCRYPT_DECRYPT_H_INCLUDED
