#ifndef HASH_H
#define HASH_H

#include "../../common/utils.h"

#include <stdint.h>
#include <stddef.h>

#define MD5_DIGEST_LENGTH    16
#define SHA256_DIGEST_LENGTH 32
#define SHA512_DIGEST_LENGTH 64

#define SHA256_BLOCK_SIZE 64
#define SHA512_BLOCK_SIZE 128



// cute md4 ...
void md4_hash(uchar_t M[] , int N , uchar_t *output) ;
uchar_t* md4_padding(uchar_t M[] , uint64_t b , uint64_t *output_length ) ;

// md5 ... (pls be easy to implement)  . after : well the effort was done in MD4 , im glad .
void md5_hash(uchar_t M[] , int N , uchar_t *output) ;
uchar_t* md5_padding(uchar_t M[] , uint64_t b , uint64_t *output_length ) ;

uchar_t* sha512_padding(const uchar_t M[] , uint64_t b , uint64_t *output_length ) ;

// about to replace all the below :(

void sha256_hash(const uchar_t* data, size_t len, uchar_t digest[32]);
void sha512_hash(const uchar_t* data, size_t len, uchar_t digest[64]);

// TODO : RE-IMPLEMENT THE BELOW METHODS 


// Helper: compute hash of a file
void md5_file(const char* filename, uchar_t digest[16]);
void sha256_file(const char* filename, uchar_t digest[32]);



// Avalanche test: flip one bit in input, compare digests
double avalanche_test(const uchar_t* input, size_t len,
                      void (*hash_fn)(const uchar_t*, size_t, uchar_t*),
                      int digest_len);


// Benchmark: MB/s
double bench_hash(void (*hash_fn)(const uchar_t*, size_t, uchar_t*),
                  size_t total_bytes, int digest_len);

// HMAC-SHA256 / HMAC-SHA512 per RFC 2104.
// `key` may be any length; if longer than the block size it is first hashed.
void hmac_sha256(const uchar_t* key, size_t key_len,
                 const uchar_t* data, size_t data_len,
                 uchar_t mac[32]);

void hmac_sha512(const uchar_t* key, size_t key_len,
                 const uchar_t* data, size_t data_len,
                 uchar_t mac[64]);

void hkdf_extract_sha256(const uchar_t* salt, size_t salt_len,
                         const uchar_t* ikm, size_t ikm_len,
                         uchar_t prk[32]);

void hkdf_expand_sha256(const uchar_t prk[32],
                       const uchar_t* info, size_t info_len,
                       uchar_t* okm, size_t okm_len);

void hkdf_sha256(const uchar_t* salt, size_t salt_len,
                const uchar_t* ikm, size_t ikm_len,
                const uchar_t* info, size_t info_len,
                uchar_t* okm, size_t okm_len);

void hkdf_extract_sha512(const uchar_t* salt, size_t salt_len, const uchar_t* ikm, size_t ikm_len, uchar_t prk[64]) ;

void hkdf_expand_sha512(const uchar_t prk[64],
                       const uchar_t* info, size_t info_len,
                       uchar_t* okm, size_t okm_len) ;

void hkdf_sha512(const uchar_t* salt, size_t salt_len,
                const uchar_t* ikm, size_t ikm_len,
                const uchar_t* info, size_t info_len,
                uchar_t* okm, size_t okm_len) ;

// Constant-time memory compare for MAC verification.
// Returns 0 iff the two buffers are equal; never short-circuits.
int ct_memcmp(const void* a, const void* b, size_t len);

#endif