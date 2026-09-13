#include "../../include/common/custom_string.h"



int cstrcpy(uchar_t *dest , CString src) {
    if (dest == NULL || src.string == NULL)
    {
        fprintf(stderr , "ERROR : dest OR src.string is NULL\n");
        return -1;
    }
    
    for (size_t i = 0; i < src.length; i++)
    {
        dest[i] = src.string[i];
    }
    dest[src.length]=0x0 ;
    return 0;
}

void print_cstring(CString str){
    
}

void init_cstring(CString *dest , uchar_t *src , size_t len) {
    dest->length = len ;
    dest->string = malloc(sizeof(uchar_t)*len) ; 
    assert(dest->string != NULL);
    memcpy(dest->string , src , sizeof(uchar_t)*len) ; 
}

