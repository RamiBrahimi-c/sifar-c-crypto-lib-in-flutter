#include "hash.h"
#include <math.h>

// TODO : a code REFACTOR for the similar functions that we need both in MD4 and MD5 
// TODO : and a code documentation of my philosophy here i did it the ugly way cuz i could have just grabbed the bytes and transformed
// them into a uint32_t and handle the endianess but i just went raw so wtv you need to explain anyway ..



// f(X,Y,Z)  =  XY v not(X)Z
static void F(uchar_t x[4] ,uchar_t y[4] ,uchar_t z[4] ,uchar_t result[4]  ) {

    // X and Y
    uchar_t temp1[4] ;
    for (int i = 0; i < 4; i++)
    {
        temp1[i] = x[i] & y[i] ;
    }

    // not(X) and Z
    uchar_t temp2[4] ;
    for (int i = 0; i < 4; i++)
    {
        temp2[i] = (~x[i]) & z[i] ;
    }

    // X and Y  OR  not(x) and Z
    for (int i = 0; i < 4; i++)
    {
        result[i] = temp1[i] | temp2[i] ;
    }


}


// g(X,Y,Z)  =  XZ v Y not(Z) 
static void G(uchar_t x[4] ,uchar_t y[4] ,uchar_t z[4] ,uchar_t result[4]) {
    // X and Y
    uchar_t temp1[4] ;
    for (int i = 0; i < 4; i++)
    {
        temp1[i] = x[i] & z[i] ;
    }

    // X and not(Z)
    uchar_t temp2[4] ;
    for (int i = 0; i < 4; i++)
    {
        temp2[i] = y[i] & (~z[i]) ;
    }


    // X and Y  OR  X and not(Z)   
    for (int i = 0; i < 4; i++)
    {
        result[i] = temp1[i] | temp2[i]  ;
    }


}


// h(X,Y,Z)  =  X xor Y xor Z
static void H(uchar_t x[4] ,uchar_t y[4] ,uchar_t z[4] ,uchar_t result[4]) {

    // X XOR Y XOR Z
    for (int i = 0; i < 4; i++)
    {
        result[i] = x[i] ^ y[i] ^ z[i] ;
    }


}

// I(X,Y,Z) = Y xor (X v not(Z))
static void I(uchar_t x[4] ,uchar_t y[4] ,uchar_t z[4] ,uchar_t result[4]) {

    // X and not(Z)
    uchar_t temp1[4] ;
    for (int i = 0; i < 4; i++)
    {
        temp1[i] = x[i] | (~z[i]) ;
    }


    // Y  XOR  X and not(Z)   
    for (int i = 0; i < 4; i++)
    {
        result[i] = y[i] ^  temp1[i] ;
    }


}


static void add_uchar_bytes_(uchar_t num1[4] ,uchar_t num2[4] , uchar_t result[4] ) {



    uchar_t carry = 0 ;
    for (int i = 3; i >=0 ; i--)
    {
        uint16_t sum = num1[i] + num2[i] + carry ;
        result[i] = (sum) % 0x100 ;
        carry = sum >> 8 ;

    }

}



static uint32_t rotl_32bit_(uint32_t a , uint32_t times) {
    times %= 32 ;

    if (!times)
        return a ;


    return ((a << times) | (a >> (32 - times))) ;
}


static uint32_t bytes_to_be_(uchar_t input[4]) {
    return (uint32_t) ((input[0] << 24) | (input[1] << 16) | (input[2] << 8) | input[3]) ;
}
// ????
static void be_to_bytes_(uint32_t num , uchar_t input[4]) {
    input[0] = (num & 0xff000000 ) >> 24 ;
    input[1] = (num & 0x00ff0000 ) >> 16 ;
    input[2] = (num & 0x0000ff00 ) >> 8 ;
    input[3] = (num & 0x000000ff )  ;
}


