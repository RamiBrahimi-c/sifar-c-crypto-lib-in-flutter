#include "block_cipher_modes_operation.h"
#include "cipher_interface.h"
/*
    TODO : OFB is not tested with official test vectors
    TODO : OFB for decryption needs a better approach cuz the same function is needed for both encrypt + decrypt 
    TODO : same thing with CTR (tested it with tineyAES in test.c line 239) although it has the problem of same function called twice
    TODO : yea same for CFB
*/

BlockCipher_MODE_OP block_cipher_mode_operation ;

void blockcipher_encrypt_modeop(uchar_t *input , uchar_t *output ,uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*encrypt_block)(const uchar_t* , uchar_t* , const void* )) {
    switch (block_cipher_mode_operation)
    {
        case ECB:
            printf("encrypting in ECB mode *-*\n");
            ecb_encrypt(input , output  , length , block_size , key , encrypt_block); 
            break;

        case CBC:
            printf("encrypting in CBC mode *-*\n");
            cbc_encrypt(input , output , iv , length , block_size , key , encrypt_block); 
            break;

        case OFB:
            printf("encrypting in OFB mode *-*\n");
            ofb_encrypt(input , output , iv , length , block_size , key , encrypt_block); 
            break;

        case CTR:
            printf("encrypting in CTR mode *-*\n");
            ctr_encrypt(input , output , iv , length , block_size , key , encrypt_block); 
            break;

        case CFB:
            printf("encrypting in CFB mode *-*\n");
            cfb_encrypt(input , output , iv , length , block_size , key , encrypt_block); 
            break;

        
        default:
            printf("encrypting in ECB mode *-*\n");
            ecb_encrypt(input , output , length , block_size , key , encrypt_block); 
            break;
    }
}
void blockcipher_decrypt_modeop(uchar_t *input , uchar_t *output ,uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*decrypt_block)(const uchar_t* , uchar_t* , const void* )) {
    switch (block_cipher_mode_operation)
    {
        case ECB:
            printf("decrypting in ECB mode *-*\n");
            ecb_decrypt(input , output  , length , block_size , key , decrypt_block); 
            break;

        case CBC:
            printf("decrypting in CBC mode *-*\n");
            cbc_decrypt(input , output , iv , length , block_size , key , decrypt_block); 
            break;

        case OFB:
            printf("decrypting in OFB mode *-*\n");
            ofb_encrypt(input , output , iv , length , block_size , key , decrypt_block); 
            break;

        case CTR:
            printf("decrypting in CTR mode *-*\n");
            ctr_encrypt(input , output , iv , length , block_size , key , decrypt_block); 
            break;

        case CFB:
            printf("decrypting in CFB mode *-*\n");
            cfb_decrypt(input , output , iv , length , block_size , key , decrypt_block); 
            break;


        default:
            printf("decrypting in ECB mode *-*\n");
            ecb_decrypt(input , output , length , block_size , key , decrypt_block); 
            break;
    }
}

void ecb_encrypt(uchar_t *input , uchar_t *output , size_t length , size_t block_size  ,const void* key ,void (*encrypt_block)(const uchar_t* , uchar_t*  , const void* ) ) {

    // simple dimple check
    size_t t = length / block_size ;
    // assert()
    if (t*block_size != length)
    {
        fprintf(stderr , "ERROR: make sure the input is padded or smth cuz length/block_size = %ld/%ld = %f \n" , length , block_size , (float) (length/block_size) ) ;
        return;
    }
    
    printf("looping from 0 to %ld\n", t);

    for (size_t i = 0; i < t ; i++)
    {
        encrypt_block(input + i*block_size , output + i*block_size , key) ;
    }
    

}

void ecb_decrypt(uchar_t *input , uchar_t *output , size_t length , size_t block_size  ,const void* key ,void (*decrypt_block)(const uchar_t* , uchar_t* , const void* ) ) {

    // simple dimple check
    size_t t = length / block_size ;
    // assert()
    if (t*block_size != length)
    {
        fprintf(stderr , "ERROR: make sure the input is padded or smth cuz length/block_size = %ld/%ld = %f \n" , length , block_size , (float) (length/block_size) ) ;
        return;
    }
    


    for (size_t i = 0; i < t ; i++)
    {
        decrypt_block(input + i*block_size , output + i*block_size , key) ;
    }
    

}


void cbc_encrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*encrypt_block)(const uchar_t* , uchar_t* , const void* ) ) {

    // simple dimple check
    size_t t = length / block_size ;
    // assert()
    if (t*block_size != length || length < block_size )
    {
        fprintf(stderr , "ERROR: (length < block_size ) OR make sure the input is padded or smth cuz length/block_size = %ld/%ld = %f \n" , length , block_size , (float) (length/block_size) ) ;
        return;
    }
    
    uchar_t *buffer =(uchar_t*) malloc(sizeof(uchar_t)*block_size) ; 
    assert(buffer != NULL && "ummmm buy more ram lol");

    mapOperation(input , iv , buffer , block_size , binaryXorUchar) ;
    
    encrypt_block(buffer , output  , key) ;
    
    for (size_t i = 1; i < t ; i++)
    {
        mapOperation(input + i*block_size , output + (i-1)*block_size , buffer , block_size , binaryXorUchar) ;
        encrypt_block(buffer , output + i*block_size , key) ;
    }
    

    free(buffer);
}

