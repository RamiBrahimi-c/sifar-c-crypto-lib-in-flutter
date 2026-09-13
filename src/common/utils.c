#ifndef UTILS_C
#define UTILS_C

#include "../../include/common/utils.h"
#include "../../include/common/constants.h"

// default : the total ASCII characters128
// int ALPHABET_LENGTH  = 128 ; 

#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to calculate the GCD using the Euclidean algorithm (iterative)
uint64_t gcd(uint64_t a, uint64_t b) {
    uint64_t temp;
    // Use absolute values to handle negative inputs correctly
    // a = (a < 0) ? -a : a;
    // b = (b < 0) ? -b : b;

    while (b != 0) {
        temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}

_Bool isCoprime(uint64_t a , uint64_t b) {
    return gcd(a , b) == 1 ; 
}


uint16_t modInverse(uint16_t a, uint16_t m) {
    if (m == 1) return 0;
    if (a == 0) return 0;
    
    // Make sure a is within modulus
    a = a % m;
    
    int16_t x0 = 0, x1 = 1;
    int16_t q, t;
    int16_t m0 = m;
    int16_t a_s = a;
    int16_t m_s = m;
    
    while (a_s > 1) {
        if (m_s == 0) return 0; // Prevent division by zero
        
        q = a_s / m_s;
        t = m_s;
        m_s = a_s % m_s;
        a_s = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    
    if (x1 < 0) {
        x1 += m0;
    }
    
    return (uint16_t)x1;
}




void multiplyMatrix(uchar_t matrix1[][MATRIX_MAX_DEGREE] , int n , int m , uchar_t matrix2[][MATRIX_MAX_DEGREE] , int a , int b , uchar_t result[][MATRIX_MAX_DEGREE]){
    assert(m==a && "cols of second matrix == rows od first matrix"
    && "WARNING: max size of columns is fixed and defined here");

        // printf("multiply matriiix\n");
    // if we work with uchar_t then we need better typre cuz multiplication 
    // grows so fast and we need `mod` to ensure a correct result
    int temp = 0 ;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < b; j++) {
            
            temp = 0 ;

            for (int k = 0; k < a; k++) {
                temp += (matrix1[i][k] * matrix2[k][j]) ;
            }
            // very important 
            // printf(" %d " , temp) ; 
            result[i][j] = (temp % ALPHABET_LENGTH) ;
        }
        // printf("\n");

    }
}



void setRandomInversibleMatrix(uchar_t  matrix[][MATRIX_MAX_DEGREE],int lines , int cols) {


    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // ?????????????????????
            matrix[i][j] = rand() % ALPHABET_LENGTH ;
        }
        
    }


    
}


void copyData(const uchar_t* input , int length , int start_index, int n , uchar_t matrix[][MATRIX_MAX_DEGREE]) {
    
    size_t i = start_index;
    int j = 0;
    while ( j < n)
    {
        // just respecting the order which is a convention
        matrix[j][0] = input[i] ;
        i = (i + 1) % length ; 
        j++ ; 
    }
    
}

void copyDataInv( uchar_t matrix[][MATRIX_MAX_DEGREE] , int lines  ,  uchar_t* result , int length , int start_index, int n ) {
    // for (size_t i = 0; i < lines; i++)
    // {
    //     // just respecting the order which is a convention
    //     result[i] =  matrix[i][0] ;
    // }

    // yea man idk what are these 2 serving .. lets just make GCC quite :D 
    lines = lines; 
    length = length;
    size_t i = start_index;
    int j = 0;
    // while ( j < n && i < length)
    while ( j < n )
    {
        // just respecting the order which is a convention
        // matrix[j][0] = input[i] ;
        result[i] =  matrix[j][0] ;
        // i = (i + 1) % length ; 
        i++ ;
        j++ ; 
    }
        
}



