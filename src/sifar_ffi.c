#include "sifar_ffi.h"
#include "ciphers/symmetric/aes.h"
#include "ciphers/symmetric/des.h"
#include "ciphers/symmetric/3des.h"
#include "ciphers/symmetric/blowfish.h"
#include "ciphers/symmetric/rc4.h"
#include "ciphers/symmetric/redpike.h"
#include "ciphers/symmetric/tea.h"
#include "ciphers/symmetric/xtea.h"
#include <stdlib.h>
#include <string.h>

void* aes_new_key(const uint8_t* key, size_t key_len) {
    AesKey* k = malloc(sizeof(AesKey));
    if (aes_set_key(k, key, key_len) != 0) { free(k); return NULL; }
    return k;
}
void aes_destroy_key(void* k) { free(k); }

void* des_new_key(const uint8_t* key, size_t key_len) {
    DesKey* k = malloc(sizeof(DesKey));
    if (des_set_key(k, key, key_len) != 0) { free(k); return NULL; }
    return k;
}
void des_destroy_key(void* k) { free(k); }

void* tdes_new_key(const uint8_t* key, size_t key_len) {
    TDesKey* k = malloc(sizeof(TDesKey));
    if (tdes_set_key(k, key, key_len) != 0) { free(k); return NULL; }
    return k;
}
void tdes_destroy_key(void* k) { free(k); }


void* rc4_new_key(const uint8_t* key, size_t key_len) {
    Rc4Key* k = malloc(sizeof(Rc4Key));
    if (rc4_set_key(k, key, key_len) != 0) { free(k); return NULL; }
    return k;
}
void rc4_destroy_key(void* k) { free(k); }



void* blowfish_new_key(const uint8_t* key, size_t key_len) {
    BlowfishKey* k = malloc(sizeof(BlowfishKey));
    if (blowfish_set_key(k, key, key_len) != 0) { free(k); return NULL; }
    return k;
}
void blowfish_destroy_key(void* k) { free(k); }



void* redpike_new_key(const uint8_t* key, size_t key_len) {
    RedpikeKey* k = malloc(sizeof(RedpikeKey));
    if (redpike_set_key(k, key, key_len) != 0) { free(k); return NULL; }
    return k;
}
void redpike_destroy_key(void* k) { free(k); }



void* tea_new_key(const uint8_t* key, size_t key_len) {
    TeaKey* k = malloc(sizeof(TeaKey));
    if (tea_set_key(k, key, key_len) != 0) { free(k); return NULL; }
    return k;
}
void tea_destroy_key(void* k) { free(k); }


void* xtea_new_key(const uint8_t* key, size_t key_len) {
    XTeaKey* k = malloc(sizeof(XTeaKey));
    if (xtea_set_key(k, key, key_len) != 0) { free(k); return NULL; }
    return k;
}
void xtea_destroy_key(void* k) { free(k); }



#define STB_IMAGE_IMPLEMENTATION
#include "third-party/stb-nothing/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "third-party/stb-nothing/stb_image_write.h"


#define STB_VORBIS_IMPLEMENTATION
#include "third-party/stb-nothing/stb_vorbis.c"



