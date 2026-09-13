#include "dh.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>



void dh_generate_params(BigRa9m* p, BigRa9m* g, int bits)
{
    // Generate large prime
    generate_prime_bigra9m3(p , bits) ; 


    // Generator: use 2 or 5
    BigRa9m p_minus_1, q, test , big_1 , big_2;
    bigra9m_inits(&p_minus_1, &q, &test , &big_1 , &big_2, NULL);
    bigra9m_assign_uint64_t(&big_1 , 1) ;
    bigra9m_assign_uint64_t(&big_2 , 2) ;

    bigra9m_sub(p , &big_1 , &p_minus_1) ; 


    unsigned long candidates[] = {2, 3, 5, 7, 11, 13};
    int found = 0;

    for (int i = 0; i < 6; i++) {
        bigra9m_assign_uint64_t(g , candidates[i] ) ; 
        bigra9m_div2(&p_minus_1 , &big_2 , &q ) ; 

        bigra9m_modular_exponentiation(g , &q , p , &test) ; 


        if (!bigra9m_isEqual_uint64(test, 1)) {
            found = 1;
            break;
        }        
    }
    if (!found) bigra9m_assign_uint64_t(g, 2);

    bigra9m_clears(&p_minus_1, &q, &test , &big_1 , &big_2, NULL);
}

void dh_generate_keypair(DHParty* party, const BigRa9m* p, const BigRa9m* g)
{
    bigra9m_assign(&party->p, *p);   // store p

    BigRa9m p_minus_1 , big_1; 
    bigra9m_inits(&p_minus_1 , &big_1 , NULL) ; 
    bigra9m_sub(p , &big_1 , &p_minus_1) ; 


    setrandom_bigra9m2(&party->private_key , &p_minus_1) ; 
    
    if (bigra9m_isEqual_uint64(party->private_key, 0) )
        bigra9m_assign_uint64_t(&party->private_key, 1);

    bigra9m_modular_exponentiation( g, &party->private_key, p , &party->public_key);

    bigra9m_clears(&p_minus_1 , &big_1 , NULL) ; 

}



void dh_compute_shared(BigRa9m* shared, const DHParty* me, const BigRa9m* their_public)
{
    bigra9m_modular_exponentiation( their_public , &me->private_key, &me->p ,shared ) ;
}



void dh_clear_party(DHParty* party)
{
    bigra9m_clears(&party->p, &party->private_key, &party->public_key, NULL);
}

