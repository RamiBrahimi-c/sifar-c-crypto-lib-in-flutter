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

void xtea_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
void xtea_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
void xtea_set_key(void* key_struct, const uchar_t* key_str , size_t key_len);
void xtea_free_key(void* key_struct);

Cipher* get_xtea_cipher(void);







#endif
