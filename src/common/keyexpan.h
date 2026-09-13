#ifndef KEYEXPAN_h
#define KEYEXPAN_h


typedef unsigned char uchar_t ; 

extern uchar_t sbox [16][16] ; 
extern uchar_t rev_sbox [16][16] ; 

uchar_t subByte(uchar_t byte) ;
uchar_t rev_subByte(uchar_t byte) ;

void subWord(uchar_t array[4]) ;

void rotWord(uchar_t array[4]) ;

typedef struct 
{
    uchar_t word[4] ;  
}uc_word;


void xorWord(uchar_t buffer1[4] ,uchar_t buffer2[4] ) ;

void xorWords(uc_word *buffer1 ,uc_word *buffer2 ) ;


void build_rci(uchar_t rci[10]);



void key_expan(uchar_t *key ,uchar_t *output   , int Nk , int Nr , uchar_t rci[10]) ;


#endif
