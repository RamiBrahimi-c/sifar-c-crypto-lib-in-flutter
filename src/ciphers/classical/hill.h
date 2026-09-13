#ifndef HILL_H
#define HILL_H



#include "../../cipher_interface.h"
#include <stdint.h>
#include "../../common/constants.h"


typedef struct {
    uchar_t matrix[MATRIX_MAX_DEGREE][MATRIX_MAX_DEGREE];
    int n;
} HillKey;




void hill_encrypt(const uchar_t* input, uchar_t* output , int length, const void* key);
void hill_decrypt(const uchar_t* input, uchar_t* output, const void* key);
void hill_set_key(void* key_struct, const uchar_t* key_str , size_t key_len);
void hill_free_key(void* key_struct);

Cipher* get_hill_cipher(void);




#endif