void cbc_decrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*decrypt_block)(const uchar_t* , uchar_t* , const void* ) ) {

    // simple dimple check
    size_t t = length / block_size ;
    if (t*block_size != length || length < block_size )
    {
        fprintf(stderr , "ERROR: (length < block_size ) OR make sure the input is padded or smth cuz length/block_size = %ld/%ld = %f \n" , length , block_size , (float) (length/block_size) ) ;
        return;
    }
    
    uchar_t *buffer =(uchar_t*) malloc(sizeof(uchar_t)*block_size) ; 
    assert(buffer != NULL && "ummmm buy more ram lol");

    decrypt_block(input , buffer  , key) ;
    mapOperation(buffer , iv , output , block_size , binaryXorUchar) ;
    
    
    for (size_t i = 1; i < t ; i++)
    {
        decrypt_block(input +i*block_size , buffer  , key) ;
        mapOperation(buffer , input + (i-1)*block_size , output + i*block_size , block_size , binaryXorUchar) ;
    }
    

    free(buffer);    
}


void ofb_encrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*encrypt_block)(const uchar_t* , uchar_t*, const void* ) ) {
    size_t t = length / block_size ;
    if (t*block_size != length || length < block_size )
    {
        fprintf(stderr , "ERROR: (length < block_size ) OR make sure the input is padded or smth cuz length/block_size = %ld/%ld = %f \n" , length , block_size , (float) (length/block_size) ) ;
        return;
    }

    // basically the Si
    uchar_t *prev_si =(uchar_t*) malloc(sizeof(uchar_t)*block_size) ; 
    uchar_t *cur_si =(uchar_t*) malloc(sizeof(uchar_t)*block_size) ; 
    assert(prev_si != NULL && "ummmm buy more ram lol");
    assert(cur_si != NULL && "ummmm buy more ram lol");

    encrypt_block(iv , cur_si  , key) ;
    mapOperation(cur_si , input , output , block_size , binaryXorUchar) ;

    memcpy(prev_si , cur_si , sizeof(uchar_t)*block_size) ; 
    for (size_t i = 1; i < t ; i++)
    {
        encrypt_block(prev_si , cur_si , key) ;
        mapOperation( cur_si , input + i*block_size , output + i*block_size , block_size , binaryXorUchar) ;
        memcpy(prev_si , cur_si , sizeof(uchar_t)*block_size) ; 
    }
    

    free(cur_si);
    free(prev_si);

}


void ofb_decrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*decrypt_block)(const uchar_t* , uchar_t*, const void* ) ) {
    // ofb_encrypt(input , output , iv , length , block_size , key , ofb_encrypt) ;
}

void ctr_encrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*encrypt_block)(const uchar_t* , uchar_t* , const void* ) ) {
    size_t t = length / block_size ;
    if (t*block_size != length || length < block_size )
    {
        fprintf(stderr , "ERROR: (length < block_size ) OR make sure the input is padded or smth cuz length/block_size = %ld/%ld = %f \n" , length , block_size , (float) (length/block_size) ) ;
        return;
    }
    uchar_t *buffer =(uchar_t*) malloc(sizeof(uchar_t)*block_size) ; 
    assert(buffer != NULL && "ummmm buy more ram lol");
    
    encrypt_block(iv , buffer  , key) ;
    mapOperation(buffer , input , output , block_size , binaryXorUchar) ;
    
    
    for (size_t i = 1; i < t ; i++)
    {
        encrypt_block(iv , buffer , key) ;
        mapOperation(buffer , input + i*block_size , output + i*block_size  , block_size , binaryXorUchar) ;
    }
    
    
    free(buffer);
    
}

void ctr_decrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*decrypt_block)(const uchar_t* , uchar_t* , const void* ) ) {
    // ctr_encrypt(input , output , iv , length , block_size , key , decrypt_block) ;
    
}

void cfb_encrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*encrypt_block)(const uchar_t* , uchar_t*, const void* ) ) {
    size_t t = length / block_size ;
    if (t*block_size != length || length < block_size )
    {
        fprintf(stderr , "ERROR: (length < block_size ) OR make sure the input is padded or smth cuz length/block_size = %ld/%ld = %f \n" , length , block_size , (float) (length/block_size) ) ;
        return;
    }

    // basically the Si
    uchar_t *buffer =(uchar_t*) malloc(sizeof(uchar_t)*block_size) ; 
    assert(buffer != NULL && "ummmm buy more ram lol");

    encrypt_block(iv , buffer  , key) ;

    mapOperation(buffer , input , output , block_size , binaryXorUchar) ;
    
    for (size_t i = 1; i < t ; i++)
    {
        encrypt_block(output + (i-1)*block_size  , buffer , key) ;
        mapOperation( buffer , input + i*block_size , output + i*block_size , block_size , binaryXorUchar) ;
    }
    

    free(buffer);
    
}

void cfb_decrypt(uchar_t *input , uchar_t *output , uchar_t *iv , size_t length , size_t block_size  ,const void* key ,void (*decrypt_block)(const uchar_t* , uchar_t*, const void* ) ) {
    size_t t = length / block_size ;
    if (t*block_size != length || length < block_size )
    {
        fprintf(stderr , "ERROR: (length < block_size ) OR make sure the input is padded or smth cuz length/block_size = %ld/%ld = %f \n" , length , block_size , (float) (length/block_size) ) ;
        return;
    }

    // basically the Si
    uchar_t *buffer =(uchar_t*) malloc(sizeof(uchar_t)*block_size) ; 
    assert(buffer != NULL && "ummmm buy more ram lol");

    decrypt_block(iv , buffer  , key) ;

    mapOperation(buffer , input , output , block_size , binaryXorUchar) ;
    
    for (size_t i = 1; i < t ; i++)
    {
        decrypt_block(input + (i-1)*block_size  , buffer , key) ;
        mapOperation( buffer , input + i*block_size , output + i*block_size , block_size , binaryXorUchar) ;
    }
    

    free(buffer);

}

