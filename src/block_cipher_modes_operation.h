#ifndef BLOCK_CIPHER_MODES_OPERATION_H
#define BLOCK_CIPHER_MODES_OPERATION_H

#include "cipher_interface.h"
#include "common/utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// why would something that causes problems with threading be introduced here :
// void blockcipher_encrypt_modeop(uchar_t *input , uchar_t *output ,uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*encrypt_block)(const uchar_t* , uchar_t*, const void* )) ;
// void blockcipher_decrypt_modeop(uchar_t *input , uchar_t *output ,uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*decrypt_block)(const uchar_t* , uchar_t*, const void* )) ;

// returns 0 on success otherwise non-zero for error and it woll be reported to stderr
int ecb_encrypt(uchar_t *input , uchar_t *output , size_t length , size_t block_size  ,const void* key ,int (encrypt_block)(const uchar_t* , uchar_t* , const void* ) ) ;
// returns 0 on success otherwise non-zero for error and it woll be reported to stderr
int ecb_decrypt(uchar_t *input , uchar_t *output , size_t length , size_t block_size  ,const void* key ,int (decrypt_block)(const uchar_t* , uchar_t* , const void* ) ) ;

// returns 0 on success otherwise non-zero for error and it woll be reported to stderr
int cbc_encrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,int (encrypt_block)(const uchar_t* , uchar_t*, const void* ) ) ;
// returns 0 on success otherwise non-zero for error and it woll be reported to stderr
int cbc_decrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,int (decrypt_block)(const uchar_t* , uchar_t*, const void* ) ) ;

// returns 0 on success otherwise non-zero for error and it woll be reported to stderr
int ofb_encrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,int (encrypt_block)(const uchar_t* , uchar_t*, const void* ) ) ;
// returns 0 on success otherwise non-zero for error and it woll be reported to stderr
int ofb_decrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,int (decrypt_block)(const uchar_t* , uchar_t*, const void* ) ) ;


// returns 0 on success otherwise non-zero for error and it woll be reported to stderr
int cfb_encrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,int (encrypt_block)(const uchar_t* , uchar_t*, const void* ) ) ;
// returns 0 on success otherwise non-zero for error and it woll be reported to stderr
int cfb_decrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,int (decrypt_block)(const uchar_t* , uchar_t*, const void* ) ) ;


// returns 0 on success otherwise non-zero for error and it woll be reported to stderr
int ctr_encrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,int (encrypt_block)(const uchar_t* , uchar_t* , const void* ) ) ;
// returns 0 on success otherwise non-zero for error and it woll be reported to stderr
int ctr_decrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,int (decrypt_block)(const uchar_t* , uchar_t* , const void* ) ) ;


// returns 0 on success otherwise non-zero for error and it woll be reported to stderr
int gcm_encrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,int (encrypt_block)(const uchar_t* , uchar_t*, const void* ) ) ;
// returns 0 on success otherwise non-zero for error and it woll be reported to stderr
int gcm_decrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,int (decrypt_block)(const uchar_t* , uchar_t*, const void* ) ) ;



#endif