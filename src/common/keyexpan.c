#include <stdio.h>
#include <string.h>
#include "../../include/common/rijnbox.h"
#include "../../include/common/keyexpan.h"
#include "../../include/common/galois_field_op.h"



typedef unsigned char uchar_t ; 

uchar_t sbox [16][16] ; 
uchar_t rev_sbox [16][16] ; 

uchar_t subByte(uchar_t byte) {
    uchar_t i_index = (byte & 0xf0) >> 4 ;
    uchar_t j_index = byte & 0x0f ;
    // printf("byte = %x \n" , byte) ; 
    // printf("i = %x \n" , i_index) ; 
    // printf("j = %x \n" , j_index) ; 

    return sbox[i_index][j_index] ;
}
uchar_t rev_subByte(uchar_t byte) {
    uchar_t i_index = (byte & 0xf0) >> 4 ;
    uchar_t j_index = byte & 0x0f ;
    // printf("byte = %x \n" , byte) ; 
    // printf("i = %x \n" , i_index) ; 
    // printf("j = %x \n" , j_index) ; 

    return rev_sbox[i_index][j_index] ;
}
void subWord(uchar_t array[4]) {
    for (size_t i = 0; i < 4; i++)
    {
        array[i] = subByte(array[i]) ; 
    }
    
}

void rotWord(uchar_t array[4]) {
    uchar_t temp = array[0] ; 
    for (size_t i = 1; i < 4; i++)
    {
        array[i-1] = array[i] ; 
    }
    array[3] = temp ;
    
}



void xorWord(uchar_t buffer1[4] ,uchar_t buffer2[4] ) {
    for (size_t i = 0; i < 4; i++)
        buffer1[i] ^= buffer2[i] ; 
    
}

void xorWords(uc_word *buffer1 ,uc_word *buffer2 ) {
    for (size_t i = 0; i < 4; i++)
        buffer1->word[i] ^= buffer2->word[i] ; 
    
}


void build_rci(uchar_t rci[10]){
    for (int i = 1; i < 11; i++)
    {
        uint16_t temp = 1 << (i -1)  ;
        // temp  = div_GF_16bit_edited(temp , 0x11b) ;
        // printf("temp = %x \n" , temp ) ;
        rci[i-1] =    div_euclud_GF_16bit(temp , 0x11b) ; 
        // printf("result  %d : %x \n" , i  ,rci[i] ) ;   
    }
    
}


