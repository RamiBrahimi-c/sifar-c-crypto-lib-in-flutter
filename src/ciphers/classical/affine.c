#ifndef AFFINE_C
#define AFFINE_C

#include "affine.h"
#include "../../common/utils.h"
#include "../../common/constants.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>




int affine_encrypt(const uchar_t* input,uchar_t* output , size_t length, const void* key) {
    assert(key != NULL && "key is null");
    AffineKey *affine_key = (AffineKey *) (key) ;
    
    
    // int length = strlen((char *) input) ; 
    printf("alpha num : %d \n" , ALPHABET_LENGTH) ; 
    printf("length : %ld\n" , length ) ; 
    
    for (size_t i = 0; i < length; i++)
    {
        output[i] = ((affine_key->a * input[i]) + affine_key->b) % ALPHABET_LENGTH ;
        
    }
    return 0 ; 
}


int affine_decrypt(const uchar_t* input,uchar_t* output  , size_t length, const void* key) {
    assert(key != NULL && "key is null");
    AffineKey *affine_key = (AffineKey *) (key) ;
    
    
    
    uint16_t inv_a = modInverse(affine_key->a , ALPHABET_LENGTH) ;
    printf("mod multiplicative inv : %u \n" , inv_a); 
    for (size_t i = 0; i < length; i++)
    {
        output[i] = ( (input[i] - affine_key->b) * inv_a ) % ALPHABET_LENGTH ;
    }
    

    return 0 ; 
}


int affine_set_key(void* key_struct, const uchar_t* key_str , size_t key_len) {
    AffineKey *affineKey = (AffineKey*) key_struct ;
    if (affineKey) {
        fprintf(stderr , "stderr: affineKey is null \n") ; 
        return 1 ; 
    }
    uint64_t key_num = atoi(key_str) ; 
    
    assert(key_num>0 && isCoprime(key_num , ALPHABET_LENGTH) == 1 && "key must be coprime with alphabet number ");

    affineKey->a = key_num; 
    // affineKey->b = 0; 
    affineKey->b = key_num ^ UINT64_MAX; 
    
    printf(" a = %lu   b = %lu  \n" , affineKey->a , affineKey->b );

    return 0 ; 
}  



int affine_free_key(void* key_struct)  {
    free(key_struct) ; 
    return 0 ; 
}

Cipher* get_affine_cipher(void);

#endif