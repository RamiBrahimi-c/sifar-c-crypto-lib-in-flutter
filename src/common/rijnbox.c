#include <stdint.h>
#include <stdlib.h>





#include <stdio.h>
#include "../../include/common/rijnbox.h"
#include "../../include/common/galois_field_op.h"


#define ROTL8(x,shift) ((uchar_t) ((x) << (shift)) | ((x) >> (8 - (shift))))



/* 
    very interesting approach was found in Wiki
*/
void initialize_aes_sbox(uchar_t sbox[256]) {
	uchar_t p = 1, q = 1;
	
	/* loop invariant: p * q == 1 in the Galois field */
	do {
		/* multiply p by 3 */
		p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);

		/* divide q by 3 (equals multiplication by 0xf6) */
		q ^= q << 1;
		q ^= q << 2;
		q ^= q << 4;
		q ^= q & 0x80 ? 0x09 : 0;

		/* compute the affine transformation */
		uchar_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);

		sbox[p] = xformed ^ 0x63;
	} while (p != 1);

	/* 0 is a special case since it has no inverse */
	sbox[0] = 0x63;
}






uchar_t affine_transformation(uchar_t input) {
    return input ^ ROTL8(input , 1) ^ ROTL8(input , 2) ^ ROTL8(input , 3) ^ ROTL8(input , 4) ^ 0x63; 
}


uchar_t reverse_affine_transformation(uchar_t input) {
    return ROTL8(input , 1) ^ ROTL8(input , 3) ^ ROTL8(input , 6) ^ 0x05; 
}



void set_sbox(uchar_t sbox[][16]) {
    for (size_t i = 0; i < 16; i++)
        {
            for (size_t j = 0; j < 16; j++)
            {
                uchar_t input = (i << 4) | (j) ;
                sbox[i][j] = affine_transformation(modInverse_polynoms(input ,0x11b )); 
            } 
            
        }
}

void set_rev_sbox(uchar_t rev_sbox[][16]) {

    for (size_t i = 0; i < 16; i++)
    {
        for (size_t j = 0; j < 16; j++)
        {
            uchar_t input = (i << 4) | (j) ;
            rev_sbox[i][j] = modInverse_polynoms(reverse_affine_transformation(input) ,0x11b ); 
        } 
        
    }
}


/* 
int main() {

    printf("wth is rijindel box !!!!\n") ; 

    uchar_t sbox[16][16] ; 
    uchar_t rev_sbox[16][16] ; 
    uchar_t sbox2[256] ; 


    // initialize_aes_sbox(sbox2) ; 

    uchar_t result = modInverse_polynoms(0x1d , 0x11b) ;
    printf("inverse : %x\n" , result) ; 
    // set_sbox(sbox) ; 
    // PRINT_MATRIX(sbox , 16 , 16) ; 
    
    // set_rev_sbox(rev_sbox) ; 
    // PRINT_MATRIX(rev_sbox , 16 , 16) ; 



    return 0;
} */