// Determinant modulo using Gaussian elimination
int64_t determinant_gaussian_mod(int n, uchar_t matrix[][MATRIX_MAX_DEGREE], uint64_t mod) {
    // Create a copy with proper type - use uint64_t for calculations to prevent overflow
    uint64_t temp[MATRIX_MAX_DEGREE][MATRIX_MAX_DEGREE];
    
    // Copy uchar_t to uint64_t properly
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp[i][j] = matrix[i][j] % mod; // Apply mod immediately
        }
    }
    
    int sign = 1;
    int64_t det = 1;
    
    for (int i = 0; i < n; i++) {
        // Find pivot with invertible element
        int pivot = -1;
        for (int j = i; j < n; j++) {
            if (temp[j][i] != 0) {
                uint64_t inv = modInverse(temp[j][i], mod);
                if (inv != 0) {
                    pivot = j;
                    break;
                }
            }
        }
        
        if (pivot == -1) {
            return 0; // Matrix is singular modulo mod
        }
        
        // Swap rows if needed
        if (pivot != i) {
            for (int j = 0; j < n; j++) {
                uint64_t tmp = temp[i][j];
                temp[i][j] = temp[pivot][j];
                temp[pivot][j] = tmp;
            }
            sign = -sign;
        }
        
        // Store the pivot value before normalizing (for determinant calculation)
        uint64_t pivot_value = temp[i][i];
        
        // Make pivot 1 (for modular arithmetic)
        uint64_t inv = modInverse(temp[i][i], mod);
        if (inv == 0) {
            return 0; // Pivot not invertible
        }
        
        for (int j = 0; j < n; j++) {
            temp[i][j] = (temp[i][j] * inv) % mod;
        }
        
        // Eliminate below
        for (int j = i + 1; j < n; j++) {
            if (temp[j][i] != 0) {
                uint64_t factor = temp[j][i];
                for (int k = i; k < n; k++) {
                    // Use int64_t temporarily to handle negative intermediate values
                    int64_t val = (int64_t)temp[j][k] - (int64_t)factor * (int64_t)temp[i][k];
                    val %= (int64_t)mod;
                    if (val < 0) val += mod;
                    temp[j][k] = (uint64_t)val;
                }
            }
        }
        
        // Update determinant using the original pivot value (before normalization)
        det = (det * (int64_t)pivot_value) % (int64_t)mod;
    }
    
    // Apply sign to determinant
    det = (det * sign) % (int64_t)mod;
    if (det < 0) det += mod;
    
    return det;
}

_Bool isMatrixInversible_mod(uchar_t matrix[][MATRIX_MAX_DEGREE], int lines, int cols, uint64_t mod) {
    if (lines != cols || lines == 0) {
        return 0;
    }
    
    int64_t det = determinant_gaussian_mod(lines, matrix, mod);
    return det != 0 && modInverse( (uint16_t) (det % ALPHABET_LENGTH) , ALPHABET_LENGTH);
}

// void getCofactor(int n, uint64_t matrix[][MATRIX_MAX_DEGREE], 
//                  uint64_t temp[][MATRIX_MAX_DEGREE-1], int skipRow, int skipCol) {
//     int i = 0, j = 0;
//     for (int row = 0; row < n; row++) {
//         for (int col = 0; col < n; col++) {
//             if (row != skipRow && col != skipCol) {
//                 temp[i][j++] = matrix[row][col];
//                 if (j == n - 1) {
//                     j = 0;
//                     i++;
//                 }
//             }
//         }
//     }
// }

void getCofactor(int n, uchar_t matrix[][MATRIX_MAX_DEGREE], 
                 uchar_t temp[][MATRIX_MAX_DEGREE], int skipRow, int skipCol) {
    int i = 0, j = 0;
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (row != skipRow && col != skipCol) {
                temp[i][j++] = matrix[row][col];
                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}


int64_t determinant_mod(int n, uchar_t matrix[][MATRIX_MAX_DEGREE], uint64_t mod) {
    // Base cases
    if (n == 1) {
        return (int64_t)(matrix[0][0] % mod);
    }
    
    if (n == 2) {
        // Use larger types to prevent overflow
        int64_t a = matrix[0][0];
        int64_t b = matrix[0][1];
        int64_t c = matrix[1][0];
        int64_t d = matrix[1][1];
        
        int64_t det = (a * d - b * c);
        printf("det : %ld\n", det);
        
        det %= (int64_t)mod;
        if (det < 0) det += mod;
        return det;
    }
    
    // Recursive case - temp should match matrix type
    uchar_t temp[MATRIX_MAX_DEGREE][MATRIX_MAX_DEGREE];
    int64_t det = 0;
    int sign = 1;
    
    for (int f = 0; f < n; f++) {
        getCofactor(n, matrix, temp, 0, f);
        
        // Use int64_t for multiplication to prevent overflow
        int64_t cofactor_det = determinant_mod(n - 1, temp, mod);
        int64_t term = (int64_t)sign * (int64_t)matrix[0][f] * cofactor_det;
        
        det = (det + term) % (int64_t)mod;
        
        sign = -sign;
    }
    
    if (det < 0) det += mod;
    return det;
}



// Calculate cofactor matrix (matrix of minors with signs)
void cofactorMatrix(int n, uchar_t matrix[][MATRIX_MAX_DEGREE], 
                    uchar_t cofactor[][MATRIX_MAX_DEGREE], uint64_t mod) {
    if (n == 1) {
        cofactor[0][0] = 1; // Cofactor of 1x1 matrix is 1
        return;
    }
    
    uchar_t temp[MATRIX_MAX_DEGREE][MATRIX_MAX_DEGREE];
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // Get cofactor (submatrix excluding row i and col j)
            getCofactor(n, matrix, temp, i, j);
            
            // Calculate determinant of submatrix
            int64_t det = determinant_mod(n - 1 ,  temp, mod);
            
            // Apply sign: (-1)^(i+j)
            if ((i + j) % 2 == 1) {
                det = (-det) % (int64_t)mod;
                if (det < 0) det += mod;
            }
            
            cofactor[i][j] = (uchar_t)(det % mod);
        }
    }
}



