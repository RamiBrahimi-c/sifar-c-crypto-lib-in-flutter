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



void des_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
void des_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
void des_set_key(void* key_struct, const uchar_t* key_str , size_t key_len);
void des_free_key(void* key_struct);

Cipher* get_des_cipher(void);







#endif