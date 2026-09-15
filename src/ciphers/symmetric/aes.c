#ifndef AES_C
#define AES_C


#include "aes.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>     



#include <stdio.h>
// #include "../../common/keyexpan.h"

// #include "../../common/galois_field_op.h"
// #include "../../common/rijnbox.h"
#include "../../block_cipher_modes_operation.h"

#define AES_BLOCK_SIZE 16





void fill_state(uchar_t *input  , int index, uchar_t state[4][4]) {
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] = input[index] ; 
            index++ ; 
        }
        
    }
    
}


void add_round_key(uchar_t *key , int k , uchar_t state[4][4]) {
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] ^= key[k] ;
            k++ ;  
        }
        
    }
    
}

/*
    the function that is used in AES rounds !!!!
*/
void sub_bytes(uchar_t state[4][4] ) {
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] = subByte(state[j][i]) ; 
        }
        
    }
    
}

void rev_sub_bytes(uchar_t state[4][4] ) {
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] = rev_subByte(state[j][i]) ; 
        }
        
    }
    
}




// Function to reverse a portion of the array
void reverse(uchar_t* arr, int start, int end) {
    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

// Function to rotate an array by d elements to the left
void rotateArr(uchar_t* arr, int n, int d) {
    
    // Handle the case where d > size of array
    d %= n;

    // Reverse the first d elements
    reverse(arr, 0, d - 1);

    // Reverse the remaining n-d elements
    reverse(arr, d, n - 1);

    // Reverse the entire array
    reverse(arr, 0, n - 1);
}

// Function to rotate an array by d elements to the right
void rotateArrR(uchar_t* arr, int n, int d) {
    
    // Handle the case where d > size of array
    d %= n;

    // Reverse the last d elements
    reverse(arr, n -d, n - 1 );

    // Reverse the remaining n-d elements
    reverse(arr, 0, n-d-1 );

    // Reverse the entire array
    reverse(arr, 0, n - 1);
}


void shift_rows(uchar_t state[4][4]) {
    int rotation_num = 0 ; 
    for (int i = 0; i < 4; i++)
    {
        rotateArr(state[i] ,4 , rotation_num ) ;         
        rotation_num++ ; 
    }
    
}


void inv_shift_rows(uchar_t state[4][4]) {
    int rotation_num = 0 ; 
    for (int i = 0; i < 4; i++)
    {
        rotateArrR(state[i] ,4 , rotation_num ) ;         
        rotation_num++ ; 
    }
    
}




void mix_culumns(uchar_t state[4][4]) {
    uchar_t temp0 , temp1 , temp2 , temp3 ; 
    for (int i = 0; i < 4; i++)
    {
        temp0 = state[0][i] ; 
        temp1 = state[1][i] ; 
        temp2 = state[2][i] ; 
        temp3 = state[3][i] ; 

        state[0][i] = ( mod_GF(mul_GF_16bit(0x02 , temp0 )) ^ mod_GF(mul_GF_16bit(0x03 , temp1 )) ^  temp2 ^ temp3) ;   
        
        state[1][i] = temp0 ^ mod_GF(mul_GF_16bit(0x02 , temp1 )) ^ mod_GF(mul_GF_16bit(0x03 , temp2 )) ^ temp3 ;   
        
        state[2][i] = temp0 ^ temp1 ^  mod_GF(mul_GF_16bit(0x02 , temp2 )) ^ mod_GF(mul_GF_16bit(0x03 , temp3 )) ;   
        
        state[3][i] = mod_GF(mul_GF_16bit(0x03 , temp0 )) ^ temp1 ^  temp2 ^ mod_GF(mul_GF_16bit(0x02 , temp3 )) ;   
    }
    
}



void rev_mix_culumns(uchar_t state[4][4]) {
    uchar_t temp0 , temp1 , temp2 , temp3 ; 
    for (int i = 0; i < 4; i++)
    {
        temp0 = state[0][i] ; 
        temp1 = state[1][i] ; 
        temp2 = state[2][i] ; 
        temp3 = state[3][i] ; 

        state[0][i] =(uchar_t) mod_GF(mul_GF_16bit(0x0e , temp0 )) ^ mod_GF(mul_GF_16bit(0x0b , temp1 )) ^  mod_GF(mul_GF_16bit(0x0d ,temp2)) ^ mod_GF(mul_GF_16bit(0x09 ,temp3 )) ;   
        
        state[1][i] = mod_GF(mul_GF_16bit(0x09 , temp0 )) ^ mod_GF(mul_GF_16bit(0x0e , temp1 )) ^ mod_GF(mul_GF_16bit(0x0b , temp2 )) ^ mod_GF(mul_GF_16bit(0x0d , temp3 )) ;   
        
        state[2][i] = mod_GF(mul_GF_16bit(0x0d , temp0 )) ^ mod_GF(mul_GF_16bit(0x09 , temp1)) ^  mod_GF(mul_GF_16bit(0x0e , temp2 )) ^ mod_GF(mul_GF_16bit(0x0b , temp3 )) ;   
        
        state[3][i] = mod_GF(mul_GF_16bit(0x0b , temp0 )) ^ mod_GF(mul_GF_16bit(0x0d , temp1 )) ^ mod_GF(mul_GF_16bit(0x09 , temp2 )) ^ mod_GF(mul_GF_16bit(0x0e , temp3 )) ;   
    }
    
}




void fill_state_inv(uchar_t *key  , int k, uchar_t state[4][4]) {
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            key[k] = state[j][i] ; 
            k++ ; 
        }
        
    }
    
}



