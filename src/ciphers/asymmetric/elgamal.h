#ifndef ELGAMAL_H
#define ELGAMAL_H

#include "bigra9m.h"

typedef struct {
    BigRa9m p;      // large prime (> 512 bits)
    BigRa9m g;      // generator
    BigRa9m x;      // private key (random)
    BigRa9m y;      // public key (y = g^x mod p)
    int bits;
} ElGamalKey;

typedef struct {
    BigRa9m c1;     // g^k mod p
    BigRa9m c2;     // M * y^k mod p
} ElGamalCiphertext;



void elgamal_generate_keypair(ElGamalKey* key, int bits);
void elgamal_encrypt(ElGamalCiphertext* ct, const BigRa9m* message, const ElGamalKey* key);
void elgamal_decrypt(BigRa9m* message, const ElGamalCiphertext* ct, const ElGamalKey* key);
void elgamal_free_key(ElGamalKey* key);
void elgamal_free_ciphertext(ElGamalCiphertext* ct);



#endif