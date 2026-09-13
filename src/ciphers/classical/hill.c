#ifndef HILL_C
#define HILL_C


#include "../../../include/ciphers/classical/hill.h"
#include "../../../include/common/utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



void hill_encrypt(const uchar_t* input, uchar_t* output , int length, const void* key) {
    assert(key != NULL && "key is null");
    HillKey *hill_key = (HillKey *) (key) ;
    
    int step = hill_key->n ; 

    // just temporary stuff to hold results in apropriate format
    uchar_t matrix2[MATRIX_MAX_DEGREE][MATRIX_MAX_DEGREE] ;
    int l2 = step , c2 = 1 ;
    // int l2 = strlen((char *) input) , c2 = 1 ;
    
    uchar_t result[MATRIX_MAX_DEGREE][MATRIX_MAX_DEGREE] ;
    
    // int length = strlen((char *) input) ; 
    
    printf("alpha num : %d \n" , ALPHABET_LENGTH) ; 
    printf("length : %d\n" , length ) ; 
    

    size_t loop_times = length/step ;
    // int loop_times = strlen((char *) input)/step ;
    if (strlen((char *) input) %  step != 0 )
    {
        loop_times++ ; 
    }
    
    
    size_t i = 0;
    size_t j = 0;
    while (i < loop_times )
    {
        // copy n bytes from input to matrix 1
        copyData(input ,length, j  , step, matrix2) ; 
        // PRINT_MATRIX(matrix2 , l2 , c2);
        
        
        multiplyMatrix(hill_key->matrix , hill_key->n , hill_key->n ,matrix2 , l2 , c2  , result) ;
        // printf("it became : \n");
        copyDataInv( result  ,l2 , output  , length, j , hill_key->n  ) ; 
        // PRINT_MATRIX(result , hill_key->n , c2);
        
        j += step ;
        i +=1 ;
        // multiplyMatrix(matrix1 , l1 , c1 , matrix2 , l2 , c2 , result) ;
        // output[i] = ((hill_key->a * input[i]) + hill_key->b) % ALPHABET_LENGTH ;
        
    }


    
}


void hill_decrypt(const uchar_t* input, uchar_t* output, const void* key) {
    assert(key != NULL && "key is null");
    HillKey *hill_key = (HillKey *) (key) ;
    
    int step = hill_key->n ; 
    // just temporary stuff to hold results in apropriate format
    uchar_t matrix1[MATRIX_MAX_DEGREE][MATRIX_MAX_DEGREE] ;
    int l1 = hill_key->n , c1 = hill_key->n ;

    matrixInverseMod(hill_key->matrix , matrix1 , hill_key->n , ALPHABET_LENGTH) ;
    PRINT_MATRIX(matrix1 , hill_key->n , hill_key->n , "%d") ;

    


    uchar_t matrix2[MATRIX_MAX_DEGREE][MATRIX_MAX_DEGREE] ;
    int l2 = step , c2 = 1 ;
    // int l2 = strlen((char *) input) , c2 = 1 ;
    
    uchar_t result[MATRIX_MAX_DEGREE][MATRIX_MAX_DEGREE] ;
    
    int length = strlen((char *) input) ; 
    
    printf("alpha num : %d \n" , ALPHABET_LENGTH) ; 
    printf("length : %d\n" , length ) ; 
    

    size_t loop_times = strlen((char *) input)/step ;
    if (strlen((char *) input) %  step != 0 )
    {
        loop_times++ ; 
    }
    
    
    size_t i = 0;
    size_t j = 0;
    while (i < loop_times )
    {
        // copy n bytes from input to matrix 1
        copyData(input ,length, j  , step, matrix2) ; 
        // PRINT_MATRIX(matrix2 , l2 , c2);
        
        
        multiplyMatrix(matrix1 , l1 , c1 ,matrix2 , l2 , c2  , result) ;
        // printf("it became : \n");
        copyDataInv( result  ,l2 , output  , length, j , hill_key->n  ) ; 
        // PRINT_MATRIX(result , hill_key->n , c2);
        
        j += step ;
        i +=1 ;
        // multiplyMatrix(matrix1 , l1 , c1 , matrix2 , l2 , c2 , result) ;
        // output[i] = ((hill_key->a * input[i]) + hill_key->b) % ALPHABET_LENGTH ;
        
    }

    // printf("in decrypt function \n");

    // PRINT_ARRAY(output , strlen(output));
    output[strlen((const char *) input)-1] = '\0' ; 
    // PRINT_ARRAY(output , strlen(output));
    // printf("in decrypt function \n");
}





void hill_set_key(void* key_struct, const uchar_t* key_str , size_t key_len) {
    HillKey *hill_key = (HillKey*) key_struct ;

    uint64_t matrix_degree = atoi(key_str) ; 
    

    assert(matrix_degree>0 
        && matrix_degree < MATRIX_MAX_DEGREE
        && "respect range of matrix degree ");
    
    hill_key->n = matrix_degree ; 

    setRandomInversibleMatrix(hill_key->matrix , hill_key->n , hill_key->n);
    // hill_key->matrix[0][0] = 9 ; 
    // hill_key->matrix[0][1] = 4 ; 
    // hill_key->matrix[1][0] = 5 ; 
    // hill_key->matrix[1][1] = 7 ; 


    // for (size_t i = 0; i < hill_key->n; i++)
    // {
    //     for (size_t j = 0; j < hill_key->n; j++)
    //     {
    //         printf(" %d " , hill_key->matrix[i][j]);
    //     }printf("\n");
        
    // }
    
    // printf("determinant : %lu \n",
    //     determinant_gaussian_mod(hill_key->n , hill_key->matrix , ALPHABET_LENGTH));
    
    //     printf("determinant method 2 : %lu \n",
    //         determinant_mod(hill_key->n , hill_key->matrix , ALPHABET_LENGTH));
    
    // printf("is matrix inversible : %d ??? \n" , 
    //     isMatrixInversible_mod(hill_key->matrix , hill_key->n , hill_key->n , ALPHABET_LENGTH));
    
    
        // exit(0);
        
    while (!isMatrixInversible_mod(hill_key->matrix , hill_key->n , hill_key->n , ALPHABET_LENGTH)
            ){
        setRandomInversibleMatrix(hill_key->matrix , hill_key->n , hill_key->n) ; 
    }
    uint64_t det = determinant_gaussian_mod(hill_key->n , hill_key->matrix , ALPHABET_LENGTH) ;
    printf("INFO : matrix inversible with determinent %lu and inverse %u : \n" , det  
            , modInverse(det , ALPHABET_LENGTH));

    // for (size_t i = 0; i < hill_key->n; i++)
    // {
        // for (size_t j = 0; j < hill_key->n; j++)
        // {
            // printf(" %d " , hill_key->matrix[i][j]);
        // }printf("\n");
        
    // }
    

    
}


void hill_free_key(void* key_struct) ;

Cipher* get_hill_cipher(void);




#endif
