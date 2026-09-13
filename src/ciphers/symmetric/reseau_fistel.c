#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void fistel_iteration(unsigned char *src, unsigned char *dest, void (*fistel_function)(unsigned char*, unsigned char*))
{
    int l = strlen(src);
    
    if (l % 2 == 1)
    {
        src[l] = 'x';
        src[l + 1] = '\0';
    }
    

    unsigned char c = src[l / 2];
    src[l / 2] = '\0';

    unsigned char *left = strdup(src);
    src[l / 2] = c;

    unsigned char *right = strdup(src + l / 2);

    unsigned char *crypter_right = malloc((strlen(right) + 2) * sizeof(unsigned char));

    fistel_function(right, crypter_right);

    for (int i = 0; i < l / 2; i++)
    {
        left[i] = left[i] ^ crypter_right[i];
    }
    
    strcpy(dest, right);
    strcat(dest, left);

    free(left);
    free(right);
    free(crypter_right);
}


void encrypt_fistel(unsigned char *src, unsigned char *dest, int n_iter, void (*fistel_function)(unsigned char*, unsigned char*))
{
    unsigned char* temp_src = strdup(src);

    for (int i = 0; i < n_iter; i++)
    {
        fistel_iteration(temp_src, dest, fistel_function);
        strcpy(temp_src, dest);
    }
    
    free(temp_src);
}