#ifndef CIPHER_INTERFACE_H
#define CIPHER_INTERFACE_H

#include "./common/constants.h"
#include "common/custom_string.h"
#include <stddef.h>

typedef struct {
    void (*encrypt)(const uchar_t* input, uchar_t* output, const void* key);
    void (*decrypt)(const uchar_t* input, uchar_t* output, const void* key);
    void (*set_key)(void* key_struct, const uchar_t* key_str , size_t key_len);
    void (*free_key)(void* key_struct);
    char* name;
} Cipher;

typedef enum CipherType {
    STREAM_CIPHER,
    BLOCK_CIPHER
}CipherType;

typedef enum BlockCipher_MODE_OP { // gotta find a better name
    ECB,
    CBC,
    OFB,
    CFB,
    CTR,
    GCM
}BlockCipher_MODE_OP;


typedef union Cipher_Info{
    CipherType cipher_type;
    BlockCipher_MODE_OP block_cipher_mode_op;
}Cipher_Info;


void register_cipher(Cipher* cipher);
Cipher* get_cipher(const char* name);

// the name sucks ik :(
// i let it as global variable to be accessible from anywhere + automatically initilized to 0 (ECB mode) and stored in .BSS :D
extern BlockCipher_MODE_OP block_cipher_mode_operation ;

#endif