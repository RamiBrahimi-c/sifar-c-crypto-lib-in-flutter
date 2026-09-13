#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static unsigned char caesar_key = 5;

void encrypt_caesar(unsigned char *src , int, unsigned char *dest)
{
    unsigned char *c;
    unsigned char *new_src = malloc(sizeof(unsigned char) * strlen((const char*) src));

    int i = 0;
    for (c = src; *c != '\0'; c++)
    {
        new_src[i] = toupper(*c);
        i++;
    }

    unsigned char *d = dest;

    for (c = new_src; *c != '\0'; c++)
    {
        *d = (*c - 'A' + caesar_key) % 26 + 'A';
        d++;
    }
    
    free(new_src);
}
