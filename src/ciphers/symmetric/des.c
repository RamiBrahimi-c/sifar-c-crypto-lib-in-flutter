/*
    BRUH .. OT DOESNT FREAKING WORK AHJDNLKJNDKJDNLKJQNDKJNQKJDNKJQNDKJSQNDKJNSKJDNKN
    screw ts
*/

#ifndef DES_C
#define DES_C

#include "des.h"
#include "../../common/utils.h"
#include "../../common/constants.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../block_cipher_modes_operation.h"



#define DES_BLOCK_SIZE 8

// static void swapUnsignedChar(uchar_t *a ,uchar_t *b ) {
//     uchar_t temp = *a ; 
//     *a = *b ; 
//     *b = temp ;
// }



/*
    returns a bit from buffer in first position LSB
*/
static uchar_t getBitInBuffer(uchar_t *buffer ,int size , int position  ) {
    if (position < 1 || position > size * 8 )
    {
        printf("ERROR: cant look up to a bit in position : %d < 0 OR  > %d " , position , size * 8);
        exit(0) ; 

        return 0 ;
    }
    int total_size = 8 * size ; 
    
    
    int index = (position / 8)  ;
    
    
    if (position % 8 == 0)
    {
        index-- ; 
    }

    while (index < 0)
    {
        index += 1 ; 
    }
     
    uchar_t temp_byte = buffer[index] ; 
    
    int byte_pos = 8  - (position % 8) ;
    byte_pos = (byte_pos == 8) ? 0 : byte_pos ; 
    

    uchar_t k = 1 ; 
    for (size_t i = 0; i < byte_pos; i++)
    {
        k = k << 1 ; 

    }
    
    return (temp_byte & k) >> (byte_pos ) ;     
}


/*
    set bit in buffer to the value of @bit which is represented in first position LSB
*/
static uchar_t setBitInBuffer(uchar_t *buffer ,int size , int position , uchar_t bit  ) {
    if (position < 1 || position > size * 8 )
    {
        printf("ERROR: cant look up to a bit in position : %d < 0 OR  > %d " , position , size * 8);
        exit(0) ; 
        return -1 ;
    }
    int total_size = 8 * size ; 
    
    int index = (position / 8)  ;
    
    
    if (position % 8 == 0)
    {
        index-- ; 
    }
    
    
    while (index < 0)
    {
        index += 1 ; 
    }
    
    
    uchar_t temp_byte = buffer[index] ; 
    
    
    int byte_pos = 8  - (position % 8) ;
    byte_pos = (byte_pos == 8) ? 0 : byte_pos ; 
    
    
    bit &= 0b00000001 ; 
    uchar_t k = 1 ; 

    for (size_t i = 0; i < byte_pos; i++)
    {
        k <<= 1 ;


    }
    uchar_t f = 0xff - k ; 
    
    if (!bit)
    buffer[index] &= f ;
    else 
    buffer[index] |= k ;
    

    return 0 ;     
}


static void expansionTo48Bits(uchar_t *input , int input_size ,uchar_t *output , int output_size  ) {
    assert(input_size == 4 && "input size must be 4 bytes") ; 
    assert(output_size == 6 && "input size must be 6 bytes") ; 
    
    int j = 0 ;
    int index_expanded_part = 1 ;
    
    
    for (size_t i = 0; i < 8; i++)
    {
        // printf("\n\t\t=====iteration %ld ===== \n\n\n" , i+1) ; 

        uchar_t byte = 0 ; 
        // this part is about extracting 6 bits exactly from @input in position j+k
        
        for (size_t k = 0; k < 6; k++)
        {
            if ((j + k) % 32 == 0)
                byte = getBitInBuffer(input , input_size ,32 ) ;
            else                  
                byte = getBitInBuffer(input , input_size ,(j + k) % 32 ) ;
            
            setBitInBuffer(output , output_size , index_expanded_part , byte) ;
            index_expanded_part++ ;
        }


        j = (j + 4) % 32 ; 
    }

}


