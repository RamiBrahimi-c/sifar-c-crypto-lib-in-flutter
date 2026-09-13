#ifndef BLOCK_CIPHER_MODES_OPERATION_H
#define BLOCK_CIPHER_MODES_OPERATION_H

#include "cipher_interface.h"
#include "common/utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


void blockcipher_encrypt_modeop(uchar_t *input , uchar_t *output ,uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*encrypt_block)(const uchar_t* , uchar_t*, const void* )) ;
void blockcipher_decrypt_modeop(uchar_t *input , uchar_t *output ,uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*decrypt_block)(const uchar_t* , uchar_t*, const void* )) ;


void ecb_encrypt(uchar_t *input , uchar_t *output , size_t length , size_t block_size  ,const void* key ,void (*encrypt_block)(const uchar_t* , uchar_t* , const void* ) ) ;
void ecb_decrypt(uchar_t *input , uchar_t *output , size_t length , size_t block_size  ,const void* key ,void (*decrypt_block)(const uchar_t* , uchar_t* , const void* ) ) ;

void cbc_encrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*encrypt_block)(const uchar_t* , uchar_t*, const void* ) ) ;
void cbc_decrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*decrypt_block)(const uchar_t* , uchar_t*, const void* ) ) ;

void ofb_encrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*encrypt_block)(const uchar_t* , uchar_t*, const void* ) ) ;
void ofb_decrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*decrypt_block)(const uchar_t* , uchar_t*, const void* ) ) ;


void cfb_encrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*encrypt_block)(const uchar_t* , uchar_t*, const void* ) ) ;
void cfb_decrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*decrypt_block)(const uchar_t* , uchar_t*, const void* ) ) ;


void ctr_encrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*encrypt_block)(const uchar_t* , uchar_t* , const void* ) ) ;
void ctr_decrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*decrypt_block)(const uchar_t* , uchar_t* , const void* ) ) ;


void gcm_encrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*encrypt_block)(const uchar_t* , uchar_t*, const void* ) ) ;
void gcm_decrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*decrypt_block)(const uchar_t* , uchar_t*, const void* ) ) ;



#endif