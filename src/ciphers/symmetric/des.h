#ifndef DES_H
#define DES_H


#include "../../cipher_interface.h"
#include "../../common/constants.h"
#include <stdint.h>

typedef struct {
    // 8 or 7 ????????
    uchar_t key[8] ; 
    CipherType type;
} DesKey;



int des_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
int des_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
int des_set_key(void* key_struct, const uchar_t* key_str , size_t key_len);
int des_free_key(void* key_struct);

Cipher* get_des_cipher(void);



int des_encrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int des_encrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int des_encrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int des_decrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int des_encrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int des_decrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int des_encrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int des_decrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;


#endif