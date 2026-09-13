#include "sifar_ffi.h"
#include "ciphers/symmetric/aes.h"
#include <stdlib.h>

void* aes_new_key(const uint8_t* key, size_t key_len) {
    AesKey* k = malloc(sizeof(AesKey));
    if (aes_set_key(k, key, key_len) != 0) { free(k); return NULL; }
    return k;
}
void aes_destroy_key(void* k) { free(k); }
