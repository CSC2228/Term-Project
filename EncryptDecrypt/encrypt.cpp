#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include<iostream>

//OpenSSL includes
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include "cipher_params.h"
#include "file_encrypter_decrypter.h"

using namespace std;

//defining key size and block size
#define AES_256_KEY_SIZE 32
#define AES_BLOCK_SIZE 16
#define BUFSIZE 1024

int main(int argc, char *argv[])
{
    FILE *f_input, *f_enc, *f_dec;
    /* Make sure user provides the input file */
    if (argc != 3) {
        printf("Usage: %s /path/to/input/file /path/to/encrypted/file\n", argv[0]);
        return 1;
    }

    //structure to pass in key, iv, encryption flag and cipher type
    cipher_params_t params;

    /*TO-DO: These values should not be hard-coded
    /* Key to use for encryption and decryption */
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

    /* Initialization Vector */
    unsigned char *iv = (unsigned char *)"0123456789012345";

    /* Generate cryptographically strong pseudo-random bytes for key and IV */
    /* TO-DO: the generated iv and key should be randomly generated and securely stored to enable decryption
    //if (!RAND_bytes(key, sizeof(key)) || !RAND_bytes(iv, sizeof(iv))) {
        /* OpenSSL reports a failure, act accordingly */
        //fprintf(stderr, "ERROR: RAND_bytes error: %s\n", strerror(errno));
        //return errno;
    //}

    //Initialize key and iv
    params.key = key;
    params.iv = iv;

    //1 indicates that file needs to be encrypted, 0 indicates decryption
    params.encrypt = 1;

    //specify the cipher type
    params.cipher_type = EVP_aes_256_cbc();

   //open the file provided by user for encryption
   f_input = fopen(argv[1], "rb");
    if (!f_input) {
        /* Unable to open file for reading */
        fprintf(stderr, "ERROR: fopen error: %s\n", strerror(errno));
        return errno;
    }

    /* Open and truncate file to zero length or create cipher text file for writing */
    f_enc = fopen(argv[2], "wb");
    if (!f_enc) {
        /* Unable to open file for writing */
        fprintf(stderr, "ERROR: fopen error: %s\n", strerror(errno));
        return errno;
    }

    /* Encrypt the given file */
    file_encrypt_decrypt(params, f_input, f_enc);

    /* Encryption done, close the file descriptors */
    fclose(f_input);
    fclose(f_enc);
    return 0;
 }

