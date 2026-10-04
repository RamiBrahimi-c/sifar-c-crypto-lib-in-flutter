#ifndef SIFAR_FFI_H
#define SIFAR_FFI_H

#include <stdint.h>
#include <stddef.h>

// Opaque handles — Dart only ever sees pointers to these
// typedef struct AesKey AesKey;
// typedef struct DesKey DesKey;
// typedef struct Rc4Key Rc4Key;
// typedef struct RedpikeKey RedpikeKey;
// typedef struct TeaKey TeaKey;
// typedef struct XTeaKey XTeaKey;

typedef unsigned char uchar_t;

// Public AES API — the only things Dart will call
int aes_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int aes_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int aes_set_key(void* key_struct, const uchar_t* key_str, size_t key_len);
int aes_free_key(void* key_struct);

// Public AES API — the only things Dart will call
int des_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int des_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int des_set_key(void* key_struct, const uchar_t* key_str, size_t key_len);
int des_free_key(void* key_struct);

// Public AES API — the only things Dart will call
int tdes_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int tdes_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int tdes_set_key(void* key_struct, const uchar_t* key_str, size_t key_len);
int tdes_free_key(void* key_struct);

// Public AES API — the only things Dart will call
int blowfish_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int blowfish_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int blowfish_set_key(void* key_struct, const uchar_t* key_str, size_t key_len);
int blowfish_free_key(void* key_struct);

// Public AES API — the only things Dart will call
int rc4_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int rc4_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int rc4_set_key(void* key_struct, const uchar_t* key_str, size_t key_len);
int rc4_free_key(void* key_struct);

// Public AES API — the only things Dart will call
int redpike_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int redpike_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int redpike_set_key(void* key_struct, const uchar_t* key_str, size_t key_len);
int redpike_free_key(void* key_struct);


// Public AES API — the only things Dart will call
int tea_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int tea_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int tea_set_key(void* key_struct, const uchar_t* key_str, size_t key_len);
int tea_free_key(void* key_struct);


int xtea_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int xtea_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int xtea_set_key(void* key_struct, const uchar_t* key_str, size_t key_len);
int xtea_free_key(void* key_struct);


void* aes_new_key(const uchar_t* key, size_t key_len) ; 
void aes_destroy_key(void* k) ; 

void* des_new_key(const uchar_t* key, size_t key_len);
void  des_destroy_key(void* k);

void* tdes_new_key(const uchar_t* key, size_t key_len);
void  tdes_destroy_key(void* k);

void* blowfish_new_key(const uchar_t* key, size_t key_len);
void  blowfish_destroy_key(void* k);

void* rc4_new_key(const uchar_t* key, size_t key_len);
void  rc4_destroy_key(void* k);

void* redpike_new_key(const uchar_t* key, size_t key_len);
void  redpike_destroy_key(void* k);

void* tea_new_key(const uchar_t* key, size_t key_len);
void  tea_destroy_key(void* k);

void* xtea_new_key(const uchar_t* key, size_t key_len);
void  xtea_destroy_key(void* k);



int encrypt_image_file(char* name, uchar_t* input_path, uchar_t* output_path,
                       uchar_t* _key, size_t _key_len);


int decrypt_image_file(char* name, uchar_t* input_path, uchar_t* output_path,
                       uchar_t* _key, size_t _key_len);


int get_block_size(const char* cipher_name);  // returns 0 if unknown
int get_key_size(const char* cipher_name) ;



// hashing stuff 

// Hashing
int get_digest_size(const char* hash_name);

void md4_hash(uchar_t M[], int N, uchar_t* output);
void md5_hash(uchar_t M[], int N, uchar_t* output);
void sha256_hash(const uchar_t* data, size_t len, uchar_t digest[32]);
void sha512_hash(const uchar_t* data, size_t len, uchar_t digest[64]);



int cipher_encrypt_mode(
    const char* cipher, const char* mode,
    const uchar_t* input, uchar_t* output, const uchar_t* iv,
    int length, const void* key);

int cipher_decrypt_mode(
    const char* cipher, const char* mode,
    const uchar_t* input, uchar_t* output, const uchar_t* iv,
    int length, const void* key);


int has_iv(const char* mode) ;



#endif
