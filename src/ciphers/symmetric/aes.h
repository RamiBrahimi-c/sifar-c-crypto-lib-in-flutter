#ifndef AES_H
#define AES_H

#include "../../cipher_interface.h"
#include "../../common/constants.h"
#include <stdint.h>



#include <stdio.h>
#include "../../common/keyexpan.h"
#include "../../common/galois_field_op.h"
#include "../../common/rijnbox.h"
//  extern uchar_t sbox[64][64] ; 


typedef enum AES_TYPE {
    AES128 , 
    AES192 , 
    AES256 
}AES_TYPE;

void fill_state(uchar_t *input  , int index, uchar_t state[4][4]);

void add_round_key(uchar_t *key , int k , uchar_t state[4][4]) ;
/*
    the function that is used in AES rounds !!!!
*/
void sub_bytes(uchar_t state[4][4] ) ;
void rev_sub_bytes(uchar_t state[4][4] ) ;
// C Code to left rotate an array using Reversal Algorithm

#include <stdio.h>

// Function to reverse a portion of the array
void reverse(uchar_t* arr, int start, int end);

// Function to rotate an array by d elements to the left
void rotateArr(uchar_t* arr, int n, int d) ;
void rotateArrR(uchar_t* arr, int n, int d) ;


void shift_rows(uchar_t state[4][4]) ;
void inv_shift_rows(uchar_t state[4][4]) ;


void mix_culumns(uchar_t state[4][4]) ;
void rev_mix_culumns(uchar_t state[4][4]) ;

void fill_state_inv(uchar_t *key  , int k, uchar_t state[4][4]) ;


static void aes_cipher_block(const uchar_t *input   , uchar_t *output ,const void *key ) ;
static void aes_cipher_inverse_block(const uchar_t *input   , uchar_t *output ,const void *key ) ;
void aes_cipher(uchar_t *input   , uchar_t *output, uchar_t *key  , int length, int Nr) ;
void aes_cipher_decrypt(uchar_t *input   , uchar_t *output, uchar_t *key  , int length, int Nr) ;





// Opaque type – actual definition in aes.c
typedef struct AesKey AesKey;

typedef struct AesKey {
    AES_TYPE mode ; // aes126 , aes192 or aes256
    uchar_t key[32] ; // max is 32 ...
    size_t key_length ;
    uchar_t *expanded_key ;
    size_t expanded_key_length ;
    int Nr ; // rounds number 
    int Nk ; // number of 32 bit words compromising the key
    CipherType type;
}AesKey;


int aes_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int aes_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int aes_set_key(void* key_struct, const uchar_t* key_str , size_t key_len);
int aes_free_key(void* key_struct);

Cipher* get_aes_cipher(void);

#endif
