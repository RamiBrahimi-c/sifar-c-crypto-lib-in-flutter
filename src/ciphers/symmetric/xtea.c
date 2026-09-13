#include "xtea.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
// #include "../../block_cipher_modes_operation.h"
#include "../../../include/block_cipher_modes_operation.h"
// TODO : maybe we need some refacturing here ?? idk
// who is even gonna notice *-* ????

#define XTEA_BLOCK_SIZE 8 

/*
    uchar_t *vv : gotta be size of 8 bytes
    uchar_t *kk : gotta be size of 16 bytes
*/
static void xtea_decrypt_block(const uchar_t *vv ,uchar_t *output,const void *key) {
    assert(key != NULL && "key is null");
    XTeaKey *xtea_key = (XTeaKey *) (key) ;
    uchar_t *kk = xtea_key->key ;

    uint32_t v[2] ;
    uint32_t k[4] ;

        // set up `v` and `k`
    v[0] = (vv[0] << 24) | (vv[1] << 16) | (vv[2] << 8) | (vv[3]) ; 
    v[1] = (vv[4] << 24) | (vv[5] << 16) | (vv[6] << 8) | (vv[7]) ; 



    k[0] = (kk[0]  << 24) | (kk[1]  << 16) | (kk[2]  << 8) | (kk[3]) ; 
    k[1] = (kk[4]  << 24) | (kk[5]  << 16) | (kk[6]  << 8) | (kk[7]) ; 
    k[2] = (kk[8]  << 24) | (kk[9]  << 16) | (kk[10] << 8) | (kk[11]) ; 
    k[3] = (kk[12] << 24) | (kk[13] << 16) | (kk[14] << 8) | (kk[15]) ; 


    uint32_t z , y ; 
    uint32_t delta = 0x9e3779b9 ; // (sqrt(5)-1)* 2 ^ 31
    uint32_t sum = delta*32 ; 
    uint32_t n , m ; 
    uint32_t a , b , c , d , e , f ; 

    
            z = v[0] ; 
            y = v[1] ; 

    for (size_t i = 0; i < 32; i++)
    {

        // key set up 
        m = sum & 0x3 ; 
        
        
        a = (z << 4) ^ (z >> 5) ;  
        a += z  ;  
        
        b =    (sum + k[m]) ;
        
        c = a ^ b ;
        y -= c  ;
        
        
        
        sum -= delta ;
        

        d = (y << 4) ^ (y >> 5)  ;  
        d += y ; 
        
        n = (sum >> 11) & 0x3 ;   
        e = d ^  (sum + k[n]) ;
        
        
        z -=  e ;

        


    }

    v[0] = z ;
    v[1] = y ;

        // copy back result to output ...
    output[0] = v[0] >> 24 ; 
    output[1] = v[0] >> 16 ; 
    output[2] = v[0] >> 8 ; 
    output[3] = v[0]  ; 


    output[4] = v[1] >> 24 ; 
    output[5] = v[1] >> 16 ; 
    output[6] = v[1] >> 8 ; 
    output[7] = v[1]  ; 

    
} 


/*
    uchar_t *vv : gotta be size of 8 bytes
    uchar_t *kk : gotta be size of 16 bytes
*/
static void xtea_encrypt_block(const uchar_t *vv  , uchar_t *output,const void *key) {
    assert(key != NULL && "key is null");
    XTeaKey *xtea_key = (XTeaKey *) (key) ;
    uchar_t *kk = xtea_key->key ;

    // printf("") ; 
    // the specifications has `long *` but long could be 8 bytes so 
    // imma limit it to exactly 32 bit word
    uint32_t v[2] ;
    uint32_t k[4] ;

        // set up `v` and `k`
    v[0] = (vv[0] << 24) | (vv[1] << 16) | (vv[2] << 8) | (vv[3]) ; 
    v[1] = (vv[4] << 24) | (vv[5] << 16) | (vv[6] << 8) | (vv[7]) ; 



    k[0] = (kk[0]  << 24) | (kk[1]  << 16) | (kk[2]  << 8) | (kk[3]) ; 
    k[1] = (kk[4]  << 24) | (kk[5]  << 16) | (kk[6]  << 8) | (kk[7]) ; 
    k[2] = (kk[8]  << 24) | (kk[9]  << 16) | (kk[10] << 8) | (kk[11]) ; 
    k[3] = (kk[12] << 24) | (kk[13] << 16) | (kk[14] << 8) | (kk[15]) ; 


    uint32_t z , y ; 
    uint32_t sum = 0 ; 
    uint32_t delta = 0x9e3779b9 ; // (sqrt(5)-1)* 2 ^ 31
    uint32_t n , m ; 
    uint32_t a , b , c , d , e , f ; 

    
            z = v[0] ; 
            y = v[1] ; 

    for (size_t i = 0; i < 32; i++)
    {

        // key set up 

        d = (y << 4) ^ (y >> 5)  ;  
        d += y ; 
        
        n = (sum >> 11) & 0x3 ;   
        e = d ^  (sum + k[n]) ;
        
        
        z = z + e ;
        
        
        sum += delta ;
        
        m = sum & 0x3 ; 
        
        
        a = (z << 4) ^ (z >> 5) ;  
        a += z  ;  
        
        b =    (sum + k[m]) ;
        
        c = a ^ b ;
        y = c + y ;
        

        


    }

    v[0] = z ;
    v[1] = y ;

        // copy back result to *output ...
    output[0] = v[0] >> 24 ; 
    output[1] = v[0] >> 16 ; 
    output[2] = v[0] >> 8 ; 
    output[3] = v[0]  ; 


    output[4] = v[1] >> 24 ; 
    output[5] = v[1] >> 16 ; 
    output[6] = v[1] >> 8 ; 
    output[7] = v[1]  ; 
}



void xtea_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key) {

    uchar_t *iv = malloc(sizeof(uchar_t)*XTEA_BLOCK_SIZE) ;
    blockcipher_encrypt_modeop(input , output , iv , length , XTEA_BLOCK_SIZE , key , xtea_encrypt_block) ;
    free(iv);    

    
}


void xtea_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key) {

    uchar_t *iv = malloc(sizeof(uchar_t)*XTEA_BLOCK_SIZE) ;
    blockcipher_decrypt_modeop(input , output , iv , length , XTEA_BLOCK_SIZE , key , xtea_decrypt_block) ;
    free(iv);
    
}


void xtea_set_key(void* key_struct, const uchar_t* key_str , size_t key_len) {
    XTeaKey *xtea_key = (XTeaKey *) key_struct ;
    
    // how can we make sure that key_str is actually TEA_KEY_MAX_SIZE bytes ...
    assert(key_len >= 16 && "key length here must be 16 bytes");    
    memcpy(xtea_key->key , key_str , sizeof(uchar_t)*XTEA_KEY_MAX_SIZE) ; 
    xtea_key->length = XTEA_KEY_MAX_SIZE   ; 
    xtea_key->type = BLOCK_CIPHER ; 
}


void xtea_free_key(void* key_struct);

Cipher* get_xtea_cipher(void);