static void transformationBySBox(uchar_t *input , int input_size , uchar_t *output , int output_size ) {
    assert(input_size == 6  && "input size must be 6 bytes 48 bits") ; 
    assert(output_size == 4  && "input size must be 4 bytes 32 bits") ; 

    int j = 0 ; 
    uchar_t sbox_result ;
    
    
    uchar_t p[8][4][16] = {
        // S1
        {
            {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
            {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
            {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
            {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
        },
        // S2
        {
            {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
            {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
            {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
            {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
        },
        // S3
        {
            {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
            {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
            {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
            {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
        },
        // S4
        {
            {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
            {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
            {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
            {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
        },
        // S5
        {
            {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
            {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
            {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
            {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
        },
        // S6
        {
            {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
            {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
            {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
            {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
        },
        // S7
        {
            {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
            {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
            {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
            {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
        },
        // S8
        {
            {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
            {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
            {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
            {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
        }
            };    


    int n = 1 ;             
    for (size_t i = 0; i < 8; i++)
    {
        // printf("\n\t\t=====iteration %ld ===== \n\n\n" , i+1) ; 

        uchar_t byte = 0 ; 
        sbox_result = 0 ; 
        // this part is about extracting 6 bits exactly from @input in position j+k
        
        // for (size_t k = 1; k <= 6; k++)
        // {
        //     if ((j + k) % 48 == 0)
        //         byte |= getBitInBuffer(input , input_size ,48 ) << 7-k +1;
        //     else                  
        //         byte |= getBitInBuffer(input , input_size ,(j + k) % 48 ) << 7-k +1;
            
        //     // setBitInBuffer(output , output_size , index_expanded_part , byte) ;
        //     // index_expanded_part++ ;
        // }
        for (size_t k = 1; k <= 6; k++) {
            uchar_t bit = getBitInBuffer(input, input_size, j + k);
            if (bit) byte |= (1 << (8 - k)); 
        }

        // now they are stored in "byte"
        // in this format 0bXXXXXX00
        // byte = 0b10101000 ; 

        // s box transformation

        uchar_t temp = 0;
        uchar_t index_row = 0;
        uchar_t index_col = 0;

        // setting rows index
        temp = byte & 0b10000000 ;
        index_row |= temp >> 6 ; 
        temp = byte & 0b00000100 ;
        index_row |= temp >> 2 ; 
        
        
        
        // setting cols index
        temp = byte & 0b01000000 ;
        index_col |= temp >> 3 ; 

        temp = byte & 0b00100000 ;
        index_col |= temp >> 3 ; 

        temp = byte & 0b00010000 ;
        index_col |= temp >> 3 ; 

        temp = byte & 0b00001000 ;
        index_col |= temp >> 3 ; 


        // printf("byte is %.8b \t %d \n" , byte , byte);
        // printf("row is %.8b \t %d \n" , index_row , index_row);
        // printf("col is %.8b \t %d \n" , index_col , index_col);
        

        // s box result in this supposed format 
        // idk
        
        
        uchar_t bit_tobe_written = p[i][index_row][index_col]  ;
        int counter = 5; 
        for (size_t k = 5; k <=  8; k++)
        {
            uchar_t temp_bit = getBitInBuffer(&bit_tobe_written , 1 , k ) ;
            setBitInBuffer(output , output_size , n , temp_bit ) ; 
            n++ ;

        }



        j = (j + 6) % 48 ; 
    }

}





static void permutation(uchar_t *input , int input_size , uchar_t *output , int output_size) {
    assert(input_size == 4 && "input size must be 4 bytes 32 bits");
    assert(output_size == 4 && "output size must be 4 bytes 32 bits");

    int arr[] = {
        16 , 7 , 20 , 21 , 29 , 12 , 28 , 17 ,
        1 , 15 , 23 , 26 , 5 , 18 , 31 , 10 ,
        2 , 8 , 24 , 14 , 32 , 27 , 3 , 9 ,
        19 , 13 , 30 , 6 , 22 , 11 , 4 , 25 
    } ;
    int length = sizeof(arr) / sizeof(int) ; 

    for (size_t i = 0; i < length; i++)
    {
        uchar_t temp_bit = getBitInBuffer(input , input_size , arr[i] ) ;
        setBitInBuffer(output , output_size , i + 1 , temp_bit ) ;
    }
    
}


static void setFinalPermutation(uchar_t *input , int input_size , uchar_t *output , int output_size ) {
    int arr[] = {
        40 , 8 , 48 , 16 , 56 , 24 , 64 , 32
    };
    int length = sizeof(arr) / sizeof(int) ; 

    int index = 1 ;
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < length; j++) {
            uchar_t temp_bit = getBitInBuffer(input , input_size , arr[j] - i ) ;
            setBitInBuffer(output , output_size , index , temp_bit   ) ;
            index++ ;
        }
        /* code */
    }
    

    
}

static void setFirstPermutation(uchar_t *input , int input_size , uchar_t *right , int right_size , uchar_t *left , int left_size) {
    assert(input_size == 8 && "input size must be 8 bytes") ; 
    assert(left_size == 4 && "left size must be 4 bytes") ; 
    assert(right_size == 4 && "right size must be 4 bytes") ; 
    
    
    int arr[] = {
        58 , 50 , 42 , 34 , 26 , 18 , 10 , 2 , 60 , 52 , 44 , 36 , 28 , 20 , 12 , 4 ,
        62 , 54 , 46 , 38 , 30 , 22 , 14 , 6 , 64 , 56 , 48 , 40 , 32 , 24 , 16 , 8
    };
    int c_matrix[4][16] = {
        {58 , 50 , 42 , 34 , 26 , 18 , 10 , 2 , 60 , 52 , 44 , 36 , 28 , 20 , 12 , 4 },
        {62 , 54 , 46 , 38 , 30 , 22 , 14 , 6 , 64 , 56 , 48 , 40 , 32 , 24 , 16 , 8 },
        {57 , 49 , 41 , 33 , 25 , 17 , 9 , 1 , 59 , 51 , 43 , 35 , 27 , 19 , 11 , 3 },
        {61 , 53 , 45 , 37 , 29 , 21 , 13 , 5 , 63 , 55 , 47 , 39 , 31 , 23 , 15 , 7 }
    };
    int length = sizeof(arr) / sizeof(int) ; 
    int rows = 4 ; 
    int cols = 16 ; 
    // printf("length %d \n" , length) ; 
    // for (size_t i = 0; i < length; i++)
    // {
        // uchar_t temp_bit = getBitInBuffer(input , input_size , arr[i] ) ;
        // setBitInBuffer(left , left_size , i + 1 , temp_bit   ) ;
        // temp_bit = getBitInBuffer(input , input_size , arr[i] - 1 ) ;
        // setBitInBuffer(right , right_size , i + 1 , temp_bit ) ;
        // }
    int counter_left = 1 ; 
    for (size_t i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            uchar_t temp_bit = getBitInBuffer(input , input_size , c_matrix[i][j] ) ;
            setBitInBuffer(left , left_size , counter_left , temp_bit   ) ;
            counter_left ++ ; 
            
        }
        
    }
    int counter_right = 1 ; 
    for (size_t i = 2; i < 4; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            uchar_t temp_bit = getBitInBuffer(input , input_size , c_matrix[i][j] ) ;
            setBitInBuffer(right , right_size , counter_right , temp_bit   ) ;
            counter_right ++ ; 
            
        }
        
    }

}



/*
    input must be 64 bits (8 bytes) 
    output must be 56 bits (7 bytes)
*/
static void reducingKey64to56Bits(uchar_t *input , int input_size , uchar_t *output , int output_size) {
    assert(input_size == 8 && "input must be 8 bytes");
    assert(output_size == 7 && "output must be 8 bytes");
    
    uchar_t arr[8][7] = {
        {57, 49, 41, 33, 25, 17, 9},
        {1, 58, 50, 42, 34, 26, 18},
        {10, 2, 59, 51, 43, 35, 27},
        {19, 11, 3, 60, 52, 44, 36},
        {63, 55, 47, 39, 31, 23, 15},
        {7, 62, 54, 46, 38, 30, 22},
        {14, 6, 61, 53, 45, 37, 29},
        {21, 13, 5, 28, 20, 12, 4}
    };
    int rows = 8 ;
    int cols = 7 ;

    int counter = 1 ; 
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            uchar_t temp_bit =  getBitInBuffer(input , input_size , arr[i][j]);
            setBitInBuffer(output , output_size ,counter , temp_bit );
            counter++ ;
        }
        
    }
    

}


/*
    input_size must be in bytes !!!!!!!!!!!!!!!!!!!!
*/
static void leftRound(uchar_t *input , int input_size ) {
    
    uchar_t reminder = getBitInBuffer(input , input_size , 1 ) ; 
    int length = input_size * 8 ; 
    // printf("length %d \n" , length) ; 
    int k = 1 ; 
    for (size_t i = 0; i < length - 1; i++)
    {
        uchar_t temp_bit = getBitInBuffer(input , input_size , k+1 ) ; 
        setBitInBuffer(input , input_size , k , temp_bit ) ;
        // printf("iteration : %d \n" , i) ; 
        // PRINT_ARRAY(input , 2) ;  
        k++ ;
    }
    setBitInBuffer(input , input_size , k , reminder ) ; 
    
        
}

static void leftRoundNTimes(uchar_t *input , int input_size , int times) {
    for (size_t i = 0; i < times; i++)
    {
        leftRound(input , input_size) ; 
    }
    
}




static int getShiftsNumByRounds(int round_number) {
    if (round_number == 1 || round_number == 2 ||  round_number == 9 || round_number == 16)
        return 1 ; 
    
    return 2 ; 
}


static void rotate28BitsLeft(uchar_t *half) {
    // 1. Save the very first bit
    uchar_t bit1 = getBitInBuffer(half, 4, 1);
    
    // 2. Move bits 2 through 28 one space to the left
    for (int i = 1; i < 28; i++) {
        uchar_t next_bit = getBitInBuffer(half, 4, i + 1);
        setBitInBuffer(half, 4, i, next_bit);
    }
    
    // 3. Put the original bit 1 into the 28th position
    setBitInBuffer(half, 4, 28, bit1);
}



/*
    @param1 input is on 56 bits ( 7 bytes )
*/
static void genKey(uchar_t *input , int input_size , uchar_t *output , int output_size , int round_number) {
    assert(output_size == 7 ) ;    
    assert(input_size == 7 ) ;    
    

    // left shift

    uchar_t left[4] = {0}; 
    uchar_t right[4] = {0} ; 
    // how are we gonna represent em ??????????????????
    // convention : counting from left to right 
    // 56 bits :
    // XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX
    // byte 1   byte 2   byte 3   byte 4   byte 5   byte 6   byte 7
    //                              ^
    // belongs to left part <-   XXXX XXXX -> belongs to right part 
    //


    // first let's deal with left part : 
    // left is uchar_t [4] means 4 bytes <=> 32 bits ...
    
    memcpy(left , input , 4) ;
    
    // left now has first 4 bytes 
    // XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX
    //                                ^^^^
    //                                unwanted bits
    // time to get rid of last 4 bits in 4th byte by :
    // grabbing last byte & performing AND OP with 0b11110000
    uchar_t temp = left[3] & 0b11110000 ; 
    left[3] = temp ;
    
    // secondly the right part ...
    // also 4 bytes 32 bits .. 
    // memcpy(right , input + 3 , 4) ;

    // Replace that whole 'memcpy(right...)' and the shift loop with this:
    memset(right, 0, 4);
    for (int k = 1; k <= 28; k++) {
        uchar_t bit = getBitInBuffer(input, 7, k + 28); // Get bits 29-56
        setBitInBuffer(right, 4, k, bit);
    }    
    /* 
    // right now has last 4 bytes 
    // XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX
    // ^^^^
    // unwanted bits
    // ig same operation here AND OP between first byte and 0b00001111

    temp = right[0] & 0b00001111 ;
    right[0] = temp ;
    // kinda a problem here ... right does look like this now ....
    // 0000XXXX XXXXXXXX XXXXXXXX XXXXXXXX
    // we need a left shift ?
    // lemme think
    // yea lets just use get and set bits in buffer 

    int i = 1 ; 
    int j = 5 ;
    
    for (size_t k = 0; k < 28; k++)
    {
        uchar_t temp = getBitInBuffer(right , 4 , j) ; 
        setBitInBuffer(right , 4 , i , temp) ; 
        i++ ; 
        ++j ;
    }
    right[3] &= 0b11110000 ;
    
    // now right is in this format :
    // XXXXXXXX XXXXXXXX XXXXXXXX XXXX0000
    // ready for the left shift ...
 */
    int times = getShiftsNumByRounds(round_number) ; 
    assert(times ==1 || times ==2 && "gotta be 1 or 2") ;
    for(int t = 0; t < times; t++) {
        rotate28BitsLeft(left);
        rotate28BitsLeft(right);
    }
    
    // leftRoundNTimes(left , 4 , times) ; 
    // leftRoundNTimes(right , 4 , times) ;
    
    // // merge them ...
    // // left format :
    // // XXXXXXXX XXXXXXXX XXXXXXXX XXXX000X
    // uchar_t temp_bit = getBitInBuffer(left ,4 , 32 ) ; 
    // setBitInBuffer(left , 4 , 28 , temp_bit) ;
    // setBitInBuffer(left , 4 , 32 , 0) ;
    
    // temp_bit = getBitInBuffer(right ,4 , 32 ) ; 
    // setBitInBuffer(right , 4 , 28 , temp_bit) ;
    // setBitInBuffer(right , 4 , 32 , 0) ;
    
    // if (times == 2)
    // {
    //     // only 2 times are handled 
    //     // XXXXXXXX XXXXXXXX XXXXXXXX XXXX00XX       
    //     temp_bit = getBitInBuffer(left ,4 , 31 ) ; 
    //     setBitInBuffer(left , 4 , 27 , temp_bit) ;
    //     setBitInBuffer(left , 4 , 31 , 0) ;
        
    //     temp_bit = getBitInBuffer(right ,4 , 31 ) ; 
    //     setBitInBuffer(right , 4 , 27 , temp_bit) ;
    //     setBitInBuffer(right , 4 , 31 , 0) ;

    // }
    
    // we must merge them ????
    // result (7 bytes ) 56 bits :
    // XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX
    // <-          left             -><-         right                      ->
    int counter_left_read = 1 ; 
    int counter_left_write = 1 ; 
    int counter_right_read = 1 ; 
    int counter_right_write = 29 ; 

    uchar_t result_buffer[7] = {0} ; 

    for (size_t i = 1; i <= 28; i++)
    {
        // printf("here iteration %ld \n" , i);
        // printf("counter_left_read %d \n" , counter_left_read);
        // printf("counter_left_write %d \n" , counter_left_write);
        // printf("counter_right_write %d \n" , counter_right_write);
        // printf("counter_right_read %d \n" , counter_right_read);

        uchar_t temp_varl = getBitInBuffer(left , 4 , counter_left_read) ; 
        uchar_t temp_varr = getBitInBuffer(right , 4 , counter_right_read) ; 
        setBitInBuffer(output , 7 , counter_left_write ,temp_varl );
        setBitInBuffer(output , 7 , counter_right_write , temp_varr );
        counter_left_read++ ; 
        counter_right_read++ ;
        
        ++counter_left_write ; 
        ++counter_right_write ; 
    }
    
    // stop
}

static void expansionKey(uchar_t *input ,int input_size , uchar_t *output , int output_size ){
    assert(input_size == 7) ; 
    assert(output_size == 6) ; 
    // output size must be 6 bytes 48 bits

    uchar_t matrix[6][8] = {
        {14, 17, 11, 24, 1, 5, 3, 28},
        {15, 6, 21, 10, 23, 19, 12, 4},
        {26, 8, 16, 7, 27, 20, 13, 2},
        {41, 52, 31, 37, 47, 55, 30, 40},
        {51, 45, 33, 48, 44, 49, 39, 56},
        {34, 53, 46, 42, 50, 36, 29, 32}
    };

    int rows = 6 ; 
    int cols = 8 ; 
    int counter = 1 ; 


    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            uchar_t x = getBitInBuffer(input , input_size , matrix[i][j]);
            setBitInBuffer(output , output_size , counter , x );
            ++counter ; 
        }
        
    }

  

}

static void mergeLeftRight(uchar_t *output , int output_size, uchar_t *left , int left_size , uchar_t * right , int right_size) {
    assert(output_size == 8 && "input size must be 8 bytes") ; 
    assert(left_size == 4 && "left size must be 4 bytes") ; 
    assert(right_size == 4 && "right size must be 4 bytes") ; 
    

    for (size_t i = 1; i <= 32; i++)
    {
        uchar_t temp_bit = getBitInBuffer(left  , left_size , i);
        setBitInBuffer(output , output_size , i , temp_bit) ; 
        
        temp_bit = getBitInBuffer(right  , right_size , i);
        setBitInBuffer(output , output_size , i + 32 , temp_bit) ; 

        
    }
    
}


static int des_encrypt_wrapper(const uchar_t *input  , uchar_t *output , const void *key ) {
    assert(key != NULL && "key is null");
    DesKey *des_key = (DesKey *) (key) ;
    int rounds = 16 ; 

    uchar_t right[4] = {0} ;
    uchar_t left[4] = {0} ;
    uchar_t right_extended[6] = {0} ;
    // uchar_t key[8] = {0};
    // for (size_t i = 0; i < 8; i++)
    // {
    //     key[i] = 0x01 ; 
    // }
    // PRINT_ARRAY(key , 8) ; 
    uchar_t key_result[7] = {0} ;
    uchar_t key_reduced[7] = {0};
    uchar_t key_result_compressed[6] = {0} ;
    uchar_t result_sbox[4] = {0};
    uchar_t result_ppermutation[4]  = {0};
    uchar_t final_permutation[8] = {0} ;
    // uchar_t esult_ppermutation[8] ;

    setFirstPermutation(input , 8 ,right ,4 , left , 4  );
    
    reducingKey64to56Bits(des_key->key , 8 , key_reduced , 7) ;
    
    for (size_t i = 1; i <= rounds; i++)
    {
        expansionTo48Bits(right , 4 ,right_extended , 6 ) ; 
        
        // key set up
        genKey(key_reduced , 7 , key_result , 7 , i);
        memcpy(key_reduced , key_result , 7) ; 
        expansionKey(key_result , 7 , key_result_compressed , 6) ; 
        // exit(55) ; 

        mapOperation(right_extended ,key_result_compressed , right_extended , 6 , binaryXorUchar );
        transformationBySBox(right_extended , 6 , result_sbox , 4 ) ; 
        
        permutation(result_sbox , 4 ,result_ppermutation , 4  ) ; 
        mapOperation(result_ppermutation ,left , result_ppermutation , 4 , binaryXorUchar );

        

        // wrong ????????????????
        // memcpy(right , result_ppermutation , 4) ; 

        // uchar_t temp[4] ;
        // memcpy(temp , left , 4) ; 
        // memcpy(left , right  , 4) ; 
        // memcpy( right ,  temp  , 4) ;
        // replaced with : 
        // Replace that chunk with this:
        uchar_t next_right[4];
        // for (int k = 0; k < 4; k++) next_right[k] = left[k] ^ result_ppermutation[k];
        memcpy(next_right , result_ppermutation , 4) ;

        memcpy(left, right, 4);      // New Left is Old Right
        memcpy(right, next_right, 4); // New Right is the XOR result         
    
        
    
    }
    
        // after 16 rounds 
        // mergeLeftRight(final_permutation , 8 , left , 4 , right , 4 ) ; 
        mergeLeftRight(final_permutation, 8, right, 4, left, 4);
        setFinalPermutation(final_permutation , 8 , output , 8);
    
    return 0 ; 
    
}

static int des_decrypt_block(const uchar_t *input, uchar_t *output,const void *key) {
    assert(key != NULL && "key is null");
    DesKey *des_key = (DesKey *) (key) ;

    int rounds = 16;
    uchar_t right[4] = {0};
    uchar_t left[4] = {0};
    uchar_t right_extended[6] = {0};
    uchar_t key_result[7] = {0};
    uchar_t key_reduced[7] = {0};
    uchar_t key_result_compressed[6] = {0};
    uchar_t result_sbox[4] = {0};
    uchar_t result_ppermutation[4] = {0};
    uchar_t final_permutation[8] = {0};
    
    // 1. Initial permutation (SAME as encryption!)
    setFirstPermutation(input, 8, right, 4, left, 4);
    
    // 2. Generate all round keys first (store them)
    uchar_t all_round_keys[16][7] = {0};
    reducingKey64to56Bits(des_key->key, 8, key_reduced, 7);
    
    for (int i = 1; i <= rounds; i++) {
        genKey(key_reduced, 7, all_round_keys[i-1], 7, i);
        memcpy(key_reduced, all_round_keys[i-1], 7);
    }
    
    // 3. Decryption rounds (reverse order!)
    for (int i = rounds; i >= 1; i--) {
        // Same F function as encryption
        expansionTo48Bits(right, 4, right_extended, 6);
        expansionKey(all_round_keys[i-1], 7, key_result_compressed, 6);
        mapOperation(right_extended, key_result_compressed, right_extended, 6, binaryXorUchar);
        transformationBySBox(right_extended, 6, result_sbox, 4);
        permutation(result_sbox, 4, result_ppermutation, 4);
        mapOperation(result_ppermutation, left, result_ppermutation, 4, binaryXorUchar);
        
        // Same swap as encryption!
        uchar_t next_right[4];
        memcpy(next_right, result_ppermutation, 4);
        memcpy(left, right, 4);
        memcpy(right, next_right, 4);
    }
    
    // 4. Final permutation (SAME as encryption!)
    mergeLeftRight(final_permutation, 8, right, 4, left, 4);
    setFinalPermutation(final_permutation, 8, output, 8);

    return 0 ; 
}




int des_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key) {


    int result =  ecb_encrypt( input , output ,  length , DES_BLOCK_SIZE  , key , des_encrypt_wrapper  ) ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: des_encrypt : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}


int des_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key ){

    int result =  ecb_decrypt( input , output ,  length , DES_BLOCK_SIZE  , key , des_decrypt_block  ) ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: des_decrypt : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}


int des_encrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key)
{

    int result =  cbc_encrypt( input , output , iv , length , DES_BLOCK_SIZE  , key ,des_encrypt_wrapper ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: des_encrypt_cbc : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int des_decrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv, int length, const void* key)
{
    int result =  cbc_decrypt( input , output , iv , length , DES_BLOCK_SIZE  , key ,des_decrypt_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: des_decrypt_cbc : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int des_encrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key)
{

    int result =  cfb_encrypt( input , output , iv , length , DES_BLOCK_SIZE  , key ,des_encrypt_wrapper ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: des_encrypt_cfb : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int des_decrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv, int length, const void* key)
{
    int result =  cfb_decrypt( input , output , iv , length , DES_BLOCK_SIZE  , key ,des_encrypt_wrapper ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: des_decrypt_cfb : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}


int des_encrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key)
{

    int result =  ofb_encrypt( input , output , iv , length , DES_BLOCK_SIZE  , key ,des_encrypt_wrapper ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: des_encrypt_ofb : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int des_decrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv, int length, const void* key)
{
    int result =  ofb_decrypt( input , output , iv , length , DES_BLOCK_SIZE  , key ,des_encrypt_wrapper ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: des_decrypt_ofb : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int des_encrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key)
{

    int result =  ctr_encrypt( input , output , iv , length , DES_BLOCK_SIZE  , key ,des_encrypt_wrapper ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: des_encrypt_ctr : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int des_decrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv, int length, const void* key)
{
    int result =  ctr_decrypt( input , output , iv , length , DES_BLOCK_SIZE  , key ,des_encrypt_wrapper ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: des_decrypt_ctr : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}




// TODO : this part needs to be checked again
int des_set_key(void* key_struct, const uchar_t* key_str , size_t key_len) {
    DesKey *des_key = (DesKey *) key_struct ;
    if (des_key==NULL) {
        fprintf(stderr , "ERROR: des_key is NULL\n") ; 
        return 1 ; 
    }

    if (key_str==NULL) {
        fprintf(stderr , "ERROR: key_str is NULL\n") ; 
        return 2 ; 
    }
    if (key_len != 8) {
        fprintf(stderr , "ERROR: key_len is %d (it must be 8 bytes !!!)\n" , key_len) ; 
        return 3 ; 

    }
    memcpy(des_key->key , key_str ,8 ) ; 
    // printf("key set to : \n") ; 
    des_key->type = BLOCK_CIPHER ;
    // PRINT_ARRAY(des_key->key , 8) ; 
    return 0 ; 
}


int des_free_key(void* key_struct) {

    free(key_struct) ; 
    return 0 ; 
}

Cipher* get_des_cipher(void);



#endif