#include "hash.h"


#define SHA256_BLOCK_SIZE 64
#define SHA256_DIGEST_SIZE 32
#define SHA512_BLOCK_SIZE 128
#define SHA512_DIGEST_SIZE 64

/*
    gcc was saying : 
    `src/ciphers/hashing/hmac.c:12:1: warning: multi-line comment [-Wcomment]
   12 | //x / \
      | ^
    `
    you serious :( ??? 
// \o/
//  |
// / \ 
*/

void hmac_sha256(const uchar_t* key, size_t key_len, const uchar_t* data, size_t data_len, uchar_t mac[SHA256_DIGEST_SIZE]) {

    size_t B = SHA256_BLOCK_SIZE;
    // size_t t = SHA256_BLOCK_SIZE;
    uchar_t K0[SHA256_BLOCK_SIZE]  ;
    uchar_t ipad = 0x36 ;                  
    uchar_t opad = 0x5c ;                  

    if (key_len == B)
    {
        // step 1 : set key to K0
        memcpy(K0 , key , key_len) ; 
    } else if (key_len > B) {
        // step 2 : we hash key then we append 0s to reach the size of B
        uchar_t L[SHA256_DIGEST_SIZE] ; 
        
        sha256_hash(key , key_len , L) ;
        memcpy(K0 , L , SHA256_DIGEST_SIZE) ; 
        memset(K0 + SHA256_DIGEST_SIZE , 0 , SHA256_DIGEST_SIZE) ; 
        
    } else {
        // step 3 : we copy key and pad with 0s until we reach size of B
        memcpy(K0 , key , key_len) ; 
        memset(K0 + key_len , 0 , B-key_len) ; 

    }
    
    // step 4 : just XOR K0 and 0x36
    uchar_t ipad_K0[SHA256_BLOCK_SIZE] ; 
    
    for (size_t i = 0; i < B; i++)
    {
        ipad_K0[i] = K0[i]  ^ ipad ; 
    }
    
    // step 5 : we concatenate ipad_K0 and data
    uchar_t *result1 = malloc(sizeof(uchar_t)*(B+data_len)) ;
    assert(result1 != NULL && "malloc failed") ; 
    
    uchar_t result2[SHA256_DIGEST_SIZE] ; 
    memcpy(result1 , ipad_K0 , sizeof(uchar_t)*B) ;  
    memcpy(result1 + B, data , sizeof(uchar_t)*data_len) ;  
    
    // step 6 : hash the ipad_K0 || data
    sha256_hash(result1 , B + data_len , result2 ) ; 
    
    // step 7 : just XOR K0 and 0x5c

    uchar_t opad_K0[SHA256_BLOCK_SIZE] ; 
    for (size_t i = 0; i < B; i++)
    {
        opad_K0[i] = K0[i]  ^ opad ; 
    }
    
    // step 8 : we concatenate opad_K0 and hashed(ipad_K0 || data)
    uchar_t *result3 = malloc(sizeof(uchar_t)*(B+SHA256_DIGEST_SIZE)) ;
    assert(result1 != NULL && "malloc failed") ; 
     
    memcpy(result3 , opad_K0 , sizeof(uchar_t)*B) ;  
    memcpy(result3 + B, result2 , sizeof(uchar_t)*SHA256_DIGEST_SIZE) ;  
    
    // step 9 : hash the last concatenated result from (step9)
    // uchar_t digest_final[SHA256_DIGEST_SIZE] ; 
    sha256_hash(result3 , B + SHA256_DIGEST_SIZE , mac ) ; 
    
    // step 10 : by default t is the same as H digest length ...

}

// for explanation i done it above im quite lazy to repeat all of that \o/
// (ended up doing it ...)
void hmac_sha512(const uchar_t* key, size_t key_len,
                 const uchar_t* data, size_t data_len,
                 uchar_t mac[SHA512_DIGEST_SIZE]) {

    size_t B = SHA512_BLOCK_SIZE;
    // size_t t = SHA512_BLOCK_SIZE;
    uchar_t K0[SHA512_BLOCK_SIZE]  ;
    uchar_t ipad = 0x36 ;                  
    uchar_t opad = 0x5c ;                  

    if (key_len == B)
    {
        // step 1 : set key to K0
        memcpy(K0 , key , key_len) ; 
    } else if (key_len > B) {
        // step 2 : we hash key then we append 0s to reach the size of B
        
        uchar_t L[SHA512_DIGEST_SIZE] ; 
        sha512_hash(key , key_len , L) ;
        memcpy(K0 , L , SHA512_DIGEST_SIZE) ; 
        memset(K0 + SHA512_DIGEST_SIZE , 0 , SHA512_DIGEST_SIZE) ; 
        
    } else {
        // step 3 : we copy key and pad with 0s until we reach size of B
        printf("case 3 !!!\n") ; 
        memcpy(K0 , key , key_len) ; 
        memset(K0 + key_len , 0 , B-key_len) ; 

    }
    
    // step 4 : just XOR K0 and 0x36
    uchar_t ipad_K0[SHA512_BLOCK_SIZE] ; 
    
    for (size_t i = 0; i < B; i++)
    {
        ipad_K0[i] = K0[i]  ^ ipad ; 
    }
    
    // step 5 : we concatenate ipad_K0 and data
    uchar_t *result1 = malloc(sizeof(uchar_t)*(B+data_len)) ;
    assert(result1 != NULL) ; 
    
    uchar_t result2[SHA512_DIGEST_SIZE] ; 
    memcpy(result1 , ipad_K0 , sizeof(uchar_t)*B) ;  
    memcpy(result1 + B, data , sizeof(uchar_t)*data_len) ;  
    
    // step 6 : hash the ipad_K0 || data
    sha512_hash(result1 , B + data_len , result2 ) ; 
    
    // step 7 : just XOR K0 and 0x5c
    uchar_t opad_K0[SHA512_BLOCK_SIZE] ; 
    
    for (size_t i = 0; i < B; i++)
    {
        opad_K0[i] = K0[i]  ^ opad ; 
    }
    
    // step 8 : we concatenate opad_K0 and hashed(ipad_K0 || data)
    uchar_t *result3 = malloc(sizeof(uchar_t)*(B+SHA512_DIGEST_SIZE)) ;
    assert(result3 != NULL) ; 
    
    
    memcpy(result3 , opad_K0 , sizeof(uchar_t)*B) ;  
    memcpy(result3 + B, result2 , sizeof(uchar_t)*SHA512_DIGEST_SIZE) ;  
    
    
    // step 9 : hash the last concatenated result from (step9)
    // uchar_t digest_final[SHA512_DIGEST_SIZE] ; 
    sha512_hash(result3 , B + SHA512_DIGEST_SIZE , mac ) ; 
    // step 10 : by default t is the same as H digest length ...

    free(result1) ; 
    free(result3) ; 
}
