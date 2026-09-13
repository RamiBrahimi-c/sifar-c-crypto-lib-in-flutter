#include "elgamal.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void elgamal_generate_keypair(ElGamalKey* key, int bits)
{

    bigra9m_inits(&key->p, &key->g, &key->x, &key->y, NULL);

    // 1. Generate large prime p (> 512 bits)
    
    generate_prime_bigra9m3(&key->p , bits) ; 


    // 2. Choose generator g (small prime like 2 or 5 works if safe prime, else random)
    // For simplicity, use g = 2 (check if it's a generator, else 3, 5...)

    unsigned long small_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    
    BigRa9m p_minus_1, q, test , big_1 , big_2;
    bigra9m_inits(&p_minus_1, &q, &test , &big_1 , &big_2, NULL);
    bigra9m_assign_uint64_t(&big_1 , 1) ; 
    bigra9m_assign_uint64_t(&big_2 , 2) ; 
    bigra9m_sub(&key->p , &big_1 , &p_minus_1) ; 

    int found = 0;
    for (int i = 0; i < 10; i++) {

        bigra9m_assign_uint64_t(&key->g ,small_primes[i] ) ; 

        // Quick check: g^((p-1)/2) mod p != 1 (for safe primes)
        
        bigra9m_div2(&p_minus_1 , &big_2 , &q) ;
        bigra9m_modular_exponentiation(&key->g  , &q , &key->p , &test ) ; 
        if (!bigra9m_isEqual_uint64(test, 1) ) {
            found = 1;
            break;
        }
        
    }
    if (!found) {
        // Fallback: just use 2
        bigra9m_assign_uint64_t(&key->g, 2);
        
    }

    // 3. Private key x: random in [1, p-2]
    setrandom_bigra9m2(&key->x , &p_minus_1) ; 
    if (bigra9m_is_zero(key->x)) // my rand is weird and perhaps it might just give a 0
    {
        bigra9m_assign_uint64_t(&key->x , 1) ; // not sure about ts ngl
    }
    


    // 4. Public key y = g^x mod p
    
    bigra9m_modular_exponentiation(&key->g, &key->x, &key->p , &key->y) ; 

    key->bits = bits;

    bigra9m_clears(&p_minus_1, &q, &test , &big_1 , &big_2, NULL);
    
}


void elgamal_encrypt(ElGamalCiphertext* ct, const BigRa9m* message, const ElGamalKey* key)
{

    bigra9m_inits(&ct->c1, &ct->c2, NULL);

    BigRa9m k, p_minus_1 , one_big;
    bigra9m_inits(&k, &p_minus_1 , &one_big, NULL);
    bigra9m_assign_uint64_t(&one_big , 1) ;

    bigra9m_sub(&key->p , &one_big , &p_minus_1) ;
    setrandom_bigra9m2(&k , &p_minus_1) ; 
    if (bigra9m_isEqual_uint64(k , 0)) bigra9m_assign_uint64_t(&k , 1) ; 


    // c1 = g^k mod p
    bigra9m_modular_exponentiation( &key->g, &k, &key->p , &ct->c1 );
    // mpz_powm(ct->c1, key->g, k, key->p);
    
    // c2 = M * y^k mod p
    BigRa9m yk;
    bigra9m_init(&yk);
    bigra9m_modular_exponentiation( &key->y, &k, &key->p , &yk );
    bigra9m_mul(message , &yk , &ct->c2) ; 
    bigra9m_mod( ct->c2,  key->p , &ct->c2 );


    bigra9m_clears(&k, &p_minus_1 , &one_big, NULL);
}


void elgamal_decrypt(BigRa9m* message, const ElGamalCiphertext* ct, const ElGamalKey* key)
{
    // M = c2 * (c1^x)^(-1) mod p
    BigRa9m s, s_inv;
    bigra9m_inits(&s, &s_inv, NULL);

    // s = c1^x mod p
    bigra9m_modular_exponentiation( &ct->c1, &key->x, &key->p , &s);

    // s_inv = s^(-1) mod p
    bigra9m_modinverse(&s , &key->p , &s_inv ) ; 

    
    // M = c2 * s_inv mod p

    bigra9m_mul( &ct->c2, &s_inv , message);
    bigra9m_mod( *message, key->p , message);


    bigra9m_clears(&s, &s_inv, NULL);
}



void elgamal_free_key(ElGamalKey* key)
{
    bigra9m_clears(&key->p, &key->g, &key->x, &key->y, NULL);
}

void elgamal_free_ciphertext(ElGamalCiphertext* ct)
{
    bigra9m_clears(&ct->c1, &ct->c2, NULL);
}

