#include "../../../include/ciphers/hashing/hash.h"

/*
    the manuel : https://datatracker.ietf.org/doc/html/rfc1186#autoid-4
*/


static uint32_t bytes_to_be(uchar_t input[4]) {
    return (uint32_t) ((input[0] << 24) | (input[1] << 16) | (input[2] << 8) | input[3]) ;
}

// ????
static void be_to_bytes(uint32_t num , uchar_t input[4]) {
    input[0] = (num & 0xff000000 ) >> 24 ;
    input[1] = (num & 0x00ff0000 ) >> 16 ;
    input[2] = (num & 0x0000ff00 ) >> 8 ;
    input[3] = (num & 0x000000ff )  ;
}



static uint32_t rotl_32bit(uint32_t a , uint32_t times) {
    times %= 32 ;

    if (!times)
        return a ;


    return ((a << times) | (a >> (32 - times))) ;
}


static void reverse(uchar_t arr[], uint32_t start, uint32_t end) {
    while (start < end) {
        uchar_t temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}
// 32-bit conversion
// static uint32_t swap_endian_32(uint32_t value) {
//     return ((value >> 24) & 0xFF) |
//            ((value >> 8)  & 0xFF00) |
//            ((value << 8)  & 0xFF0000) |
//            ((value << 24) & 0xFF000000);
// }



static void add_uchar_bytes(uchar_t num1[4] ,uchar_t num2[4] , uchar_t result[4] ) {



    uchar_t carry = 0 ;
    for (int i = 3; i >=0 ; i--)
    {
        uint16_t sum = num1[i] + num2[i] + carry ;
        result[i] = (sum) % 0x100 ;
        carry = sum >> 8 ;

    }

}




// f(X,Y,Z)  =  XY v not(X)Z
static void f(uchar_t x[4] ,uchar_t y[4] ,uchar_t z[4] ,uchar_t result[4]  ) {

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


// g(X,Y,Z)  =  XY v XZ v YZ
static void g(uchar_t x[4] ,uchar_t y[4] ,uchar_t z[4] ,uchar_t result[4]) {
    // X and Y
    uchar_t temp1[4] ;
    for (int i = 0; i < 4; i++)
    {
        temp1[i] = x[i] & y[i] ;
    }

    // X and Z
    uchar_t temp2[4] ;
    for (int i = 0; i < 4; i++)
    {
        temp2[i] = x[i] & z[i] ;
    }

    // Y and Z
    uchar_t temp3[4] ;
    for (int i = 0; i < 4; i++)
    {
        temp3[i] = y[i] & z[i] ;
    }

    // X and Y  OR  X and Z OR  Y and Z
    for (int i = 0; i < 4; i++)
    {
        result[i] = temp1[i] | temp2[i] | temp3[i] ;
    }


}


// h(X,Y,Z)  =  X xor Y xor Z
static void h(uchar_t x[4] ,uchar_t y[4] ,uchar_t z[4] ,uchar_t result[4]) {

    // X XOR Y XOR Z
    for (int i = 0; i < 4; i++)
    {
        result[i] = x[i] ^ y[i] ^ z[i] ;
    }


}


// A = (A + f(B,C,D) + X[i]) <<< s
static uint32_t round1_operation(uchar_t *A , uchar_t *B , uchar_t *C , uchar_t *D  , uchar_t *X, uint32_t i , uint32_t s) {
    
    uchar_t temp[4]  ;
    uchar_t temp2[4]  ;
    uchar_t temp3[4]  ;
    // temp =  f(B,C,D)
    f(B  , C , D , temp) ;
    // printf("f(B,C,D) : %.8x\n" , bytes_to_be(temp)) ;

    add_uchar_bytes(temp , A , temp2) ;




    uchar_t temp4[4]  ;
    memcpy(temp4 , X + i , sizeof(uchar_t)*4) ;
    
    
    
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    reverse(temp4 , 0 , 3) ;




    add_uchar_bytes(temp2 , temp4 , temp3) ;



    be_to_bytes(  (rotl_32bit((bytes_to_be(temp3))  , s)) , A) ;

    return 0;
}



// A = (A + g(B,C,D) + X[i] + 0x5A827999) <<< s
static uint32_t round2_operation(uchar_t* A , uchar_t* B , uchar_t* C , uchar_t* D  , uchar_t *X, uint32_t i , uint32_t s) {

    uchar_t temp[4]  ;
    uchar_t temp2[4]  ;
    uchar_t temp3[4]  ;
    // temp =  g(B,C,D)
    g(B  , C , D , temp) ;
    add_uchar_bytes(temp , A , temp2) ;
    uchar_t temp4[4]  ;
    uchar_t temp5[4]  ;
    uchar_t temp6[4]  ;
    memcpy(temp4 , X + i , sizeof(uchar_t)*4) ;



    reverse(temp4 , 0 , 3) ;
    
    
    
    add_uchar_bytes(temp2 , temp4 , temp3) ;
    be_to_bytes(0x5A827999 , temp5) ;
    add_uchar_bytes(temp3 , temp5 , temp6) ;
    
    
    be_to_bytes(rotl_32bit( (bytes_to_be(temp6)) , s) , A) ;


    return 0 ; 
}


// A = (A + h(B,C,D) + X[i] + 6ED9EBA1) <<< s
static uint32_t round3_operation(uchar_t* A , uchar_t* B , uchar_t* C , uchar_t* D  , uchar_t *X, uint32_t i , uint32_t s) {
    
    uchar_t temp[4]  ;
    uchar_t temp2[4]  ;
    uchar_t temp3[4]  ;

    h(B  , C , D , temp) ;
    
    add_uchar_bytes(temp , A , temp2) ;

    uchar_t temp4[4]  ;
    uchar_t temp5[4]  ;
    uchar_t temp6[4]  ;
    memcpy(temp4 , X + i , sizeof(uchar_t)*4) ;
    
    
    
    reverse(temp4 , 0 , 3) ;
    
    
    add_uchar_bytes(temp2 , temp4 , temp3) ;

    be_to_bytes(0x6ED9EBA1 , temp5) ;
    
    add_uchar_bytes(temp3 , temp5 , temp6) ;

    
    be_to_bytes(rotl_32bit( (bytes_to_be(temp6)) , s) , A) ;




    // be_to_bytes(rotl_32bit((uint32_t) (bytes_to_le(A) +bytes_to_le(temp) + swap_endian_32(bytes_to_le(X + i) ) + 0x6ED9EBA1) , s  ) , A );
    return 0 ; 
}


// WARNING: we are returning a pointer to dynamically allocated memory that needs to be freed !!! (unless you dont care ofc :) ...)
uchar_t* md4_padding(uchar_t M[] , uint64_t b , uint64_t *output_length ) {
    // b number of bits ?
    // the specification says that it needs not be to be mulytiply of 8 ??? (pretty much weird)
    *output_length = b +64 +  (512 - ((b+64)  % 512)) ;

    uchar_t *output = malloc(*output_length * sizeof(uchar_t)  ) ;
    if (b % 8 == 0)
    {
        memcpy(output , M  , sizeof(uchar_t)* b / 8) ;
        // 512 / 8 = 64
        // this contains length % 64 bytes
        uint32_t temp = (b / 8) % 64  ;
        uint32_t index =  b / 8 ;

        output[index] = 0x80 ;
        index++ ;
        temp++ ;

        // 448 / 8 = 56
        while (temp % 64 != 56)
        {
            output[index] = 0x00 ;
            index++ ;
            temp++ ;
        }
        // append length now ... .???????????????????????

        
        #if 1
        output[index]   =   (b & 0x00000000000000ff ) ; 
        output[index+1] =   (b & 0x000000000000ff00 ) >> 8 ; 
        output[index+2] =   (b & 0x0000000000ff0000 ) >> 16; 
        output[index+3] =   (b & 0x00000000ff000000 ) >> 24; 
        index += 4 ; 
        
        output[index]   =   (b & 0x000000ff00000000 ) >> 32; 
        output[index+1] =   (b & 0x0000ff0000000000 ) >> 40; 
        output[index+2] =   (b & 0x00ff000000000000 ) >> 48; 
        output[index+3] =   (b & 0xff00000000000000 ) >> 56; 
        index += 4 ; 
        #else

                output[index] =   (b & 0xff00000000000000 ) >> 56 ; 
                output[index+1] = (b & 0x00ff000000000000 ) >> 48 ; 
                output[index+2] = (b & 0x0000ff0000000000 ) >> 40; 
                output[index+3] = (b & 0x000000ff00000000 ) >> 32; 
        
                index += 4 ; 
        
                output[index] =   (b & 0x00000000ff000000 ) >> 24 ; 
                output[index+1] = (b & 0x0000000000ff0000 ) >> 16 ; 
                output[index+2] = (b & 0x000000000000ff00 ) >> 8; 
                output[index+3] = (b & 0x00000000000000ff ); 
        
        
                index += 4 ; 
        #endif


         printf("calculated output length : %ld \n" , *output_length ) ;
         printf("calculated output length/8 : %ld \n" , *output_length / 8) ;
        printf("index : %u \n" , index) ;
        *output_length = index *8; 
        assert(index == *output_length / 8 && "these 2 should be equal ???") ;
    } else {
        // ????????????????????????
        // TODO : handle this case too ...
        // ????????????????????????
        printf("not implemented (my pc works with only 8 bits :( \n")  ;
        exit(12) ;
    }

    return output ;
}



void md4_hash(uchar_t M[] , int N , uchar_t *output) {



    uchar_t A[4] , B[4] , C[4] , D[4] ;

    
    // first word
    be_to_bytes(0x67452301 , A) ;
    // second word
    be_to_bytes(0xefcdab89 , B) ;
    // third word
    be_to_bytes(0x98badcfe , C) ;
    // fourth word
    be_to_bytes(0x10325476 , D) ;



    uchar_t X[4*16] ;
    uchar_t AA[4] , BB[4] , CC[4] , DD[4] ;
    printf("N =  %d\n" , N) ; 
    printf("here you go looping from 0 to N/(16*4)-1 : %d\n" , N/(16*4)-1) ; 
    for (size_t i = 0; i <=(size_t) (N/(16*4)-1); i++)
    {

        memcpy(X  , M + (i* 16 *4 ) , sizeof(uchar_t) * 4*16) ;
        // printf("i = %ld \n" , i) ;


        memcpy(AA , A , sizeof(uchar_t)*4) ;
        memcpy(BB , B , sizeof(uchar_t)*4) ;
        memcpy(CC , C , sizeof(uchar_t)*4) ;
        memcpy(DD , D , sizeof(uchar_t)*4) ;



        // ROUND 1 ... FIGHT
        
        
        round1_operation(A , B , C , D , X , 0 , 3 ) ;
        round1_operation(D , A , B , C , X , 4 , 7 ) ;
        round1_operation(C , D , A , B , X , 8 , 11 ) ;
        round1_operation(B , C , D , A , X , 12 , 19 ) ;


        round1_operation(A , B , C , D , X , 16 , 3 ) ;
        round1_operation(D , A , B , C , X , 20 , 7 ) ;
        round1_operation(C , D , A , B , X , 24 , 11 ) ;
        round1_operation(B , C , D , A , X , 28 , 19 ) ;


        round1_operation(A , B , C , D , X , 32 , 3 ) ;
        round1_operation(D , A , B , C , X , 36 , 7 ) ;
        round1_operation(C , D , A , B , X , 40 , 11 ) ;
        round1_operation(B , C , D , A , X , 44 , 19 ) ;


        round1_operation(A , B , C , D , X , 48 , 3 ) ;
        round1_operation(D , A , B , C , X , 52 , 7 ) ;
        round1_operation(C , D , A , B , X , 56 , 11 ) ;
        round1_operation(B , C , D , A , X , 60 , 19 ) ;
        
        
        
        
        
        
        
        // ROUND 2 ...KEEP FIGHTING
        
        round2_operation(A , B , C , D , X , 0 , 3 ) ;
        round2_operation(D , A , B , C , X , 16 , 5 ) ;
        round2_operation(C , D , A , B , X , 32 , 9 ) ;
        round2_operation(B , C , D , A , X , 48 , 13 ) ;



        round2_operation(A , B , C , D , X , 4 , 3 ) ;
        round2_operation(D , A , B , C , X , 20 , 5 ) ;
        round2_operation(C , D , A , B , X , 36 , 9 ) ;
        round2_operation(B , C , D , A , X , 52 , 13 ) ;

        
        
        round2_operation(A , B , C , D , X , 8 , 3 ) ;
        round2_operation(D , A , B , C , X , 24 , 5 ) ;
        round2_operation(C , D , A , B , X , 40 , 9 ) ;
        round2_operation(B , C , D , A , X , 56 , 13 ) ;


        round2_operation(A , B , C , D , X , 12 , 3 ) ;
        round2_operation(D , A , B , C , X , 28 , 5 ) ;
        round2_operation(C , D , A , B , X , 44 , 9 ) ;
        round2_operation(B , C , D , A , X , 60 , 13 ) ;
        

        
        
        
        
        // ROUND 3 ... FINISH HIM

        round3_operation(A , B , C , D , X , 0 , 3 ) ;
        round3_operation(D , A , B , C , X , 32 , 9 ) ;
        round3_operation(C , D , A , B , X , 16 , 11 ) ;
        round3_operation(B , C , D , A , X , 48 , 15 ) ;



        round3_operation(A , B , C , D , X , 8 , 3 ) ;
        round3_operation(D , A , B , C , X , 40 , 9 ) ;
        round3_operation(C , D , A , B , X , 24 , 11 ) ;
        round3_operation(B , C , D , A , X , 56 , 15 ) ;



        round3_operation(A , B , C , D , X , 4 , 3 ) ;
        round3_operation(D , A , B , C , X , 36 , 9 ) ;
        round3_operation(C , D , A , B , X , 20 , 11 ) ;
        round3_operation(B , C , D , A , X , 52 , 15 ) ;
   


        round3_operation(A , B , C , D , X , 12 , 3 ) ;
        round3_operation(D , A , B , C , X , 44 , 9 ) ;
        round3_operation(C , D , A , B , X , 28 , 11 ) ;
        round3_operation(B , C , D , A , X , 60 , 15 ) ;



        uchar_t temp_A[4]  ;
        uchar_t temp_B[4]  ;
        uchar_t temp_C[4]  ;
        uchar_t temp_D[4]  ;


        add_uchar_bytes(A , AA , temp_A) ;
        add_uchar_bytes(B , BB , temp_B) ;
        add_uchar_bytes(C , CC , temp_C) ;
        add_uchar_bytes(D , DD , temp_D) ;


        memcpy(A , temp_A , sizeof(uchar_t)*4) ;
        memcpy(B , temp_B , sizeof(uchar_t)*4) ;
        memcpy(C , temp_C , sizeof(uchar_t)*4) ;
        memcpy(D , temp_D , sizeof(uchar_t)*4) ;


    }




    reverse(A , 0 , 3) ;
    reverse(B , 0 , 3) ;
    reverse(C , 0 , 3) ;
    reverse(D , 0 , 3) ;



    memcpy(output , A , sizeof(uchar_t)*4) ;
    memcpy(output + 4, B , sizeof(uchar_t)*4) ;
    memcpy(output + 8, C , sizeof(uchar_t)*4) ;
    memcpy(output + 12, D , sizeof(uchar_t)*4) ;


    // PRINT_ARRAY(result , 16 , "%.2x") ;
}
