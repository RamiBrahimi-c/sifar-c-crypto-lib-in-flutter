/*
    custom string module for a better setting key operation
    and to handle the length properly better that the `strlen` approach that actually produced bugs ...
*/
#ifndef CUSTOM_STRING_H
#define CUSTOM_STRING_H

#include <stdio.h>
#include "utils.h"

typedef struct 
{
    size_t length ; 
    uchar_t *string;    
}CString;

// maybe it needs some functions ??

// a function that mimicks strcpy from `<string.h>` 
// it copies `src.length` BYTES from `src.string` to `dest`
// the user is responsible for allocation bytes to `uchar_t *dest`
// returns 0 on success , otherise it returns -1 and reports error to stderr  
int cstrcpy(uchar_t *dest , CString src) ;
void print_cstring(CString str);
void init_cstring(CString *dest , uchar_t *src , size_t len);

#endif