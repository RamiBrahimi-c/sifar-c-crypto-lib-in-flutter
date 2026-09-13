#include "../../include/common/galois_field_op.h"


uint16_t keepMSB_16bit(uint16_t a) {
    if (a < 1)
    {
        return 0 ; 
    }
    uint16_t old_a = a ; 
    size_t pos = 0 ;
    while (a > 0)
    {
        old_a = a ;
        a >>= 1 ;
        pos++ ;  
    }
    for (size_t i = 0; i < pos-1; i++)
    {
        old_a <<= 1 ;
    }
    
    
    return old_a ;     

}

uchar_t keepMSB(uchar_t a) {
    if (a < 1)
    {
        return 0 ; 
    }
    
    uchar_t old_a = a ; 
    size_t pos = 0 ;
    while (a > 0)
    {
        old_a = a ;
        a >>= 1 ;
        pos++ ;  
    }
    for (size_t i = 0; i < pos-1; i++)
    {
        old_a <<= 1 ;
    }
    
    
    return old_a ;     

}

uint16_t div_GF_16bit(uint16_t a , uint16_t b) {
    uint16_t temp_a = keepMSB_16bit(a) ; 
    uint16_t temp_b = keepMSB_16bit(b) ;
    
    if (keepMSB_16bit(a) < keepMSB_16bit(b))
    {
        // printf("%c / %c = 0\n" , a , b ) ; 
        return 0;
    }


    uint16_t result = 1 ;

    // printf("dividend : %x \n" , result) ; 

    while (temp_a > temp_b)
    {
        result <<= 1 ;
        temp_a >>= 1 ; 
    }
    // printf("%x / %x in GF(2⁸) : %x \n"  , a , b, result) ; 
    return result ; 

}



uchar_t mod_GF(uint16_t result) {

    uchar_t final_result = result & 0x00ff ;
    // x^8 = 1 + x + x³ + x⁴ = 0001 1011 = 0x1b
    if ((result & 0x0100) != 0)
    {
        final_result ^= 0x1b ; 
    }
    
    
    // x^9 = x + x² + x⁴ + x⁵ = 0011 0110 = 0x36
    if ((result & 0x0200) != 0)
    {
        final_result ^= 0x36 ; 
    }

    


    // x^10 = x² + x³ + x⁵ + x⁶ = 0110 1100 = 0x6c 
    if ((result & 0x0400) != 0)
    {
        final_result ^= 0x6c ; 
    }

    
    // x^11 = x³ + x⁴ + x⁶ + x⁷ = 1101 1000 = 0xd8
    if ((result & 0x0800) != 0)
    {
        final_result ^= 0xd8 ; 
    }
    
    // x^12 = 1 + x + x³ + x⁵ + x⁷
    // final_result ^= (result & 0x1000 == 0) ? 0 : 0xab ;  
    if ((result & 0x1000) != 0)
    {
        final_result ^= 0xab ; 
    }
    
    // x^13 = 1 + x² + x³ + x⁶   = 0100 1101 = 0x4d
    if ((result & 0x2000) != 0)
    {
        final_result ^= 0x4d ; 
    }
    
    // x^14 = x + x³ + x⁴ + x⁷ = 1001 1010 = 0x9a
    if ((result & 0x4000) != 0)
    {
        final_result ^= 0x9a ; 
    }

    return final_result ; 


}


uint16_t mul_GF_16bit(uint16_t a , uint16_t b) {

    uint32_t result = 0 ;
    uint16_t temp = a ;
    int position = 1 ;
    // uint16_t old_b = b ;

    for (size_t i = 0; i < 16; i++)
    {
        temp = a ; 
        if (b % 2 == 1)
        {
            temp <<= position -1 ; 
            
            result ^= temp ;
        }

        b >>= 1 ; 
        position++ ; 
    }



    // printf("%x * %x in GF(2³) : %x \n"  , a , old_b, final_result ) ; 

    return result ; 
}



uchar_t add_GF_16bit(uchar_t a , uchar_t b) {
    return a ^ b ;
}

uint16_t div_euclud_GF_16bit(uint16_t a , uint16_t b ) {
    if (keepMSB_16bit(a) < keepMSB_16bit(b))
    {
        return a ; 
    }
    
    // uint16_t divend = 0 ; 
    // uint16_t old_a = a ; 

    while (keepMSB_16bit(a) >= keepMSB_16bit(b))
    {
        // uchar_t temp_quot =  div_GF(a , b) ;
        uint16_t temp_quot =  div_GF_16bit(a , b) ;
        // mul div * quot
        uint16_t temp_result = mul_GF_16bit(b , temp_quot) ; 
        // result is XORed with 'a'
            uint16_t temp_reminder = add_GF_16bit(a , temp_result) ;

        a = temp_reminder ; 
    }

    // printf("%x %% %x in GF(2^3) : %x \n" ,old_a , b , a );


    return a ; 

}





uint16_t div_GF_16bit_edited(uint16_t a , uint16_t b) {
    if (keepMSB_16bit(a) < keepMSB_16bit(b))
    {
        return 0 ; 
    }
    
    uint16_t divend = 0 ; 
    // uint16_t old_a = a ; 

    while (keepMSB_16bit(a) >= keepMSB_16bit(b))
    {
        // uchar_t temp_quot =  div_GF(a , b) ;
        uint16_t temp_quot =  div_GF_16bit(a , b) ;
        // mul div * quot
        uint16_t temp_result = mul_GF_16bit(b , temp_quot) ; 
        // result is XORed with 'a'
            uint16_t temp_reminder = add_GF_16bit(a , temp_result) ;

        divend |= temp_quot ;  
        a = temp_reminder ; 
    }

    // printf("%x %% %x in GF(2^3) : %x \n" ,old_a , b , a );


    return divend ; 

}


// m ig is always x^8 + x^4 + x^3 + x + 1
// aka 1 0001 1011 = 0x11b
uint16_t modInverse_polynoms(uint16_t n, uint16_t m)
{
    // Check if inverse exists
    // if (gcd(n, m) != 1)
        // return -1;
    
    // uint16_t m0 = m;
    uint16_t y = 0, x = 1;


    // a convention for AES not mathematical one
    if (n == 0)
        return 0;

    if (m == 1)
        return 0;

    while (n > 1) {
        
        // q is quotient
        // int q = n / m;
        uint16_t q = div_GF_16bit_edited(n , m);

        uint16_t t = m;

        // m is remainder now, process same as Euclid's algo
        // m = n % m;
        m = div_euclud_GF_16bit(n , m) ; 

        n = t;
        t = y;

        // Update y and x
        // y = x - q * y;
        
        y  = add_GF_16bit( x,  (mod_GF(mul_GF_16bit(q , y))   )) ; 
        
        // the line above works just like the one below for some mathematical resaon 
        // that i dont know yet ...

        // y  = add_GF_16bit( x,  ((mul_GF_16bit(q , y))   )) ; 
        

        x = t;
    }


    return x;
}
