#ifndef TDES_H
#define TDES_H

/*
    NOTE : i literally followed a book , idk if this is HOW REAL 3des is done 
        i followed "Understanding Cryptography -- Christof Paar & Jan Pelzl" page 78

*/
#include "../../cipher_interface.h"
#include "../../common/constants.h"
#include <stdint.h>

typedef struct {
    // i dont have any idea about what the hell im doing here
    uchar_t key[24] ; //???????????????????? 
    CipherType type;
} TDesKey;



int tdes_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
int tdes_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
int tdes_set_key(void* key_struct, const uchar_t* key_str , size_t key_len);
int tdes_free_key(void* key_struct);

Cipher* get_tdes_cipher(void);



int tdes_encrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int tdes_encrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int tdes_encrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int tdes_decrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int tdes_encrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int tdes_decrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int tdes_encrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int tdes_decrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;


#endif