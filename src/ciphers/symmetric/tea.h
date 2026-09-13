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

void tea_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
void tea_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
void tea_set_key(void* key_struct, const uchar_t* key_str , size_t key_len);
void tea_free_key(void* key_struct);

Cipher* get_tea_cipher(void);







#endif
