#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static unsigned char* playfair_key =(unsigned char*) "Zakaria";

static unsigned char grid[5][5];

static void init_grid()
{
    unsigned char array[26] = {0};
    unsigned char *c;

    unsigned char *new_playfair_key = malloc(sizeof(unsigned char) * strlen((const char*) playfair_key));

    int i = 0;
    
    for (c = playfair_key; *c != '\0'; c++)
    {
        new_playfair_key[i] = toupper(*c);

        if (new_playfair_key[i] == 'J')
        {
            new_playfair_key[i] = 'I';
        }
        
        i++;
    }

    int j;    

    i = 0;
    for (c = new_playfair_key; *c != '\0'; c++)
    {        
        if (!array[*c - 'A'])
        {
            array[*c - 'A'] = 1;
            grid[i][j] = *c;
            
            j++;
            if (j >= 5)
            {
                j = 0;
                i++;
            }
        }
        
    }
    
    for (int k = 0; k < 26; k++)
    {
        if (!array[k])
        {
            if (k == 'J')
            {
                grid[i][j] = k - 1;
            }
            else
            {
                grid[i][j] = k;
            }

            j++;
            if (j >= 5)
            {
                j = 0;
                i++;
            }
        }
        
    }
    
    free(new_playfair_key);
}

void encrypt_playfair(unsigned char *src, unsigned char *dest)
{
    unsigned char *c;
    unsigned char *new_src = malloc(2 * sizeof(unsigned char) * strlen((const char*) src));

    int i = 0;
    for (c = src; *c != '\0'; c++)
    {
        new_src[i] = toupper(*c);
        
        if (new_src[i] == 'J')
        {
            new_src[i] = 'I';
        }
        
        i++;
    }

    int i1, j1, i2, j2;
    
    unsigned char *d = dest;
    unsigned char *e;
    c = new_src;
    
    while (*c != '0')
    {
        if (*(c + 1) == '0' || *c == *(c + 1))
        {
            if (*c == 'X')
            {
                *(c + 1) = 'W';
            }
            else
            {
                *(c + 1) = 'X';
            }

            e = c + 1;

            while (*e != '0')
            {
                *(e + 1) = *e;
                e++;
            }
            
            *e = '\0';
            
            if (*c == 'X')
            {
                *(c + 1) = 'W';
            }
            else
            {
                *(c + 1) = 'X';
            }
            
        }
        
        i1 = 0;
        j1 = 0;
        
        while (i1 < 5 && grid[i1][j1] != *c)
        {
            while (j1 < 5 && grid[i1][j1] != *c)
            {
                j1++;
            }
            i1++;
        }

        i2 = 0;
        j2 = 0;
        
        while (i2 < 5 && grid[i2][j2] != *(c + 1))
        {
            while (j2 < 5 && grid[i2][j2] != *(c + 1))
            {
                j1++;
            }
            i2++;
        }

        if (i1 == i2)
        {
            *d = grid[i1][(j1 + 1) % 5];
            *(d + 1) = grid[i2][(j2 + 1) % 5];
        }
        else if (j1 == j2)
        {
            *d = grid[(i1 + 1) % 5][j1];
            *(d + 1) = grid[(i2 + 1) % 5][j2];
        }
        else
        {
            *d = grid[i2][j1];
            *(d + 1) = grid[i1][j2];
        }
        

        c = c + 2;
        d = d + 2;
    }

    free(new_src);
}
