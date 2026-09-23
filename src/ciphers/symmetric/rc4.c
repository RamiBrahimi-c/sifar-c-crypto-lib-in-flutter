#ifndef RC4_C
#define RC4_C


#include "rc4.h"
#include "../../common/utils.h"
#include "../../common/constants.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>




int rc4_encrypt(const uchar_t* input, uchar_t* output , int length, const void* key) {
    Rc4Key *rc4_key = (Rc4Key *) (key) ;

    if (rc4_key == NULL) {
        fprintf(stderr , "ERROR: rc4_key is NULL\n") ; 
        return 1 ; 
    }


    // printf("======= printing info Rc4 Key =======\n");
    // printf("length of key : %d \n" , rc4_key->length);
    // printf("length of input : %d \n" , length);
    
    // printf("======= starting encryption =======\n");
    // printf("???") ; 
     
    int c = 0 ; 
    
    Rc4Key rc4_key_clone ;
    memcpy(&rc4_key_clone , rc4_key , sizeof(Rc4Key)) ; 

    // printf("i = %d \n j = %d \n length = %d " , rc4_key_clone.i
                // , rc4_key_clone.j
                // , rc4_key_clone.length);
    while (c <length)
    {
        rc4_key_clone.i = (rc4_key_clone.i + 1 ) % 256 ; 
        rc4_key_clone.j = (rc4_key_clone.j + rc4_key_clone.state[rc4_key_clone.i] ) % 256 ;
        
        swapUnsignedChar(rc4_key_clone.state + rc4_key_clone.i , rc4_key_clone.state + rc4_key_clone.j  );
        uchar_t encrypting_byte = rc4_key_clone.state[(rc4_key_clone.state[rc4_key_clone.i]  + rc4_key_clone.state[rc4_key_clone.j]) % 256 ] ;  
        // supposing i is the index 
        output[c] = encrypting_byte ^ input[c] ; 
        c++ ; 
    }
    // printf("c = %d \n" , c) ; 
    return 0 ; 
}


int rc4_decrypt(const uchar_t* input, uchar_t* output , int length, const void* key) {
    // assert(key != NULL && "key is null");
    Rc4Key *rc4_key = (Rc4Key *) (key) ;

    if (rc4_key == NULL) {
        fprintf(stderr , "ERROR: rc4_key is NULL\n") ; 
        return 1 ; 
    }
    

    // printf("======= printing info Rc4 Key =======\n");
    // printf("length : %d \n" , rc4_key->length);
    
    
    int c = 0 ; 
    
    while (c <length)
    {
        rc4_key->i = (rc4_key->i + 1 ) % rc4_key->length ; 
        rc4_key->j = (rc4_key->j + rc4_key->state[rc4_key->i] ) % rc4_key->length ;
        
        swapUnsignedChar(rc4_key->state + rc4_key->i , rc4_key->state + rc4_key->j  );
        uchar_t encrypting_byte = rc4_key->state[(rc4_key->state[rc4_key->i]  + rc4_key->state[rc4_key->j]) % rc4_key->length ] ;  
        // supposing i is the index 
        output[c] = encrypting_byte ^ input[c] ; 
        c++ ; 
    }
    return 0 ; 
}

int rc4_set_key(void* key_struct, const uchar_t* key_str , size_t key_len) {
    Rc4Key *rc4_key = (Rc4Key *) key_struct ;
    if (rc4_key==NULL) {
        fprintf(stderr , "ERROR: rc4_key is NULL\n") ; 
        return 1 ; 
    }

    rc4_key->length = RC4_KEY_MAX_SIZE ; 

    for (size_t i = 0; i < rc4_key->length ; i++)
    {
        rc4_key->state[i] = i ; 
    }

    uchar_t j = 0 ; 
    int key_length = key_len ; 

    for (size_t i = 0; i < rc4_key->length; i++)
    {
        j = (j + rc4_key->state[i] + key_str[i % key_length]) % rc4_key->length  ; 
        swapUnsignedChar(rc4_key->state + i ,rc4_key->state + j ) ;
    }
    rc4_key->i = 0 ; 
    rc4_key->j = 0 ; 
    
    rc4_key->type = STREAM_CIPHER ;

    return 0 ; 
}

int rc4_free_key(void* key_struct) {
    free(key_struct) ; 
    return 0  ;
}

Cipher* get_rc4_cipher(void);







#endif