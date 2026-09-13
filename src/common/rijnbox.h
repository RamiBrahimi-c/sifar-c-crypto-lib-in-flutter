#ifndef RIJNBOX_h
#define RIJNBOX_h

typedef unsigned char uchar_t ;

#include <stdint.h>
#include "utils.h"


void initialize_aes_sbox(uchar_t sbox[256]);




uchar_t affine_transformation(uchar_t input) ;

uchar_t reverse_affine_transformation(uchar_t input);

void set_sbox(uchar_t sbox[][16]) ;
void set_rev_sbox(uchar_t rev_sbox[][16]) ;


#endif