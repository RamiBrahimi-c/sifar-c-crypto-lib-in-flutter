#ifndef DH_H
#define DH_H

#include "bigra9m.h"

typedef struct {
    BigRa9m p;             // shared prime 
    BigRa9m private_key;
    BigRa9m public_key;
    char name[32];
} DHParty;


void dh_generate_params(BigRa9m* p, BigRa9m* g, int bits);
void dh_generate_keypair(DHParty* party, const BigRa9m* p, const BigRa9m* g);
void dh_compute_shared(BigRa9m* shared, const DHParty* me, const BigRa9m* their_public);
void dh_clear_party(DHParty* party);



#endif