#include <stdlib.h>
#include <time.h>
// #include "bigra9m.h"

#include "../../../include/ciphers/asymmetric/rsa.h"
#include "../../../include/ciphers/symmetric/aes.h"
#include "../../../include/common/utils.h"
#include "../../../include/common/constants.h"

#define DEBUG_GENKEYPAIR 0
#define DEBUG_RSA_ENC 0
#define DEBUG_RSA_DEC 0

void rsa_generate_keypair(RsaKey* key, int bits, unsigned long e_value)
{
    BigRa9m p, q, n, phi, p1, q1, gcd , big_1;
    bigra9m_inits(&p, &q, &n, &phi, &p1, &q1, &gcd , &big_1, NULL);
    bigra9m_assign_uint64_t(&big_1 , 1);
    
    int prime_bits = bits / 2 ;
    
    // Generate p

    generate_prime_bigra9m3(&p , prime_bits + 1) ; 

    #if  DEBUG_GENKEYPAIR
        printf("bits : %d \n" , bits) ;
        printf("prime bits : %d \n" , prime_bits) ;
        printf("p : ") ;
        bigra9m_print(p) ; 
    #endif 

    do
    {
        generate_prime_bigra9m3(&q , prime_bits) ; 
        
    } while (bigra9m_isEqualNum(p , q));

    #if DEBUG_GENKEYPAIR
        printf("q : ") ;
        bigra9m_print(q) ; 
    #endif

    // n = p * q
    bigra9m_mul( &p, &q , &key->n);
    
    // phi = (p-1)*(q-1)
    bigra9m_sub(&p , &big_1 , &p1 ) ; 
    bigra9m_sub(&q , &big_1 , &q1 ) ; 
    bigra9m_mul(&p1, &q1 , &phi);
    
    #if DEBUG_GENKEYPAIR
        printf("phi : ") ; 
        bigra9m_print(phi) ; 
    #endif
    
    // e
    bigra9m_assign_uint64_t(&key->e , e_value) ; 
    #if DEBUG_GENKEYPAIR
        printf("e : \n") ; 
    
        bigra9m_print(key->e) ;
    #endif
    
    bigra9m_gcd(&key->e ,&phi , &gcd) ;
    
    #if DEBUG_GENKEYPAIR
        printf("gcd : \n") ; 
        bigra9m_print(gcd) ;
    #endif
    
    if (!bigra9m_isEqual_uint64(gcd ,1 ))
    {
        bigra9m_assign_uint64_t(&key->e , 65537) ; 
        
    }
    
    // d = e⁻¹ mod phi
    bigra9m_modinverse(&key->e , &phi , &key->d) ; 
    
    #if DEBUG_GENKEYPAIR
        printf("d : ") ; 
        bigra9m_print(key->d) ;
    #endif

    key->bits = bits;
    key->is_private = 1;

    bigra9m_clears(&p, &q, &n, &phi, &p1, &q1, &gcd ,&big_1 , NULL);
}


void rsa_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key)
{
    const RsaKey* rsa = (const RsaKey*)key;
    size_t out_bytes = (rsa->bits + 7) / 8;
    memset(output, 0, out_bytes);

    BigInt m , c ;
    bigra9m_inits(&m , &c , NULL) ; 
    bigra9m_import(&m , length , sizeof(input[0]) , input ) ; 
    #if DEBUG_RSA_ENC
        printf("m : \n") ; 
        bigra9m_print(m) ; 
    #endif
    
    bigra9m_modular_exponentiation(&m , &rsa->e , &rsa->n , &c ) ; 
    
    #if DEBUG_RSA_ENC
        printf("c : \n") ; 
        bigra9m_print(c) ; 
    #endif

    // Export to temporary buffer, then right-align into output
    size_t count = 0;
    unsigned char temp[1024];
    bigra9m_export(temp , &count , sizeof(temp[0]) , &c) ; 

    #if DEBUG_RSA_ENC
        printf("out bytes : %ld \n" , out_bytes) ; 
        printf("count : %ld \n" , count) ;
    #endif
 
    if (count <= out_bytes) {
        memcpy(output + (out_bytes - count), temp, count);
    } else {
        memcpy(output, temp + (count - out_bytes), out_bytes);
    }

    bigra9m_clears(&m , &c , NULL) ; 
}

void rsa_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key)
{
    const RsaKey* rsa = (const RsaKey*)key;
    if (!rsa || !rsa->is_private || length <= 0) return;
    size_t out_bytes = (rsa->bits + 7) / 8;
    // printf("buf boooo \n") ; 
    // PRINT_ARRAY_NAI(input , length , "%x") ; 
    BigInt m , c ;
    bigra9m_inits(&m , &c , NULL) ; 
    bigra9m_import(&c , length , sizeof(input[0]) , input ) ; 
    #if DEBUG_RSA_DEC
    printf("c : \n") ; 
    bigra9m_print(c) ; 
    #endif
    bigra9m_modular_exponentiation(&c , &rsa->d , &rsa->n , &m ) ; 
    
    #if DEBUG_RSA_DEC
    printf("m : \n") ; 
    bigra9m_print(m) ; 
    #endif

    // Export to temp, then copy right‑aligned into output buffer
    size_t count = 0;
    unsigned char temp[1024];
    bigra9m_export(temp , &count , sizeof(temp[0]) , &m) ; 
    
    if (count <= out_bytes) {
        memcpy(output + (out_bytes - count), temp, count);
    } else {
        memcpy(output, temp + (count - out_bytes), out_bytes);
    }

    #if DEBUG_RSA_DEC
        printf("fuck you\n") ; 
    #endif
    bigra9m_clears(&m , &c , NULL) ;
}


