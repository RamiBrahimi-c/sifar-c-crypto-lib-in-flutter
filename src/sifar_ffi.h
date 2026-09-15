#ifndef SIFAR_FFI_H
#define SIFAR_FFI_H

#include <stdint.h>
#include <stddef.h>

// Opaque handles — Dart only ever sees pointers to these
typedef struct AesKey AesKey;

typedef unsigned char uchar_t;

// Public AES API — the only things Dart will call
int aes_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int aes_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
int aes_set_key(void* key_struct, const uchar_t* key_str, size_t key_len);
int aes_free_key(void* key_struct);


void* aes_new_key(const uchar_t* key, size_t key_len) ; 
void aes_destroy_key(void* k) ; 

// If you need to allocate a key struct from Dart:
// (you'll need to know how big AesKey is — see note below)
// size_t aes_key_size(void);


int encrypt_image_file(char* name, uchar_t* input_path, uchar_t* output_path,
                       uchar_t* _key, size_t _key_len);

#endif
