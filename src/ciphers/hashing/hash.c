#include "hash.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


// Count differing bits between two digests
static int count_diff_bits(const uchar_t* d1, const uchar_t* d2, int len)
{
    int count = 0;
    for (int i = 0; i < len; i++) {
        uchar_t diff = d1[i] ^ d2[i];
        // Count set bits
        for (int b = 0; b < 8; b++)
            if (diff & (1 << b)) count++;
    }
    return count;
}

double avalanche_test(const uchar_t* input, size_t len,
                      void (*hash_fn)(const uchar_t*, size_t, uchar_t*),
                      int digest_len)
{
    uchar_t* modified = malloc(len);
    memcpy(modified, input, len);

    uchar_t digest1[64], digest2[64];
    hash_fn(input, len, digest1);

    // Total bits across many runs
    int total_diff = 0;
    int runs = 100;

    for (int r = 0; r < runs; r++) {
        // Flip a random bit
        memcpy(modified, input, len);
        int byte_idx = rand() % len;
        int bit_idx = rand() % 8;
        modified[byte_idx] ^= (1 << bit_idx);

        hash_fn(modified, len, digest2);
        total_diff += count_diff_bits(digest1, digest2, digest_len);
    }

    free(modified);
    return (double)total_diff / (runs * digest_len * 8) * 100.0;
}

double bench_hash(void (*hash_fn)(const uchar_t*, size_t, uchar_t*),
                  size_t total_bytes, int digest_len)
{
    digest_len = digest_len ; 
    uchar_t* data = malloc(total_bytes);
    memset(data, 'A', total_bytes);

    uchar_t digest[64];
    clock_t start = clock();
    hash_fn(data, total_bytes, digest);
    clock_t end = clock();

    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    double mb_per_sec = (total_bytes / (1024.0 * 1024.0)) / seconds;

    free(data);
    return mb_per_sec;
}


// Constant-time compare: OR all byte differences, never short-circuit.
// Returns 0 iff equal — preserves memcmp semantics for the equality case.
int ct_memcmp(const void* a, const void* b, size_t len)
{
    const uchar_t* pa = (const uchar_t*)a;
    const uchar_t* pb = (const uchar_t*)b;
    uchar_t diff = 0;
    for (size_t i = 0; i < len; i++) diff |= (uchar_t)(pa[i] ^ pb[i]);
    return diff;
}