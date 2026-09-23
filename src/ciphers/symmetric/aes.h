#ifndef AES_H
#define AES_H

#include "../../cipher_interface.h"
#include "../../common/constants.h"
#include <stdint.h>



#include <stdio.h>
#include "../../common/keyexpan.h"
#include "../../common/galois_field_op.h"
#include "../../common/rijnbox.h"
//  extern uchar_t sbox[64][64] ; 


typedef enum AES_TYPE {
    AES128 , 
    AES192 , 
    AES256 
}AES_TYPE;



typedef struct AesKey {
    AES_TYPE mode ; // aes126 , aes192 or aes256
    uchar_t key[32] ; // max is 32 ...
    size_t key_length ;
    uchar_t *expanded_key ;
    size_t expanded_key_length ;
    int Nr ; // rounds number 
    int Nk ; // number of 32 bit words compromising the key
    CipherType type;
}AesKey;


int aes_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int aes_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int aes_set_key(void* key_struct, const uchar_t* key_str , size_t key_len);
int aes_free_key(void* key_struct);

int aes_encrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int aes_decrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int aes_encrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int aes_decrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int aes_encrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int aes_decrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int aes_encrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int aes_decrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

size_t aes_get_output_len(size_t input_len) ;

Cipher* get_aes_cipher(void);

#endif