// Main function to calculate matrix inverse modulo mod
void matrixInverseMod(uchar_t matrix[][MATRIX_MAX_DEGREE], 
                      uchar_t result[][MATRIX_MAX_DEGREE], 
                      int n, uint64_t mod) {
    
    // Step 1: Calculate determinant
    int64_t det = determinant_mod(n, matrix, mod);
    
    // Step 2: Calculate determinant's modular inverse
    uint64_t det_inv = modInverse((uint64_t)det, mod);
    
    // Step 3: Calculate cofactor matrix
    uchar_t cofactor[MATRIX_MAX_DEGREE][MATRIX_MAX_DEGREE];
    cofactorMatrix(n, matrix, cofactor, mod);
    
    // Step 4: Transpose cofactor matrix to get adjugate
    uchar_t adjugate[MATRIX_MAX_DEGREE][MATRIX_MAX_DEGREE];
    transposeMatrix(n, cofactor, adjugate);
    
    // Step 5: Multiply adjugate by det^(-1) modulo mod
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int64_t val = ((int64_t)adjugate[i][j] * (int64_t)det_inv) % (int64_t)mod;
            if (val < 0) val += mod;
            result[i][j] = (uchar_t)val;
        }
    }
}

void transposeMatrix(int n, uchar_t matrix[][MATRIX_MAX_DEGREE], 
                     uchar_t result[][MATRIX_MAX_DEGREE]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

void string_to_lowercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = (char)tolower(str[i]);
    }
}

void initilizeAlphabets(uchar_t *array , char choice) {
    choice = tolower(choice) ; 
    if (choice == 'l')
    {
        int i = 'a' ;
        int k = 0 ;  
        while (i <= 'z')
        {
            array[k] = i ;
            i++ ; 
        }
        
        
    } else {
        int i = 'A' ;
        int k = 0 ;  
        while (i <= 'Z')
        {
            array[k] = i ;
            i++ ; 
        }
    }
    
}



// Comparison function for qsort()
int compareUChars(const void* a, const void* b) {
    return (*(uchar_t*)a - *(uchar_t*)b);
}

_Bool areElementsUniqueSorted(const uchar_t arr[], int n) {
    char temp_array[ARRAY_MAX_SIZE] ; 

    strcpy(temp_array ,(const char*) arr) ; 
    // Sort the array
    qsort(temp_array, n, sizeof(uchar_t), compareUChars);

    // Check adjacent elements
    for (int i = 0; i < n - 1; i++) {
        if (temp_array[i] == temp_array[i + 1]) {
            return 0; // Found adjacent equal elements (duplicates)
        }
    }
    return 1; // No duplicates found
}


int findIndexMap(const uchar_t *array , uchar_t value ) {
    for (int i = 0; array[i] != '\0'; i++)
        if (array[i]==value)
            return i ; 
    
    return -1 ; 
}



void swapUnsignedChar(uchar_t *a ,uchar_t *b ) {
    uchar_t temp = *a ; 
    *a = *b ; 
    *b = temp ;
}



#endif