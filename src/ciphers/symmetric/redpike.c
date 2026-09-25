

#include "redpike.h"
#include <string.h>
#include <assert.h>
#include "../../block_cipher_modes_operation.h"


#define ROUNDS 16
#define ROTL(X, R) (((X) << ((R) & 31)) | ((X) >> (32 - ((R) & 31))))
#define ROTR(X, R) (((X) >> ((R) & 31)) | ((X) << (32 - ((R) & 31))))

typedef uint32_t word;
#define REDPIKE_BLOCK_SIZE 8

// all https://web.archive.org/web/20150602092816/http://permalink.gmane.org/gmane.comp.security.cypherpunks/3680

static int redpike_encrypt_block(const uchar_t * x1 , uchar_t *output, const void * key)
{
    assert(key != NULL && "key is null");
    RedpikeKey *redpike_key = (RedpikeKey *) (key) ;
	assert(redpike_key != NULL && "key is null");
	uchar_t *k1 = redpike_key->key ;
    word x[2] ; 
    word k[2] ; 

    x[0] = (x1[0] << 24) | (x1[1] << 16) | (x1[2] << 8) | (x1[3]) ; 
    x[1] = (x1[4] << 24) | (x1[5] << 16) | (x1[6] << 8) | (x1[7]) ; 



    k[0] = (k1[0] << 24) | (k1[1] << 16) | (k1[2] << 8) | (k1[3]) ; 
    k[1] = (k1[4] << 24) | (k1[5] << 16) | (k1[6] << 8) | (k1[7]) ; 

    unsigned int i;
    word rk0 = k[0];



    word rk1 = k[1];


    for (i = 0; i < ROUNDS; i++)
    {
        rk0 += CONST;
        rk1 -= CONST;


        x[0] ^= rk0;
        
        x[0] += x[1];
        x[0] = ROTL(x[0], x[1]);

        x[1] = ROTR(x[1], x[0]);
        x[1] -= x[0];
        x[1] ^= rk1;
    }

    rk0 = x[0]; x[0] = x[1]; x[1] = rk0;


    output[0] = x[0] >> 24 ; 
    output[1] = x[0] >> 16 ; 
    output[2] = x[0] >> 8 ; 
    output[3] = x[0]  ; 


    output[4] = x[1] >> 24 ; 
    output[5] = x[1] >> 16 ; 
    output[6] = x[1] >> 8 ; 
    output[7] = x[1]  ; 


    return 0 ; 
}

static int redpike_decrypt_block(const uchar_t * x1 , uchar_t *output, const void * key)
{
    assert(key != NULL && "key is null");
    RedpikeKey *redpike_key = (RedpikeKey *) (key) ;
    assert(redpike_key != NULL && "key is null");
	uchar_t *k1 = redpike_key->key ;
        
    word x[2] ; 
    word k[2] ; 
    x[0] = (x1[0] << 24) | (x1[1] << 16) | (x1[2] << 8) | (x1[3]) ; 
    x[1] = (x1[4] << 24) | (x1[5] << 16) | (x1[6] << 8) | (x1[7]) ; 



    k[0] = (k1[0] << 24) | (k1[1] << 16) | (k1[2] << 8) | (k1[3]) ; 
    k[1] = (k1[4] << 24) | (k1[5] << 16) | (k1[6] << 8) | (k1[7]) ; 

    word dk[2] =
    {
        k[1] - CONST * (ROUNDS + 1),
        k[0] + CONST * (ROUNDS + 1)
    };
    void *new_key = malloc(sizeof(RedpikeKey));
    RedpikeKey *new_key_handle = (RedpikeKey*) new_key ;
    memcpy(new_key_handle , redpike_key , sizeof(RedpikeKey)) ; 

    new_key_handle->key[0] = dk[0] >> 24 ;
    new_key_handle->key[1] = dk[0] >> 16;
    new_key_handle->key[2] = dk[0] >> 8;
    new_key_handle->key[3] = dk[0] & 0xff  ;

    new_key_handle->key[4] = dk[1] >> 24 ;
    new_key_handle->key[5] = dk[1] >> 16;
    new_key_handle->key[6] = dk[1] >> 8;
    new_key_handle->key[7] = dk[1] & 0xff ;


    redpike_encrypt_block(x1,output ,new_key);
    free(new_key) ; 

    return 0 ; 
}




