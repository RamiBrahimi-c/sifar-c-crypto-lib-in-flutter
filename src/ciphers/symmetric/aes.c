#ifndef AES_C
#define AES_C


#include "aes.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>     



#include <stdio.h>
// #include "../../common/keyexpan.h"

// #include "../../common/galois_field_op.h"
#include "../../block_cipher_modes_operation.h"


#define AES_BLOCK_SIZE 16





static void fill_state(uchar_t *input  , int index, uchar_t state[4][4]) {
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] = input[index] ; 
            index++ ; 
        }
        
    }
    
}


static void add_round_key(uchar_t *key , int k , uchar_t state[4][4]) {
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
static void sub_bytes(uchar_t state[4][4] ) {
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] = subByte(state[j][i]) ; 
        }
        
    }
    
}

static void rev_sub_bytes(uchar_t state[4][4] ) {
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] = rev_subByte(state[j][i]) ; 
        }
        
    }
    
}




// Function to reverse a portion of the array
static void reverse(uchar_t* arr, int start, int end) {
    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

// Function to rotate an array by d elements to the left
static void rotateArr(uchar_t* arr, int n, int d) {
    
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
static void rotateArrR(uchar_t* arr, int n, int d) {
    
    // Handle the case where d > size of array
    d %= n;

    // Reverse the last d elements
    reverse(arr, n -d, n - 1 );

    // Reverse the remaining n-d elements
    reverse(arr, 0, n-d-1 );

    // Reverse the entire array
    reverse(arr, 0, n - 1);
}


static void shift_rows(uchar_t state[4][4]) {
    int rotation_num = 0 ; 
    for (int i = 0; i < 4; i++)
    {
        rotateArr(state[i] ,4 , rotation_num ) ;         
        rotation_num++ ; 
    }
    
}


static void inv_shift_rows(uchar_t state[4][4]) {
    int rotation_num = 0 ; 
    for (int i = 0; i < 4; i++)
    {
        rotateArrR(state[i] ,4 , rotation_num ) ;         
        rotation_num++ ; 
    }
    
}




static void mix_culumns(uchar_t state[4][4]) {
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



static void rev_mix_culumns(uchar_t state[4][4]) {
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




static void fill_state_inv(uchar_t *key  , int k, uchar_t state[4][4]) {
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            key[k] = state[j][i] ; 
            k++ ; 
        }
        
    }
    
}



static int aes_cipher_block(const uchar_t *input   , uchar_t *output ,const void *key ) {
    
    AesKey *aes_key = (AesKey *) (key) ;
    if (aes_key == NULL) {
        fprintf(stderr , "ERROR : aes_key is NULL \n") ;
        return 1 ;  
    }
    if (input == NULL || output == NULL) {
        fprintf(stderr , "ERROR : input or output is NULL \n") ;
        return 2 ;  
        
    }
    
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

    return 0 ; 
}



static int aes_cipher_inverse_block(const uchar_t *input   , uchar_t *output ,const void *key ) {
    AesKey *aes_key = (AesKey *) (key) ;
    if (aes_key == NULL) {
        fprintf(stderr , "ERROR : aes_key is NULL \n") ;
        return 1 ;  
    }
    if (input == NULL || output == NULL) {
        fprintf(stderr , "ERROR : input or output is NULL \n") ;
        return 2 ;  
        
    }
    
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

    return 0 ; 
}


static void setup_parameteres_aes(AES_TYPE type , int *Nr , int *Nk) {
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

    int result =  ecb_encrypt( input , output ,  length , AES_BLOCK_SIZE  , key , aes_cipher_block  ) ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: aes_encrypt : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}



int aes_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key)
{
    int result =  ecb_decrypt( input , output ,  length , AES_BLOCK_SIZE  , key , aes_cipher_inverse_block  ) ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: aes_decrypt : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}


int aes_encrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key)
{

    int result =  cbc_encrypt( input , output , iv , length , AES_BLOCK_SIZE  , key ,aes_cipher_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: aes_encrypt_cbc : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int aes_decrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv, int length, const void* key)
{
    int result =  cbc_decrypt( input , output , iv , length , AES_BLOCK_SIZE  , key ,aes_cipher_inverse_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: aes_decrypt_cbc : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int aes_encrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key)
{

    int result =  cfb_encrypt( input , output , iv , length , AES_BLOCK_SIZE  , key ,aes_cipher_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: aes_encrypt_cfb : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int aes_decrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv, int length, const void* key)
{
    int result =  cfb_decrypt( input , output , iv , length , AES_BLOCK_SIZE  , key ,aes_cipher_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: aes_decrypt_cfb : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}


int aes_encrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key)
{

    int result =  ofb_encrypt( input , output , iv , length , AES_BLOCK_SIZE  , key ,aes_cipher_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: aes_encrypt_ofb : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int aes_decrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv, int length, const void* key)
{
    int result =  ofb_decrypt( input , output , iv , length , AES_BLOCK_SIZE  , key ,aes_cipher_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: aes_decrypt_ofb : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int aes_encrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key)
{

    int result =  ctr_encrypt( input , output , iv , length , AES_BLOCK_SIZE  , key ,aes_cipher_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: aes_encrypt_ctr : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int aes_decrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv, int length, const void* key)
{
    int result =  ctr_decrypt( input , output , iv , length , AES_BLOCK_SIZE  , key ,aes_cipher_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: aes_decrypt_ctr : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}




#include <assert.h>
#define DEBUG_INFO_SETKEY 0

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
            #if DEBUG_INFO_SETKEY
            printf("INFO: AES128 chosen !\n");
            #endif
            aes_key->mode = AES128 ; 
            memcpy(aes_key->key ,key_str , key_len );
            aes_key->key_length = 16 ;  
        }
        else if (temp_len == 24)
        {
            #if DEBUG_INFO_SETKEY
            printf("INFO: AES196 chosen !\n");
            #endif
            
            aes_key->mode = AES192 ; 
            memcpy(aes_key->key ,key_str , key_len );
            aes_key->key_length = 24 ;  
            
        }
        else if (temp_len == 32)
        {
            #if DEBUG_INFO_SETKEY
            printf("INFO: AES256 chosen !\n");
            #endif
            aes_key->mode = AES256 ; 
            memcpy(aes_key->key ,key_str , key_len );
            aes_key->key_length = 32 ;  
            
        } else {

            fprintf(stderr , "ERROR: key length is not compatible with AES standard {16,24,32} bytes\n") ; 
            return 2 ;


        }
        
    }
    set_sbox(sbox) ; 
    
    set_rev_sbox(rev_sbox);


    uchar_t rci[10] ; 

    build_rci(rci) ;

    // PRINT_ARRAY(rci , 10 , "%x" ) ; 
        
    
    setup_parameteres_aes(aes_key->mode , &aes_key->Nr , &aes_key->Nk ) ;
    #if DEBUG_INFO_SETKEY 
    printf("INFO: mode : %d %d %d\n" , aes_key->mode , aes_key->Nr , aes_key->Nk);
    #endif
    aes_key->expanded_key_length = 4 * (aes_key->Nr+1) * 4 ; 
    #if DEBUG_INFO_SETKEY 
    printf("INFO: len : %ld \n" , aes_key->expanded_key_length);
    #endif
    
    aes_key->expanded_key = malloc(sizeof(uchar_t)*aes_key->expanded_key_length) ; 
    if (aes_key->expanded_key == NULL) {
        fprintf(stderr , "ERROR: aes_key->expanded_key is NULL\n") ; 
        return 3 ; 
    }
    assert(aes_key->expanded_key != NULL && "aes key expanded is null");
    

    key_expan(aes_key->key , aes_key->expanded_key ,aes_key->Nk ,aes_key->Nr , rci   ) ; 
    
    aes_key->type = BLOCK_CIPHER ;

    return 0 ; 
}


size_t aes_get_output_len(size_t input_len) {
    return (size_t) (input_len + (AES_BLOCK_SIZE - (input_len % AES_BLOCK_SIZE)));
}


int aes_free_key(void* key_struct)
{

    return 0  ;
}


#endif