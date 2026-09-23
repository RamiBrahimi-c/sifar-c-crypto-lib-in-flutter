#ifndef REDPIKE_H
#define REDPIKE_H


#include "../../cipher_interface.h"
#include "../../common/constants.h"
#include <stdint.h>

#define REDPIKE_KEY_MAX_SIZE 8
#define CONST 0x9E3779B9 // golden ratio huh




typedef struct {
    uchar_t key[REDPIKE_KEY_MAX_SIZE] ; 
    uint32_t constant ;
    CipherType type;
} RedpikeKey;





int redpike_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
int redpike_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
int redpike_set_key(void* key_struct, const uchar_t* key_str , size_t key_len);
int redpike_free_key(void* key_struct);

Cipher* get_redpike_cipher(void);



int redpike_encrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int redpike_encrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int redpike_encrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int redpike_decrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int redpike_encrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int redpike_decrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int redpike_encrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int redpike_decrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;




#endif