static int handle_encryption(char *algo , uchar_t *key_str , size_t key_len , uchar_t * original_text ,uchar_t * encrypted_text  ,size_t length) {

    if (strcmp(algo , "aes")==0)
    {
        void *key_cipher = (void*) calloc(1 , sizeof(AesKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(AesKey) ) ; 
            return 1;
        }
        // cprintf( YELLOW , "INFO: setting key... \n");
        aes_set_key(key_cipher , key_str ,  key_len) ; 
        
        aes_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }

    // else if (strcmp(algo , "des"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(DesKey)) ;
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     des_set_key(key_cipher , key_str ,  key_len) ; 
    //     des_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (strcmp(algo , "tea"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(TeaKey)) ;
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     tea_set_key(key_cipher , key_str ,  key_len) ; 
    //     tea_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (strcmp(algo , "xtea"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(XTeaKey)) ;
        
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     xtea_set_key(key_cipher , key_str ,  key_len) ; 
    //     xtea_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (strcmp(algo , "redpike"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(RedpikeKey)) ;
        
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     redpike_set_key(key_cipher , key_str ,  key_len) ; 
    //     redpike_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (strcmp(algo , "rc4"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(Rc4Key)) ;
        
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     rc4_set_key(key_cipher , key_str ,  key_len) ; 
    //     rc4_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (strcmp(algo , "blowfish"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(BlowfishKey)) ;
        
        
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     blowfish_set_key(key_cipher , key_str ,  key_len) ; 
    //     blowfish_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (strcmp(algo , "affine"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(AffineKey)) ;
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     affine_set_key(key_cipher , key_str ,  key_len) ; 
    //     affine_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
        
    // }
    
    // else if (strcmp(algo , "hill"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(HillKey)) ;
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     hill_set_key(key_cipher , key_str ,  key_len) ; 
    //     hill_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 

        
    // }

    return 1 ; 
}


// return 0 in SUCCESS otherwise its positive integer with error printed in stderr 
int encrypt_image_file(char *name  , uchar_t *input_path,uchar_t* output_path, uchar_t * _key ,size_t _key_len) {


    printf("INFO: Testing %s... \n" , name  ) ; 
    printf("INFO: TYPE Image \n"   ) ; 

    
    int width, height, channels;
    char *algo_name = name ; 
    
    
    uchar_t *original_text = stbi_load(input_path, &width, &height, &channels, 0);
    if (original_text == NULL) {
        fprintf(stderr, "ERROR : couldnt extract pixels from original picture \n") ; 
        return 1 ;
    }
    
    int length = width * height * channels ; 
    
    // this is a lil bit risky yk ...

    if (length % 8 != 0   )
    {

        printf("INFO : doing a random padding *-*\n");
        int rest = length % 8 ;
        length += (8-rest);
    }
    
    uchar_t *encrypted_text = malloc(sizeof(uchar_t) * length );
    if (encrypted_text == NULL) {
        fprintf(stderr, "ERROR : malloc failed to allocate %ld bytes \n" , sizeof(uchar_t) * length) ; 
        stbi_image_free(original_text);    
        return 2 ;
    }

    
    int enc_result = handle_encryption(name ,_key , _key_len , original_text , encrypted_text , length ) ;
    if (enc_result != 0) {
        fprintf(stderr, "ERROR : handle for encryption failed (code %d)\n" , enc_result ) ;
        
        free(encrypted_text) ; 
        stbi_image_free(original_text);
        return 3; 
    }
    printf(  "INFO: encrypted with success\n" );
    
    int __res_funv_ = stbi_write_png(output_path, width, height, channels, encrypted_text, width * channels);
    if (__res_funv_ == 0) {
        fprintf(stderr, "ERROR : stbi write png failed \n") ; 

        free(encrypted_text) ; 
        stbi_image_free(original_text);

        return 4;
    }
    
    printf( "INFO: saved to %s (code %d ) \n" , output_path , __res_funv_  );

    
    free(encrypted_text);
    stbi_image_free(original_text);
    
    
    return  0;


}





