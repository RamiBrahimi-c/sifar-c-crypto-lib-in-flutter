#ifndef SUBSTITUTION_C
#define SUBSTITUTION_C


#include "../../../include/ciphers/classical/substitution.h"
#include "../../../include/common/utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>





void substittution_encrypt(const uchar_t* input, uchar_t* output, const void* key) {
    assert(key != NULL && "key is null");
    SubstitutionKey *substitution_key = (SubstitutionKey *) (key) ;


    size_t length = strlen((const char*) input) ; 

    for (size_t i = 0; i < length ; i++)
    {
        int index  =  (input[i] - 'A')  ;
        output[i] =  substitution_key->map[index] ;  
    }
    

}

void substittution_decrypt(const uchar_t* input, uchar_t* output, const void* key) {
    assert(key != NULL && "key is null");
    SubstitutionKey *substitution_key = (SubstitutionKey *) (key) ;


    size_t length = strlen((const char*) input) ; 

    for (size_t i = 0; i < length ; i++)
    {
        int value  =  findIndexMap(substitution_key->map , input[i] )  ;
        printf("found value : %d \t" , value) ; 
        output[i] =  value + 'A';  
        printf("inserted : %d \n" ,output[i] ) ; 
    }
    

}

void substittution_set_key(void* key_struct, const uchar_t* key_str , size_t key_len) {
    SubstitutionKey *substitution_key = (SubstitutionKey*) key_struct ;
    // CONVENTION : i will be working on alphabets
    
    substitution_key->alphabet_size = ALPHABET_LENGTH_SUBSTITUTION ; 
    

    if (substitution_key->alphabet_size == 26)
    {
        // we gotta fill our map
        // initilizeAlphabets(substitution_key->map , 'l' ) ;
        if (key_len != substitution_key->alphabet_size   )
        {
            printf("ERROR: key too short to be used in substitution encryption\n");
            return ; 
        }
        
        if (!areElementsUniqueSorted((const uchar_t*) key_str , substitution_key->alphabet_size)   )
        {
            printf("ERROR: key has duplicates values therefore it cant be used\n");
            return ; 
        }
        
        // copy it
        memcpy( substitution_key->map , key_str ,key_len ) ; 
        
    }
    


}

void substittution_free_key(void* key_struct);

Cipher* get_substittution_cipher(void);




#endif