int redpike_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key) {

    int result =  ecb_encrypt( input , output ,  length , REDPIKE_BLOCK_SIZE  , key , redpike_encrypt_block  ) ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: redpike_encrypt : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ;    
}

int redpike_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key) {

    int result =  ecb_decrypt( input , output ,  length , REDPIKE_BLOCK_SIZE  , key , redpike_decrypt_block  ) ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: redpike_encrypt : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}



int redpike_encrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key)
{

    int result =  cbc_encrypt( input , output , iv , length , REDPIKE_BLOCK_SIZE  , key ,redpike_encrypt_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: redpike_encrypt_cbc : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int redpike_decrypt_cbc(const uchar_t* input, uchar_t* output , uchar_t *iv, int length, const void* key)
{
    int result =  cbc_decrypt( input , output , iv , length , REDPIKE_BLOCK_SIZE  , key ,redpike_decrypt_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: redpike_decrypt_cbc : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int redpike_encrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key)
{

    int result =  cfb_encrypt( input , output , iv , length , REDPIKE_BLOCK_SIZE  , key ,redpike_encrypt_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: redpike_encrypt_cfb : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int redpike_decrypt_cfb(const uchar_t* input, uchar_t* output , uchar_t *iv, int length, const void* key)
{
    int result =  cfb_decrypt( input , output , iv , length , REDPIKE_BLOCK_SIZE  , key ,redpike_encrypt_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: redpike_decrypt_cfb : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}


int redpike_encrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key)
{

    int result =  ofb_encrypt( input , output , iv , length , REDPIKE_BLOCK_SIZE  , key ,redpike_encrypt_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: redpike_encrypt_ofb : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int redpike_decrypt_ofb(const uchar_t* input, uchar_t* output , uchar_t *iv, int length, const void* key)
{
    int result =  ofb_decrypt( input , output , iv , length , REDPIKE_BLOCK_SIZE  , key ,redpike_encrypt_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: redpike_decrypt_ofb : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int redpike_encrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv , int length, const void* key)
{

    int result =  ctr_encrypt( input , output , iv , length , REDPIKE_BLOCK_SIZE  , key ,redpike_encrypt_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: redpike_encrypt_ctr : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}

int redpike_decrypt_ctr(const uchar_t* input, uchar_t* output , uchar_t *iv, int length, const void* key)
{
    int result =  ctr_decrypt( input , output , iv , length , REDPIKE_BLOCK_SIZE  , key ,redpike_encrypt_block ) ; ; 
    if (result != 0) {
        fprintf(stderr , "ERROR: redpike_decrypt_ctr : something wrong happened , couldnt encrypt \n" ) ;
        return 1 ;  
    }

    return 0 ; 
}




int redpike_set_key(void* key_struct, const uchar_t* key_str , size_t key_len) {
    RedpikeKey *redpike_key = (RedpikeKey *) key_struct ;
    if (redpike_key == NULL) {
		fprintf(stderr , "ERROR : redpike_key is null\n") ;
		return 1 ;  
	}
	
	if (key_len != 8) {
		fprintf(stderr , "ERROR : key_len is too short must be (=8) bytes \n") ;
		return 2 ;  
	}

    // how can we make sure that key_str is actually 8 bytes ...
    assert(key_len == 8 && "key length here must be 8 bytes");
    memcpy(redpike_key->key , key_str , sizeof(uchar_t)*REDPIKE_KEY_MAX_SIZE) ; 
    redpike_key->constant = CONST ; 

    redpike_key->type = BLOCK_CIPHER ;

	return 0 ; 
}



int redpike_free_key(void* key_struct) {

	free(key_struct) ; 
	return 0;
}

Cipher* get_redpike_cipher(void);





