#ifndef RC4_H
#define RC4_H


#include "../../cipher_interface.h"
#include "../../common/constants.h"
#include <stdint.h>

#define RC4_KEY_MAX_SIZE 256

typedef struct {
    uchar_t state[RC4_KEY_MAX_SIZE] ; 
    int i ;
    int j ;
    int length ;
    CipherType type;
} Rc4Key;



int rc4_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
int rc4_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
int rc4_set_key(void* key_struct, const uchar_t* key_str , size_t key_len);
int rc4_free_key(void* key_struct);

Cipher* get_rc4_cipher(void);







#endif