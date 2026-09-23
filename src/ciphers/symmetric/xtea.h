#ifndef XTEA_H
#define XTEA_H


#include "../../cipher_interface.h"
#include "../../common/constants.h"
#include <stdint.h>

#define XTEA_KEY_MAX_SIZE 16
#define XTEA_ROUNDS 32

typedef struct {
    uchar_t key[XTEA_KEY_MAX_SIZE] ; 
    int length ;
    CipherType type;
} XTeaKey;

/*
    eXtended Tea :(
*/

int xtea_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
int xtea_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
int xtea_set_key(void* key_struct, const uchar_t* key_str , size_t key_len);
int xtea_free_key(void* key_struct);

Cipher* get_xtea_cipher(void);



int xtea_encrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int xtea_encrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int xtea_encrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int xtea_decrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int xtea_encrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int xtea_decrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int xtea_encrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int xtea_decrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;





#endif