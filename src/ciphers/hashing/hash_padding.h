#ifndef HASH_PADDING_H
#define HASH_PADDING_H

// #include "hash.h"

uchar_t* md4_padding(uchar_t M[] , uint64_t b , uint64_t *output_length ) ;
uchar_t* md5_padding(uchar_t M[] , uint64_t b , uint64_t *output_length ) ;




#endif