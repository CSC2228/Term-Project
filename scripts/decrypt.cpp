#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include<iostream>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include "cipher_params.h"
#include "file_encrypter_decrypter.h"

using namespace std;

#define AES_256_KEY_SIZE 32
#define AES_BLOCK_SIZE 16
#define BUFSIZE 1024

int main(int argc, char *argv[])
{
    FILE *f_input, *f_enc, *f_dec;
        /* Make sure user provides the input file */
    if (argc != 3) {
        printf("Usage: %s /path/to/encrypted/file /path/to/decrypted/file\n", argv[0]);
        return 1;
    }

    cipher_params_t params;

    /* Key to use for encryption and decryption */
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

    /* Initialization Vector */
    unsigned char *iv = (unsigned char *)"0123456789012345";


    /* Generate cryptographically strong pseudo-random bytes for key and IV */
    //if (!RAND_bytes(key, sizeof(key)) || !RAND_bytes(iv, sizeof(iv))) {
        /* OpenSSL reports a failure, act accordingly */
        //fprintf(stderr, "ERROR: RAND_bytes error: %s\n", strerror(errno));
        //return errno;
    //}

    params.key = key;
    params.iv = iv;

    params.encrypt = 0;
    params.cipher_type = EVP_aes_256_cbc();


    /* Open the encrypted file for reading in binary ("rb" mode) */
    f_input = fopen(argv[1], "rb");
    if (!f_input) {
        /* Unable to open file for reading */
        fprintf(stderr, "ERROR: fopen error: %s\n", strerror(errno));
        return errno;
    }

    /* Open and truncate file to zero length or create decrypted file for writing */
    f_dec = fopen(argv[2], "wb");
    if (!f_dec) {
        /* Unable to open file for writing */
        fprintf(stderr, "ERROR: fopen error: %s\n", strerror(errno));
        return errno;
    }

    /* Decrypt the given file */
    file_encrypt_decrypt(params, f_input, f_dec);

    /* Close the open file descriptors */
    fclose(f_input);
    fclose(f_dec);

    /* Free the memory allocated to our structure */
    //free(params);

    return 0;

 }