void rsa_set_key(void* key_struct, const char* key_str , size_t key_len)
{
    RsaKey* rsa = (RsaKey*)key_struct;
    // parse "n_hex:e_hex:d_hex:bits"
    char* copy = strdup(key_str);
    char* tok_n = strtok(copy, ":");
    char* tok_e = strtok(NULL, ":");
    char* tok_d = strtok(NULL, ":");
    char* tok_bits = strtok(NULL, ":");
    if (tok_n && tok_e && tok_d && tok_bits) {
        bigra9m_assign_str(&rsa->n, tok_n);
        bigra9m_assign_str(&rsa->e, tok_e);
        bigra9m_assign_str(&rsa->d, tok_d);
        // mpz_set_str(rsa->n, tok_n, 16);
        // mpz_set_str(rsa->e, tok_e, 16);
        // mpz_set_str(rsa->d, tok_d, 16);
        rsa->bits = atoi(tok_bits);
        rsa->is_private = 1;
        printf("wth1\n") ; 
    } else if (tok_n && tok_e && tok_bits) {
        bigra9m_assign_str(&rsa->n, tok_n);
        bigra9m_assign_str(&rsa->e, tok_e);
        rsa->bits = atoi(tok_bits);
        rsa->is_private = 0;
        printf("wth2\n") ; 
    } else {
        printf("wth\n") ; 
    }
    free(copy);
}



void rsa_free_key(void* key_struct)
{
    RsaKey* rsa = (RsaKey*)key_struct;
    bigra9m_clears(&rsa->n, &rsa->e, &rsa->d, NULL);
    // mpz_clears(rsa->n, rsa->e, rsa->d, NULL);
}



void rsa_export_key(const RsaKey* key) {
    printf("Modulus n (hex): ");
    bigra9m_print(key->n) ; 
    printf("Public exponent e (hex): ");
    bigra9m_print(key->e) ; 
    if (key->is_private) {
        printf("Private exponent d (hex): ");
        bigra9m_print(key->d) ; 
    }
    printf("Bits: %d\n", key->bits);
}

void test_rsa_32bytes(int bits) {
    printf("testing with %d bits\n", bits);
    fflush(stdout);

    RsaKey key;
    // // // // // // // mpz_inits(key., key.e, key.d, NULL);
    bigra9m_inits(&key.e , &key.n , &key.d , NULL) ; 

    rsa_generate_keypair(&key, bits, 65537);
    printf("generated key pairs successfully for %d bits\n", bits);
    fflush(stdout);

    uchar_t plain[32];
    for (int i = 0; i < 32; i++) plain[i] = (uchar_t)(i + 0x41);

    size_t out_bytes = (bits + 7) / 8;
    uchar_t* cipher = malloc(out_bytes);
    uchar_t* decrypted = malloc(out_bytes);

    rsa_encrypt(plain, cipher, 32, &key);
    rsa_decrypt(cipher, decrypted, out_bytes, &key);

    uchar_t recovered[32];
    memcpy(recovered, decrypted + (out_bytes - 32), 32);

    printf("RSA-%d match: %s\n", bits,
           memcmp(plain, recovered, 32) == 0 ? "YES" : "NO");
    fflush(stdout);

    free(cipher);
    free(decrypted);
    rsa_free_key(&key);
}


void sayhi() {
    printf("hi fuck this all\n") ; 
}


void hybrid_test(void) {
    printf("============================= hybrid test =============================\n") ; 
    printf("hi") ;
    RsaKey rsa;
    bigra9m_inits(&rsa.n, &rsa.e, &rsa.d, NULL) ; 
    // mpz_inits(rsa.n, rsa.e, rsa.d, NULL);
    rsa_generate_keypair(&rsa, 2048, 65537);
    
    // 1. Random AES‑256 key (32 bytes)
    uchar_t aes_key[32];
    for (int i = 0; i < 32; i++) aes_key[i] = getRandomInt(0xcafe0) & 0xFF;   // weak randomness
    
    // 2. Encrypt AES key with RSA
    size_t rsa_out = (rsa.bits + 7) / 8;
    uchar_t encrypted_key[rsa_out];
    clock_t t1 = clock();
    rsa_encrypt(aes_key,  encrypted_key , 32, &rsa);
    clock_t t2 = clock();
    double rsa_time = (double)(t2 - t1) / CLOCKS_PER_SEC;

    // 3. Read 1 MB file (or create a dummy 1 MB buffer)
    size_t file_size = 1024 * 1024;
    uchar_t* file_data = malloc(file_size);
    memset(file_data, 'A', file_size);   // dummy content

    // 4. Encrypt with AES 
    AesKey aes_ctx;   // assuming AES struct
    aes_set_key(&aes_ctx, (const uchar_t*) aes_key , 32);

    size_t padded_size = file_size + (16 - (file_size % 16));   // PKCS#7 padding
    uchar_t* aes_cipher = malloc(padded_size);
    clock_t t3 = clock();
    aes_encrypt(file_data, aes_cipher, file_size, &aes_ctx);   // this will write padded_size bytes
    clock_t t4 = clock();
    double aes_time = (double)(t4 - t3) / CLOCKS_PER_SEC;

    printf("RSA key encryption time: %f s\n", rsa_time);
    printf("AES file encryption time: %f s\n", aes_time);

    free(file_data);
    free(aes_cipher);
    rsa_free_key(&rsa);
    printf("============================= end hybrid test =============================\n") ; 
}