static void aes_cipher_block(const uchar_t *input   , uchar_t *output ,const void *key ) {
    assert(key != NULL && "key is null");
    AesKey *aes_key = (AesKey *) (key) ;
    assert(aes_key != NULL && "aes_key is null");
    
    uchar_t state[4][4] ; 
    fill_state(input , 0 , state) ;


    add_round_key(aes_key->expanded_key , 0 , state) ; 


    for (int i = 1; i <= aes_key->Nr -1 ; i++)
    {

        sub_bytes(state) ; 
        
        shift_rows(state) ; 

        mix_culumns(state) ; 

        add_round_key(aes_key->expanded_key , 4 * i * 4  , state) ; 

    }

    sub_bytes(state) ; 

    shift_rows(state) ; 

    add_round_key(aes_key->expanded_key , 4 * aes_key->Nr * 4  , state) ; 
    
    fill_state_inv(output , 0 , state) ; 

}



static void aes_cipher_inverse_block(const uchar_t *input   , uchar_t *output ,const void *key ) {
    assert(key != NULL && "key is null");
    AesKey *aes_key = (AesKey *) (key) ;
    assert(aes_key != NULL && "aes_key is null");

    
    uchar_t state[4][4] ; 
    fill_state(input , 0 , state) ;


    add_round_key(aes_key->expanded_key , 4 * aes_key->Nr * 4 , state) ; 


    for (int i = aes_key->Nr - 1; i >= 1 ; i--)
    {
        inv_shift_rows(state) ; 
        
        rev_sub_bytes(state) ; 


        add_round_key(aes_key->expanded_key , 4 * i * 4  , state) ; 
        

        
        rev_mix_culumns(state) ; 



    }

    inv_shift_rows(state) ; 


    rev_sub_bytes(state) ; 



    add_round_key(aes_key->expanded_key , 0  , state) ; 
    
    fill_state_inv(output , 0 , state) ; 

}


void setup_parameteres_aes(AES_TYPE type , int *Nr , int *Nk) {
    if (type == AES128 ) {
        *Nk = 4 ; 
        *Nr = 10 ; 
    }
    if (type == AES192 ) {
        *Nk = 6 ; 
        *Nr = 12 ; 
    }
    if (type == AES256 ) {
        *Nk = 8 ; 
        *Nr = 14 ; 
    }
}


/*
    for now only input with length of 16*k bytes is supported
*/
int aes_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key)
{

    uchar_t *iv = malloc(sizeof(uchar_t)*AES_BLOCK_SIZE) ;
    blockcipher_encrypt_modeop(input , output , iv , length , AES_BLOCK_SIZE , key , aes_cipher_block) ;
    free(iv);    
    

    return 0 ; 
}



int aes_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key)
{

    uchar_t *iv = malloc(sizeof(uchar_t)*AES_BLOCK_SIZE) ;
    blockcipher_decrypt_modeop(input , output , iv , length , AES_BLOCK_SIZE , key , aes_cipher_inverse_block) ;
    free(iv);    

    return 0 ; 
}


#include <assert.h>

int aes_set_key(void* key_struct, const uchar_t* key_str , size_t key_len)
{
    AesKey *aes_key = (AesKey *) key_struct ;

    if (!key_str)
    {

        fprintf(stderr , "ERROR: key @key_str is null\n") ; 
        return 1 ; 

    
    } else {
        int temp_len = key_len ; 
        if (temp_len == 16)
        {
            printf("INFO: AES128 chosen !\n");
            aes_key->mode = AES128 ; 
            memcpy(aes_key->key ,key_str , key_len );
            aes_key->key_length = 16 ;  
        }
        else if (temp_len == 24)
        {
            printf("INFO: AES196 chosen !\n");
            
            aes_key->mode = AES192 ; 
            memcpy(aes_key->key ,key_str , key_len );
            aes_key->key_length = 24 ;  
            
        }
        else if (temp_len == 32)
        {
            printf("INFO: AES256 chosen !\n");
            aes_key->mode = AES256 ; 
            memcpy(aes_key->key ,key_str , key_len );
            aes_key->key_length = 32 ;  
            
        } else {
            fprintf(stderr , "ERROR: key length is not compatible with AES standard {16,24,32} bytes\n") ; 
            return 2 ;

            // TODO : assigning random key here is much wiser :)
            printf("INFO: AES128 is assigned with custom standard key  !\n");
            aes_key->mode = AES128 ;
            uchar_t temp_standard_key[16] = {0x2b  ,0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c } ; 
            memcpy(aes_key->key ,temp_standard_key , 16 );
            aes_key->key_length = 16 ;  

        }
        
    }
    set_sbox(sbox) ; 
    
    set_rev_sbox(rev_sbox);


    uchar_t rci[10] ; 

    build_rci(rci) ;

    // PRINT_ARRAY(rci , 10 , "%x" ) ; 
        
    
    setup_parameteres_aes(aes_key->mode , &aes_key->Nr , &aes_key->Nk ) ;
    printf("INFO: mode : %d %d %d\n" , aes_key->mode , aes_key->Nr , aes_key->Nk);
    aes_key->expanded_key_length = 4 * (aes_key->Nr+1) * 4 ; 
    printf("INFO: len : %ld \n" , aes_key->expanded_key_length);
    
    aes_key->expanded_key = malloc(sizeof(uchar_t)*aes_key->expanded_key_length) ; 
    
    assert(aes_key->expanded_key != NULL && "aes key expanded is null");
    

    key_expan(aes_key->key , aes_key->expanded_key ,aes_key->Nk ,aes_key->Nr , rci   ) ; 
    
    aes_key->type = BLOCK_CIPHER ;

    return 0 ; 
}



int aes_free_key(void* key_struct)
{

    return 0  ;
}


#endif
