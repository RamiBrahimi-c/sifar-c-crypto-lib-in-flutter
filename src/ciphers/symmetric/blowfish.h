#ifndef BLOWFISH_C
#define BLOWFISH_C


#include <stdint.h>

#include "../../cipher_interface.h"
#include "../../common/constants.h"

typedef struct {
    uchar_t *key ; 
    uint32_t length ;
    uint32_t p[18] ; 
    uint32_t s1[256] ; 
    uint32_t s2[256] ; 
    uint32_t s3[256] ; 
    uint32_t s4[256] ; 
    CipherType type;


} BlowfishKey;

/*
    before implementing :
        it sounds cool .. lets try implementing it !!!!!!!
    after implementing :
        yea ... wtv 🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡
*/

int blowfish_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
int blowfish_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
int blowfish_set_key(void* key_struct, const uchar_t* key_str , size_t key_len);
int blowfish_free_key(void* key_struct);



int blowfish_encrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int blowfish_encrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int blowfish_encrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int blowfish_decrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int blowfish_encrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int blowfish_decrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;

int blowfish_encrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;
int blowfish_decrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key) ;



Cipher* get_blowfish_cipher(void);





#endif