#ifndef UTULS_H
#define UTULS_H


#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"



// Basic matrix print macro
#define PRINT_MATRIX(matrix, rows, cols , format) \
    do { \
        printf("Matrix (%d x %d):\n", rows, cols); \
        for (int _i = 0; _i < (rows); _i++) { \
            for (int _j = 0; _j < (cols); _j++) { \
                printf(format " ", (matrix)[_i][_j]); \
            } \
            printf("\n"); \
        } \
        printf("\n"); \
    } while (0)
// no length line printed
#define PRINT_ARRAY_NAI(array , length , format) \
    do { \
        for (int _i = 0; _i < (length); _i++) { \
                printf(format " ", (array)[_i]); \
        } \
        printf("\n"); \
    } while (0)
#define PRINT_ARRAY(array , length , format) \
    do { \
        printf("Array (%d):\n", length); \
        for (int _i = 0; _i < (length); _i++) { \
                printf(format " ", (array)[_i]); \
        } \
        printf("\n"); \
    } while (0)
#define PRINT_ARRAY_NOSPCLEN(array , length , format) \
    do { \
        for (int _i = 0; _i < (length); _i++) { \
                printf(format , (array)[_i]); \
        } \
        printf("\n"); \
    } while (0)
#define PRINT_ARRAY_NOSPC(array , length , format) \
    do { \
        printf("Array (%d):\n", length); \
        for (int _i = 0; _i < (length); _i++) { \
                printf(format , (array)[_i]); \
        } \
        printf("\n"); \
    } while (0)




// Function to calculate the GCD using the Euclidean algorithm (iterative)
uint64_t gcd(uint64_t a, uint64_t b) ;

_Bool isCoprime(uint64_t a , uint64_t b) ;

uint16_t modInverse(uint16_t a  ,uint16_t m ) ;


void multiplyMatrix(uchar_t matrix1[][MATRIX_MAX_DEGREE] , int n , int m , uchar_t matrix2[][MATRIX_MAX_DEGREE] , int a , int b , uchar_t result[][MATRIX_MAX_DEGREE]);


void setRandomInversibleMatrix(uchar_t  matrix[][MATRIX_MAX_DEGREE],int lines , int cols) ;

void copyData(const uchar_t* input , int length, int start_index, int n , uchar_t matrix[][MATRIX_MAX_DEGREE]) ;

// void copyDataInv( uchar_t matrix[][MATRIX_MAX_DEGREE] , int lines  ,  uchar_t* result ) ;
void copyDataInv( uchar_t matrix[][MATRIX_MAX_DEGREE] , int lines  ,  uchar_t* result , int length , int start_index, int n ) ;


// Determinant modulo using Gaussian elimination
int64_t determinant_gaussian_mod(int n, uchar_t matrix[][MATRIX_MAX_DEGREE], uint64_t mod) ;

_Bool isMatrixInversible_mod(uchar_t matrix[][MATRIX_MAX_DEGREE], int lines, int cols, uint64_t mod) ;


int64_t determinant_mod(int n, uchar_t matrix[][MATRIX_MAX_DEGREE], uint64_t mod) ;

void getCofactor(int n, uchar_t matrix[][MATRIX_MAX_DEGREE],uchar_t temp[][MATRIX_MAX_DEGREE], int skipRow, int skipCol) ;

int64_t determinant_mod(int n, uchar_t matrix[][MATRIX_MAX_DEGREE], uint64_t mod) ;

void cofactorMatrix(int n, uchar_t matrix[][MATRIX_MAX_DEGREE], 
                    uchar_t cofactor[][MATRIX_MAX_DEGREE], uint64_t mod) ;

void matrixInverseMod(uchar_t matrix[][MATRIX_MAX_DEGREE], 
                      uchar_t result[][MATRIX_MAX_DEGREE], 
                      int n, uint64_t mod) ;                    
void transposeMatrix(int n, uchar_t matrix[][MATRIX_MAX_DEGREE], 
                     uchar_t result[][MATRIX_MAX_DEGREE]) ;

void string_to_lowercase(char *str) ;

void initilizeAlphabets(uchar_t *array , char choice) ;

// Comparison function for qsort()
int compareUChars(const void* a, const void* b) ;


_Bool areElementsUniqueSorted(const uchar_t arr[], int n) ;


int findIndexMap(const uchar_t *array , uchar_t value ) ;

void swapUnsignedChar(uchar_t *a ,uchar_t *b ) ;


uchar_t binaryXorUchar(void *a , void *b) ;

void mapOperation(uchar_t *arr1 ,uchar_t *arr2  , uchar_t *result, int length  ,uchar_t opp(void * , void *) ) ;


#endif