static int handle_decryption(char *algo , uchar_t *key_str , size_t key_len , uchar_t * original_text ,uchar_t * encrypted_text  ,size_t length) {

    if (strcmp(algo , "aes")==0)
    {
        void *key_cipher = (void*) calloc(1 , sizeof(AesKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(AesKey) ) ; 
            return 1;
        }
        // cprintf( YELLOW , "INFO: setting key... \n");
        aes_set_key(key_cipher , key_str ,  key_len) ; 
        
        aes_decrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }

    // else if (strcmp(algo , "des"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(DesKey)) ;
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     des_set_key(key_cipher , key_str ,  key_len) ; 
    //     des_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (strcmp(algo , "tea"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(TeaKey)) ;
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     tea_set_key(key_cipher , key_str ,  key_len) ; 
    //     tea_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (strcmp(algo , "xtea"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(XTeaKey)) ;
        
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     xtea_set_key(key_cipher , key_str ,  key_len) ; 
    //     xtea_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (strcmp(algo , "redpike"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(RedpikeKey)) ;
        
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     redpike_set_key(key_cipher , key_str ,  key_len) ; 
    //     redpike_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (strcmp(algo , "rc4"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(Rc4Key)) ;
        
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     rc4_set_key(key_cipher , key_str ,  key_len) ; 
    //     rc4_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (strcmp(algo , "blowfish"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(BlowfishKey)) ;
        
        
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     blowfish_set_key(key_cipher , key_str ,  key_len) ; 
    //     blowfish_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (strcmp(algo , "affine"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(AffineKey)) ;
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     affine_set_key(key_cipher , key_str ,  key_len) ; 
    //     affine_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
        
    // }
    
    // else if (strcmp(algo , "hill"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(HillKey)) ;
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     hill_set_key(key_cipher , key_str ,  key_len) ; 
    //     hill_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 

        
    // }

    return 1 ; 
}


// return 0 in SUCCESS otherwise its positive integer with error printed in stderr 
int decrypt_image_file(char *name  , uchar_t *input_path,uchar_t* output_path, uchar_t * _key ,size_t _key_len) {


    printf("INFO: Testing %s... \n" , name  ) ; 
    printf("INFO: TYPE Image \n"   ) ; 

    
    int width, height, channels;
    char *algo_name = name ; 
    
    
    uchar_t *original_text = stbi_load(input_path, &width, &height, &channels, 0);
    if (original_text == NULL) {
        fprintf(stderr, "ERROR : couldnt extract pixels from original picture \n") ; 
        return 1 ;
    }
    
    int length = width * height * channels ; 
    
    // this is a lil bit risky yk ...

    if (length % 8 != 0   )
    {

        printf("INFO : doing a random padding *-*\n");
        int rest = length % 8 ;
        length += (8-rest);
    }
    
    uchar_t *decrypted_text = malloc(sizeof(uchar_t) * length );
    if (decrypted_text == NULL) {
        fprintf(stderr, "ERROR : malloc failed to allocate %ld bytes \n" , sizeof(uchar_t) * length) ; 
        stbi_image_free(original_text);    
        return 2 ;
    }

    
    int enc_result = handle_decryption(name ,_key , _key_len , original_text , decrypted_text , length ) ;
    if (enc_result != 0) {
        fprintf(stderr, "ERROR : handle for encryption failed (code %d)\n" , enc_result ) ;
        
        free(decrypted_text) ; 
        stbi_image_free(original_text);
        return 3; 
    }
    printf(  "INFO: encrypted with success\n" );
    
    int __res_funv_ = stbi_write_png(output_path, width, height, channels, decrypted_text, width * channels);
    if (__res_funv_ == 0) {
        fprintf(stderr, "ERROR : stbi write png failed \n") ; 

        free(decrypted_text) ; 
        stbi_image_free(original_text);

        return 4;
    }
    
    printf( "INFO: saved to %s (code %d ) \n" , output_path , __res_funv_  );

    
    free(decrypted_text);
    stbi_image_free(original_text);
    
    
    return  0;


}




int get_block_size(const char* cipher_name) {
    if (!cipher_name) return 0;
    if (strcmp(cipher_name, "aes") == 0)      return 16;
    if (strcmp(cipher_name, "des") == 0)      return 8;
    if (strcmp(cipher_name, "tdes") == 0)      return 8;
    if (strcmp(cipher_name, "blowfish") == 0) return 8;
    if (strcmp(cipher_name, "tea") == 0)      return 8;
    if (strcmp(cipher_name, "xtea") == 0)     return 8;
    if (strcmp(cipher_name, "redpike") == 0)     return 8;
    if (strcmp(cipher_name, "rc4") == 0)      return 1;
    // classical ciphers operate byte-wise → 1
    if (strcmp(cipher_name, "caesar") == 0)   return 1;
    if (strcmp(cipher_name, "vigenere") == 0) return 1;
    if (strcmp(cipher_name, "affine") == 0)   return 1;
    // unknown
    return 0;
}


