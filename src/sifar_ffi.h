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

void* rc4_new_key(const uchar_t* key, size_t key_len);
void  rc4_destroy_key(void* k);

void* redpike_new_key(const uchar_t* key, size_t key_len);
void  redpike_destroy_key(void* k);

void* tea_new_key(const uchar_t* key, size_t key_len);
void  tea_destroy_key(void* k);

void* xtea_new_key(const uchar_t* key, size_t key_len);
void  xtea_destroy_key(void* k);


// If you need to allocate a key struct from Dart:
// (you'll need to know how big AesKey is — see note below)
// size_t aes_key_size(void);

int encrypt_image_file(char* name, uchar_t* input_path, uchar_t* output_path,
                       uchar_t* _key, size_t _key_len);


int decrypt_image_file(char* name, uchar_t* input_path, uchar_t* output_path,
                       uchar_t* _key, size_t _key_len);


int get_block_size(const char* cipher_name);  // returns 0 if unknown
int get_key_size(const char* cipher_name) ;


#endif
