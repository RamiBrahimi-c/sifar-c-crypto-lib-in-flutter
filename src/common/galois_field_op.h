#ifndef GALOIS_FIELD_OP_H
#define GALOIS_FIELD_OP_H


#include <stdio.h>
#include <stdint.h>


typedef unsigned char uchar_t ; 

uint16_t keepMSB_16bit(uint16_t a);

uchar_t keepMSB(uchar_t a) ;

uint16_t div_GF_16bit(uint16_t a , uint16_t b);


uchar_t mod_GF(uint16_t result) ;

uint16_t mul_GF_16bit(uint16_t a , uint16_t b) ;


uchar_t add_GF_16bit(uchar_t a , uchar_t b) ;

uint16_t div_euclud_GF_16bit(uint16_t a , uint16_t b );



uint16_t div_GF_16bit_edited(uint16_t a , uint16_t b) ;

// m ig is always x^8 + x^4 + x^3 + x + 1
// aka 1 0001 1011 = 0x11b
uint16_t modInverse_polynoms(uint16_t n, uint16_t m);



#endif