/* 
    key exansion function
*/
void key_expan(uchar_t *key ,uchar_t *output   , int Nk , int Nr , uchar_t rci[10]) {
    int i = 0 ; 
    // printf("\t\tFIRST LOOP ABOUT TO ENTER IN\n") ;
    while (i <= Nk-1)
    {
        memcpy(output + 4*i , key + 4*i ,sizeof(uchar_t)*4 );
        // printf("\t\t\t\t\t\t \n") ; 
        for (int j = 4*i; j < (4*i)+4; j++)
        {
            // printf("\toutput[%d]=%x\n" , j , output[j]) ;
        }
        
        i++ ; 
    }
    // printf("\t\tFIRST LOOP we left IN\n") ;
    // they say now i = Nk
    
    int times_debug = 0 ; 

    // printf("\t\tSECOND LOOP ABOUT TO ENTER IN\n") ;
    while (i <= 4*Nr + 3)
    {
        // printf("\t\t\t******************************** ITERATION %d ********************************\n" , i ) ;
        // printf("i = %d \n" , i ) ;
        // uchar_t temp = output[i-1] ; 
        uc_word temp = {0} ;
        // int index = (i - 1)*4 ;
        // printf("first index : %d \n" , index) ; 
        temp.word[0] = output[(i - 1)*4 ] ; 
        temp.word[1] = output[(i - 1)*4 + 1] ; 
        temp.word[2] = output[(i - 1)*4 + 2] ; 
        temp.word[3] = output[(i - 1)*4 + 3] ; 
        // printf("\t\t temp : \n") ; 
        for (int j = 0; j < 4; j++)
        {
            // printf("\ttemp[%d]=%x\n" , j , temp.word[j]) ;
        }
        

        uc_word Rcon = {0} ; 
        Rcon.word[0] = rci[i/Nk -1] ; 
        Rcon.word[1] = 0 ;
        Rcon.word[2] = 0 ;
        Rcon.word[3] = 0 ;

        // printf("\t\t Rcon : \n") ; 
        for (int j = 0; j < 4; j++)
        {
            // printf("\trcon[%d]=%x\n" , j , Rcon.word[j]) ;
        }
        
        
        // memcpy(temp.word , output - i*4 - 4, sizeof(uchar_t)*4) ;
        // printf("\t\t temp : \n") ; 
        // for (int j = 0; j < 4; j++)
        // {
            // printf("\ttemp[%d]=%x\n" , j , temp.word[j]) ;
        // }
        
        // return ;
        // printf("\t\t*********** entering conditions *************\n") ; 

        if (i % Nk == 0)
        {
            // printf("condition 1 (i %% Nk == 0)\n") ; 
            rotWord(temp.word) ; 
            
            // printf("\t\t ********* temp after rotWord ********* : \n") ; 
            for (int j = 0; j < 4; j++)
            {
                // printf("\ttemp[%d]=%x\n" , j , temp.word[j]) ;
            }

            subWord(temp.word) ;

            // printf("\t\t ********* temp after subword *********** : \n") ; 
            for (int j = 0; j < 4; j++)
            {
                // printf("\ttemp[%d]=%x\n" , j , temp.word[j]) ;
            }
            xorWord(temp.word , Rcon.word) ; 
            // printf("\t\t ********* temp after xoe with Rcon *********** : \n") ; 
            for (int j = 0; j < 4; j++)
            {
                // printf("\ttemp[%d]=%x\n" , j , temp.word[j]) ;
            }
            
            // return ; 

        } else if (Nk > 6 && (i % Nk == 4) ) {
            subWord(temp.word) ;
            
        }
        // printf("\t********* EXITING CONSITIONS ***************\n");
        uc_word temp2 = {0} ; 
        // ?????????????????????????????????????????????????
        memcpy(temp2.word , output +  (i  - Nk)*4 , sizeof(uchar_t)*4) ;

        // printf("\t\t ********* temp2 (w[i-Nk]) *********** : \n") ; 
        for (int j = 0; j < 4; j++)
        {
            // printf("\ttemp[%d]=%x\n" , j , temp2.word[j]) ;
        }
        xorWord(temp.word , temp2.word) ; 
            // printf("\t\t ********* temp after xor with temp2 (w[i-Nk])  *********** : \n") ; 
            for (int j = 0; j < 4; j++)
            {
                // printf("\ttemp[%d]=%x\n" , j , temp.word[j]) ;
            }
        
        
        memcpy( output + i*4 , temp.word, sizeof(uchar_t)*4) ;
        // printf("\t\t ********* output after copying temp to it...  *********** : \n") ; 
        for (int j = 4*i; j < (4*i)+4; j++)
        {
            // printf("\toutput[%d]=%x\n" , j , output[j]) ;
        }
            
            
        i++ ;
        times_debug++ ; 

        // if (times_debug == -8) {
            // // printf("EXITING NOW after looping for %d !!!!!!\n" , times_debug) ; 
            // return ; 

        // }
    }
    // printf("\t\t ********* done with everything your key are ready !!! *********\n") ; 
    
}


/* 
int main() {



    initialize_aes_sbox(sbox) ; 

    PRINT_MATRIX(sbox , 16 , 16) ; 


    // printf("%x \n" , subByte(0xcf));


    // for aes1256
    int Nk = 8; 
    uchar_t original_key[32] = {0x60 , 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4   } ; 
    int original_key_length = 32 ; 
    // for aes128
    int Nr  = 14 ; 
    uchar_t expanded_key[4*4*(Nr+1)] ; 
    // word is 32bits so 4 * sizeof(uchar_t) 

    int expanded_key_length = 4 * (Nr+1) * 4; 

    uchar_t rci[10] ; 

    build_rci(rci) ;

    PRINT_ARRAY(rci , 10 , "%x" ) ; 
    

    printf("\t\tthe original key : \n") ;
    PRINT_ARRAY(original_key , original_key_length , "%x" ) ; 
    
    key_expan(original_key , expanded_key , Nk , Nr , rci) ; 



    return 444;
}
 */

/* 
TEST 1 : GOOD



    // for aes128
    int Nk = 4; 
    uchar_t original_key[16] = {0x2b  ,0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c} ; 
    int original_key_length = 16 ; 
    // for aes128
    int Nr  = 10 ; 
    uchar_t expanded_key[4*4*11] ; 
    // word is 32bits so 4 * sizeof(uchar_t) 

    int expanded_key_length = 4 * (Nr+1) * 4; 




*/


/*
TEST 2 : perfecto




    // for aes196
    int Nk = 6; 
    uchar_t original_key[24] = {0x8e , 0x73 , 0xb0, 0xf7 , 0xda, 0x0e , 0x64, 0x52 , 0xc8 , 0x10 , 0xf3  , 0x2b, 
     0x80 , 0x90, 0x79, 0xe5, 0x62, 0xf8, 0xea, 0xd2, 0x52 , 0x2c , 0x6b  ,  0x7b   } ; 
    int original_key_length = 24 ; 
    // for aes128
    int Nr  = 12 ; 
    uchar_t expanded_key[4*4*(Nr+1)] ; 
    // word is 32bits so 4 * sizeof(uchar_t) 

    int expanded_key_length = 4 * (Nr+1) * 4; 


*/