static void reverse_(uchar_t arr[], uint32_t start, uint32_t end) {
    while (start < end) {
        uchar_t temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}



// a = b + ((a + F(b,c,d) + X[k] + T[i]) <<< s)
static uint32_t md5_round1_operation(uchar_t *A , uchar_t *B , uchar_t *C , uchar_t *D  , uchar_t *X, uint32_t k , uint32_t s , uint32_t i , uchar_t *T) {
    
    uchar_t temp[4]  ;
    uchar_t temp2[4]  ;
    uchar_t temp3[4]  ;
    uchar_t temp6[4]  ;
    uchar_t result[4]  ;
    // temp =  f(B,C,D)
    F(B  , C , D , temp) ;
    add_uchar_bytes_(temp , A , temp2) ;


    uchar_t temp4[4]  ;
    memcpy(temp4 , X + k , sizeof(uchar_t)*4) ;
    
    reverse_(temp4 , 0 , 3) ;
    add_uchar_bytes_(temp2 , temp4 , temp3) ;



    uchar_t temp5[4]  ;
    memcpy(temp5  , T + i , sizeof(uchar_t)*4) ;

    add_uchar_bytes_(temp3 , temp5 , temp6) ;
    
    
    be_to_bytes_(  (rotl_32bit_((bytes_to_be_(temp6))  , s)) , result) ;
    
    add_uchar_bytes_(result , B , A) ;

    return 0 ; // ?????????????
}

// a = b + ((a + G(b,c,d) + X[k] + T[i]) <<< s)
static uint32_t md5_round2_operation(uchar_t *A , uchar_t *B , uchar_t *C , uchar_t *D  , uchar_t *X, uint32_t k , uint32_t s , uint32_t i , uchar_t *T) {
    
    uchar_t temp[4]  ;
    uchar_t temp2[4]  ;
    uchar_t temp3[4]  ;
    uchar_t temp6[4]  ;
    uchar_t result[4]  ;
    // temp =  G(B,C,D)
    G(B  , C , D , temp) ;

    add_uchar_bytes_(temp , A , temp2) ;


    uchar_t temp4[4]  ;
    memcpy(temp4 , X + k , sizeof(uchar_t)*4) ;
    
    reverse_(temp4 , 0 , 3) ;
    add_uchar_bytes_(temp2 , temp4 , temp3) ;


    uchar_t temp5[4]  ;
    memcpy(temp5  , T + i , sizeof(uchar_t)*4) ;
    

    add_uchar_bytes_(temp3 , temp5 , temp6) ;
    
    be_to_bytes_(  (rotl_32bit_((bytes_to_be_(temp6))  , s)) , result) ;
    
    add_uchar_bytes_(result , B , A) ;

    // same ????
    return 0 ; 
}



// a = b + ((a + H(b,c,d) + X[k] + T[i]) <<< s)
static uint32_t md5_round3_operation(uchar_t *A , uchar_t *B , uchar_t *C , uchar_t *D  , uchar_t *X, uint32_t k , uint32_t s , uint32_t i , uchar_t *T) {
    
    uchar_t temp[4]  ;
    uchar_t temp2[4]  ;
    uchar_t temp3[4]  ;
    uchar_t temp6[4]  ;
    uchar_t result[4]  ;
    // temp =  H(B,C,D)
    H(B  , C , D , temp) ;
    add_uchar_bytes_(temp , A , temp2) ;

    uchar_t temp4[4]  ;
    memcpy(temp4 , X + k , sizeof(uchar_t)*4) ;
    
    reverse_(temp4 , 0 , 3) ;
    add_uchar_bytes_(temp2 , temp4 , temp3) ;

    uchar_t temp5[4]  ;
    memcpy(temp5  , T + i , sizeof(uchar_t)*4) ;
    
    add_uchar_bytes_(temp3 , temp5 , temp6) ;
    
    be_to_bytes_(  (rotl_32bit_((bytes_to_be_(temp6))  , s)) , result) ;
    
    add_uchar_bytes_(result , B , A) ;

    // ???
    return 0 ; 
}



// a = b + ((a + I(b,c,d) + X[k] + T[i]) <<< s)
static uint32_t md5_round4_operation(uchar_t *A , uchar_t *B , uchar_t *C , uchar_t *D  , uchar_t *X, uint32_t k , uint32_t s , uint32_t i , uchar_t *T) {
    
    uchar_t temp[4]  ;
    uchar_t temp2[4]  ;
    uchar_t temp3[4]  ;
    uchar_t temp6[4]  ;
    uchar_t result[4]  ;
    // temp =  I(B,C,D)
    I(B  , C , D , temp) ;

    add_uchar_bytes_(temp , A , temp2) ;
    
    uchar_t temp4[4]  ;
    memcpy(temp4 , X + k , sizeof(uchar_t)*4) ;
    
    reverse_(temp4 , 0 , 3) ;
    add_uchar_bytes_(temp2 , temp4 , temp3) ;

    uchar_t temp5[4]  ;
    memcpy(temp5  , T + i , sizeof(uchar_t)*4) ;

    add_uchar_bytes_(temp3 , temp5 , temp6) ;
    
    
    be_to_bytes_(  (rotl_32bit_((bytes_to_be_(temp6))  , s)) , result) ;
    add_uchar_bytes_(result , B , A) ;

    // TODO : wth do we return ??
    return 0 ; 
}

static double absd(double a) {
    return a >=0 ? a : -a ;
}

// static void uint32_t_to_bytes(uint32_t number , uchar_t bytes[4] ) {
//     bytes[0] = (number >> 24) & 0xff ;
//     bytes[1] = (number >> 16) & 0xff ;
//     bytes[2] = (number >> 8) & 0xff ;
//     bytes[3] = (number) & 0xff ;
// }



void md5_hash(uchar_t M[] , int N , uchar_t *output) {
    uchar_t A[4] , B[4] , C[4] , D[4] ;

    // first word
    be_to_bytes_(0x67452301 , A) ;
    // second word
    be_to_bytes_(0xefcdab89 , B) ;
    // third word
    be_to_bytes_(0x98badcfe , C) ;
    // fourth word
    be_to_bytes_(0x10325476 , D) ;

 

    uchar_t T[64*4] ; 

    uint32_t b ; 
    int w = 1 ; 
    for (int i = 0; i < 64*4; i+=4)
    {
        long double d =  (absd(sinl(w)) * 4294967296); 
        b = (uint32_t) d ;
        w++ ; 
        be_to_bytes_(b , T + i) ; 
    }
    
    // PRINT_ARRAY(T , 64*4 , "%x") ;

    uchar_t X[4*16] ;
    uchar_t AA[4] , BB[4] , CC[4] , DD[4] ;
    // int j ;
    // printf("N =  %d\n" , N) ; 
    // printf("here you go looping from 0 to N/(16*4)-1 : %d\n" , N/(16*4)-1) ; 
    for (size_t i = 0; i <= (size_t) (N/(16*4)-1); i++)
    {

        memcpy(X  , M + (i* 16 *4 ) , sizeof(uchar_t) * 4*16) ;
        // printf("i = %ld \n" , i) ;


        memcpy(AA , A , sizeof(uchar_t)*4) ;
        memcpy(BB , B , sizeof(uchar_t)*4) ;
        memcpy(CC , C , sizeof(uchar_t)*4) ;
        memcpy(DD , D , sizeof(uchar_t)*4) ;


        // faighting again huh ? a revenge match ??
        // wtv ..
        // ROUND 1 ... FIGHT !!!!
        
        
        md5_round1_operation(A , B , C , D , X , 0*4 , 7 , 0 *4, T ) ;
        md5_round1_operation(D , A , B , C , X , 1*4 , 12 , 1*4 , T ) ;
        md5_round1_operation(C , D , A , B , X , 2*4 , 17 , 2*4 , T ) ;
        md5_round1_operation(B , C , D , A , X , 3*4 , 22 , 3*4 , T ) ;
        
        
        md5_round1_operation(A , B , C , D , X , 4*4 , 7  , 4 *4, T ) ;
        md5_round1_operation(D , A , B , C , X , 5*4 , 12 , 5 *4, T ) ;
        md5_round1_operation(C , D , A , B , X , 6*4 , 17 , 6 *4, T ) ;
        md5_round1_operation(B , C , D , A , X , 7*4 , 22 , 7 *4, T ) ;
        
        md5_round1_operation(A , B , C , D , X , 8 *4 , 7  , 8  *4, T ) ;
        md5_round1_operation(D , A , B , C , X , 9 *4 , 12 , 9  *4, T ) ;
        md5_round1_operation(C , D , A , B , X , 10*4 , 17 , 10 *4, T ) ;
        md5_round1_operation(B , C , D , A , X , 11*4 , 22 , 11 *4 , T ) ;
        
        md5_round1_operation(A , B , C , D , X , 12*4 , 7  , 12 *4, T ) ;
        md5_round1_operation(D , A , B , C , X , 13*4 , 12 , 13 *4, T ) ;
        md5_round1_operation(C , D , A , B , X , 14*4 , 17 , 14 *4, T ) ;
        md5_round1_operation(B , C , D , A , X , 15*4 , 22 , 15 *4, T ) ;
        
        
        
        
        
        // bruh a madness here what the HELL ?
        
        // ROUND 2 ...KEEP FIGHTING 
        
        
        md5_round2_operation(A , B , C , D , X , 1 *4 , 5   , 16*4  , T ) ;
        md5_round2_operation(D , A , B , C , X , 6 *4 , 9   , 17*4  , T ) ;
        md5_round2_operation(C , D , A , B , X , 11*4 , 14  , 18*4 , T ) ;
        md5_round2_operation(B , C , D , A , X , 0 *4 , 20  , 19 *4, T ) ;
        
        md5_round2_operation(A , B , C , D , X , 5 *4 , 5   , 20*4 , T ) ;
        md5_round2_operation(D , A , B , C , X , 10*4 , 9   , 21 *4, T ) ;
        md5_round2_operation(C , D , A , B , X , 15*4 , 14  , 22 *4, T ) ;
        md5_round2_operation(B , C , D , A , X , 4 *4 , 20  , 23 *4, T ) ;

        md5_round2_operation(A , B , C , D , X , 9 *4 , 5   , 24*4 , T ) ;
        md5_round2_operation(D , A , B , C , X , 14*4 , 9   , 25 *4, T ) ;
        md5_round2_operation(C , D , A , B , X , 3*4 , 14  , 26*4, T ) ;
        md5_round2_operation(B , C , D , A , X , 8 *4 , 20  , 27 *4, T ) ;

        md5_round2_operation(A , B , C , D , X , 13 *4 , 5   , 28*4 , T ) ;
        md5_round2_operation(D , A , B , C , X , 2*4 , 9   , 29 *4, T ) ;
        md5_round2_operation(C , D , A , B , X , 7*4 , 14  , 30 *4, T ) ;
        md5_round2_operation(B , C , D , A , X , 12 *4 , 20  , 31 *4, T ) ;



        // ROUND 3 ... FINISH HIM !! im so done with this 

        md5_round3_operation(A , B , C , D , X , 5 *4  , 4   , 32*4 , T ) ;
        md5_round3_operation(D , A , B , C , X , 8*4   , 11  , 33 *4, T ) ;
        md5_round3_operation(C , D , A , B , X , 11*4  , 16  , 34 *4, T ) ;
        md5_round3_operation(B , C , D , A , X , 14 *4 , 23  , 35 *4, T ) ;

        md5_round3_operation(A , B , C , D , X , 1 *4  , 4   , 36*4 , T ) ;
        md5_round3_operation(D , A , B , C , X , 4*4   , 11  , 37 *4, T ) ;
        md5_round3_operation(C , D , A , B , X , 7*4  , 16   , 38 *4, T ) ;
        md5_round3_operation(B , C , D , A , X , 10 *4 , 23  , 39 *4, T ) ;

        md5_round3_operation(A , B , C , D , X , 13 *4  , 4   , 40*4 , T ) ;
        md5_round3_operation(D , A , B , C , X , 0*4   , 11  , 41 *4, T ) ;
        md5_round3_operation(C , D , A , B , X , 3*4  , 16  , 42 *4, T ) ;
        md5_round3_operation(B , C , D , A , X , 6 *4 , 23  , 43 *4, T ) ;

        md5_round3_operation(A , B , C , D , X , 9 *4  , 4   , 44*4 , T ) ;
        md5_round3_operation(D , A , B , C , X , 12*4   , 11  , 45 *4, T ) ;
        md5_round3_operation(C , D , A , B , X , 15*4  , 16  , 46 *4, T ) ;
        md5_round3_operation(B , C , D , A , X , 2 *4 , 23  , 47 *4, T ) ;


        // ROUND 4 ... lets just end this gng

        md5_round4_operation(A , B , C , D , X , 0 *4  , 6   , 48*4 , T ) ;
        md5_round4_operation(D , A , B , C , X , 7*4   , 10  , 49 *4, T ) ;
        md5_round4_operation(C , D , A , B , X , 14*4  , 15  , 50 *4, T ) ;
        md5_round4_operation(B , C , D , A , X , 5 *4  , 21  , 51 *4, T ) ;


        md5_round4_operation(A , B , C , D , X , 12 *4  , 6   , 52*4 , T ) ;
        md5_round4_operation(D , A , B , C , X , 3*4    , 10  , 53 *4, T ) ;
        md5_round4_operation(C , D , A , B , X , 10*4   , 15  , 54 *4, T ) ;
        md5_round4_operation(B , C , D , A , X , 1 *4   , 21  , 55 *4, T ) ;


        md5_round4_operation(A , B , C , D , X , 8 *4   , 6   , 56*4 , T ) ;
        md5_round4_operation(D , A , B , C , X , 15*4   , 10  , 57 *4, T ) ;
        md5_round4_operation(C , D , A , B , X , 6*4    , 15  , 58 *4, T ) ;
        md5_round4_operation(B , C , D , A , X , 13 *4  , 21  , 59 *4, T ) ;


        md5_round4_operation(A , B , C , D , X , 4 *4  , 6   , 60*4 , T ) ;
        md5_round4_operation(D , A , B , C , X , 11*4   , 10  , 61 *4, T ) ;
        md5_round4_operation(C , D , A , B , X , 2*4  , 15  , 62 *4, T ) ;
        md5_round4_operation(B , C , D , A , X , 9 *4  , 21  , 63 *4, T ) ;



        uchar_t temp_A[4]  ;
        uchar_t temp_B[4]  ;
        uchar_t temp_C[4]  ;
        uchar_t temp_D[4]  ;


        add_uchar_bytes_(A , AA , temp_A) ;
        add_uchar_bytes_(B , BB , temp_B) ;
        add_uchar_bytes_(C , CC , temp_C) ;
        add_uchar_bytes_(D , DD , temp_D) ;


        memcpy(A , temp_A , sizeof(uchar_t)*4) ;
        memcpy(B , temp_B , sizeof(uchar_t)*4) ;
        memcpy(C , temp_C , sizeof(uchar_t)*4) ;
        memcpy(D , temp_D , sizeof(uchar_t)*4) ;


    }




    reverse_(A , 0 , 3) ;
    reverse_(B , 0 , 3) ;
    reverse_(C , 0 , 3) ;
    reverse_(D , 0 , 3) ;



    memcpy(output , A , sizeof(uchar_t)*4) ;
    memcpy(output + 4, B , sizeof(uchar_t)*4) ;
    memcpy(output + 8, C , sizeof(uchar_t)*4) ;
    memcpy(output + 12, D , sizeof(uchar_t)*4) ;


 
}


uchar_t* md5_padding(uchar_t M[] , uint64_t b , uint64_t *output_length ) {
    // just being lazy or smarty here :(
    return md4_padding(M , b , output_length) ;
}
