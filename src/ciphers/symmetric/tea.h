#ifndef TEA_H
#define TEA_H


#include "../../cipher_interface.h"
#include "../../common/constants.h"
#include <stdint.h>

#define TEA_KEY_MAX_SIZE 16

typedef struct {
    uchar_t key[TEA_KEY_MAX_SIZE] ; 
    int length ;
    CipherType type;
} TeaKey;

/*
    i mean i just like everything about this algorithm from the name to the vibe ..
    nothing beats cup of TEA :)
*/

int tea_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
int tea_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
int tea_set_key(void* key_struct, const uchar_t* key_str , size_t key_len);
int tea_free_key(void* key_struct);

Cipher* get_tea_cipher(void);



int tea_encrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int tea_encrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int tea_encrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int tea_decrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int tea_encrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int tea_decrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int tea_encrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int tea_decrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;





#endif