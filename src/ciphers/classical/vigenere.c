#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static unsigned char *vigenere_key =(unsigned char *) "Zakaria";

void encrypt_vigenere(unsigned char *src, unsigned char *dest)
{
    unsigned char *c;
    unsigned char *new_src = malloc(sizeof(unsigned char) * strlen((const char*)src));
    unsigned char *new_vigenere_key = malloc(sizeof(unsigned char) * strlen((const char*) vigenere_key));

    int i = 0;
    
    for (c = vigenere_key; *c != '\0'; c++)
    {
        new_vigenere_key[i] = toupper(*c);
        i++;
    }

    i = 0;
    for (c = src; *c != '\0'; c++)
    {
        new_src[i] = toupper(*c);
        i++;
    }
    
    unsigned char *d = dest;
    i = 0;

    for (c = new_src; *c != '\0'; c++)
    {
        *d = (*c - 'A' + vigenere_key[i] - 'A') % 26 + 'A';
        d++;
        i++;
    }
    
    free(src);
    free(vigenere_key);
}
