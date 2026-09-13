#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H



#include "../../cipher_interface.h"
#include <stdint.h>
#include "../../common/constants.h"


typedef struct {
    uchar_t map[ARRAY_MAX_SIZE];
    int alphabet_size;
} SubstitutionKey;




void substittution_encrypt(const uchar_t* input, uchar_t* output, const void* key);
void substittution_decrypt(const uchar_t* input, uchar_t* output, const void* key);
void substittution_set_key(void* key_struct, const uchar_t* key_str , size_t key_len);
void substittution_free_key(void* key_struct);

Cipher* get_substittution_cipher(void);




#endif
