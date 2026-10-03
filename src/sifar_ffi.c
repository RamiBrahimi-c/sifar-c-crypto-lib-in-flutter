#include "sifar_ffi.h"
#include "ciphers/symmetric/aes.h"
#include "ciphers/symmetric/des.h"
#include "ciphers/symmetric/3des.h"
#include "ciphers/symmetric/blowfish.h"
#include "ciphers/symmetric/rc4.h"
#include "ciphers/symmetric/redpike.h"
#include "ciphers/symmetric/tea.h"
#include "ciphers/symmetric/xtea.h"
#include "ciphers/classical/affine.h"
#include "ciphers/classical/hill.h"
#include "ciphers/hashing/hash.h"
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
    else if (strcmp(algo , "des")==0)
    {
        void *key_cipher = (void*) calloc(1 , sizeof(DesKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(DesKey) ) ; 
            return 1;
        }
        des_set_key(key_cipher , key_str ,  key_len) ; 
        des_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "tea")==0)
    {
        void *key_cipher = (void*) calloc(1 , sizeof(TeaKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(TeaKey) ) ; 
            return 1;
        }
        tea_set_key(key_cipher , key_str ,  key_len) ; 
        tea_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }

    else if (strcmp(algo , "xtea")==0)
    {
        void *key_cipher = (void*) calloc(1 , sizeof(XTeaKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(XTeaKey) ) ; 
            return 1;
        }        
        xtea_set_key(key_cipher , key_str ,  key_len) ; 
        xtea_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "redpike")==0)
    {
        void *key_cipher = (void*) calloc(1 , sizeof(RedpikeKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(RedpikeKey) ) ; 
            return 1;
        }        
        redpike_set_key(key_cipher , key_str ,  key_len) ; 
        redpike_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "rc4")==0)
    {
        void *key_cipher = (void*) calloc(1 , sizeof(Rc4Key)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(Rc4Key) ) ; 
            return 1;
        }        
        rc4_set_key(key_cipher , key_str ,  key_len) ; 
        rc4_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "blowfish")==0)
    {
        void *key_cipher = (void*) calloc(1 , sizeof(BlowfishKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(BlowfishKey) ) ; 
            return 1;
        }        
        
        blowfish_set_key(key_cipher , key_str ,  key_len) ; 
        blowfish_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "affine")==0)
    {
        void *key_cipher = (void*) calloc(1 , sizeof(AffineKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(AffineKey) ) ; 
            return 1;
        }        
        affine_set_key(key_cipher , key_str ,  key_len) ; 
        affine_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
        
    }
    
    else if (strcmp(algo , "hill"))
    {
        void *key_cipher = (void*) calloc(1 , sizeof(HillKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(HillKey) ) ; 
            return 1;
        }
        hill_set_key(key_cipher , key_str ,  key_len) ; 
        hill_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 

        
    }

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

    else if (strcmp(algo , "des")==0)
    {
        void *key_cipher = (void*) calloc(1 , sizeof(DesKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(DesKey) ) ; 
            return 1;
        }
        des_set_key(key_cipher , key_str ,  key_len) ; 
        des_decrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "tea")==0)
    {
        void *key_cipher = (void*) calloc(1 , sizeof(TeaKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(TeaKey) ) ; 
            return 1;
        }
        tea_set_key(key_cipher , key_str ,  key_len) ; 
        tea_decrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }

    else if (strcmp(algo , "xtea")==0)
    {
        void *key_cipher = (void*) calloc(1 , sizeof(XTeaKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(XTeaKey) ) ; 
            return 1;
        }        
        xtea_set_key(key_cipher , key_str ,  key_len) ; 
        xtea_decrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "redpike")==0)
    {
        void *key_cipher = (void*) calloc(1 , sizeof(RedpikeKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(RedpikeKey) ) ; 
            return 1;
        }        
        redpike_set_key(key_cipher , key_str ,  key_len) ; 
        redpike_decrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "rc4")==0)
    {
        void *key_cipher = (void*) calloc(1 , sizeof(Rc4Key)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(Rc4Key) ) ; 
            return 1;
        }        
        rc4_set_key(key_cipher , key_str ,  key_len) ; 
        rc4_decrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "blowfish")==0)
    {
        void *key_cipher = (void*) calloc(1 , sizeof(BlowfishKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(BlowfishKey) ) ; 
            return 1;
        }        
        
        blowfish_set_key(key_cipher , key_str ,  key_len) ; 
        blowfish_decrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "affine")==0)
    {
        void *key_cipher = (void*) calloc(1 , sizeof(AffineKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(AffineKey) ) ; 
            return 1;
        }        
        affine_set_key(key_cipher , key_str ,  key_len) ; 
        affine_decrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
        
    }
    
    else if (strcmp(algo , "hill"))
    {
        void *key_cipher = (void*) calloc(1 , sizeof(HillKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(HillKey) ) ; 
            return 1;
        }
        hill_set_key(key_cipher , key_str ,  key_len) ; 
        hill_decrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 

        
    }


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
    if (strcmp(cipher_name, "3des") == 0)      return 8;
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



// returns required key size, or 0 if variable/unknown
int get_key_size(const char* cipher_name) {
    if (!cipher_name) return 0;
    if (strcmp(cipher_name, "aes") == 0)      return 16;   // 16/24/32; we default to 16
    if (strcmp(cipher_name, "des") == 0)      return 8;
    if (strcmp(cipher_name, "3des") == 0)     return 24;
    if (strcmp(cipher_name, "blowfish") == 0) return -1;   // variable 1-56
    if (strcmp(cipher_name, "rc4") == 0)      return -1;   // variable 1-256
    if (strcmp(cipher_name, "tea") == 0)      return 16;
    if (strcmp(cipher_name, "xtea") == 0)     return 16;
    if (strcmp(cipher_name, "redpike") == 0)  return 8; // matches redpike_set_key
    return 0;
}



int get_digest_size(const char* hash_name) {
    if (!hash_name) return 0;
    if (strcmp(hash_name, "md4")    == 0) return 16;
    if (strcmp(hash_name, "md5")    == 0) return 16;
    if (strcmp(hash_name, "sha256") == 0) return 32;
    if (strcmp(hash_name, "sha512") == 0) return 64;
    return 0;
}



int cipher_encrypt_mode(const char* cipher, const char* mode,
                        const uchar_t* in, uchar_t* out, const uchar_t* iv,
                        int length, const void* key) {
    if (strcmp(cipher, "aes") == 0) {
        if (strcmp(mode, "ecb") == 0) return aes_encrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return aes_encrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return aes_encrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return aes_encrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return aes_encrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "des") == 0) {
        if (strcmp(mode, "ecb") == 0) return des_encrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return des_encrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return des_encrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return des_encrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return des_encrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "3des") == 0) {
        if (strcmp(mode, "ecb") == 0) return tdes_encrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return tdes_encrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return tdes_encrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return tdes_encrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return tdes_encrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "blowfish") == 0) {
        if (strcmp(mode, "ecb") == 0) return blowfish_encrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return blowfish_encrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return blowfish_encrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return blowfish_encrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return blowfish_encrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "redpike") == 0) {
        if (strcmp(mode, "ecb") == 0) return redpike_encrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return redpike_encrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return redpike_encrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return redpike_encrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return redpike_encrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "tea") == 0) {
        if (strcmp(mode, "ecb") == 0) return tea_encrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return tea_encrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return tea_encrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return tea_encrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return tea_encrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "xtea") == 0) {
        if (strcmp(mode, "ecb") == 0) return xtea_encrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return xtea_encrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return xtea_encrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return xtea_encrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return xtea_encrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "rc4") == 0) {
        return rc4_encrypt(in, out, length, key);
    }

    return -1;
}


int cipher_decrypt_mode(const char* cipher, const char* mode,
                        const uchar_t* in, uchar_t* out, const uchar_t* iv,
                        int length, const void* key) {
    if (strcmp(cipher, "aes") == 0) {
        if (strcmp(mode, "ecb") == 0) return aes_decrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return aes_decrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return aes_decrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return aes_decrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return aes_decrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "des") == 0) {
        if (strcmp(mode, "ecb") == 0) return des_decrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return des_decrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return des_decrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return des_decrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return des_decrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "3des") == 0) {
        if (strcmp(mode, "ecb") == 0) return tdes_decrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return tdes_decrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return tdes_decrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return tdes_decrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return tdes_decrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "blowfish") == 0) {
        if (strcmp(mode, "ecb") == 0) return blowfish_decrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return blowfish_decrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return blowfish_decrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return blowfish_decrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return blowfish_decrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "redpike") == 0) {
        if (strcmp(mode, "ecb") == 0) return redpike_decrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return redpike_decrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return redpike_decrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return redpike_decrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return redpike_decrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "tea") == 0) {
        if (strcmp(mode, "ecb") == 0) return tea_decrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return tea_decrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return tea_decrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return tea_decrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return tea_decrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "xtea") == 0) {
        if (strcmp(mode, "ecb") == 0) return xtea_decrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return xtea_decrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return xtea_decrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return xtea_decrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return xtea_decrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "rc4") == 0) {
        return rc4_decrypt(in, out, length, key);
    }

    return -1;
}

int cipher_decrypt_mode(
    const char* cipher, const char* mode,
    const uchar_t* input, uchar_t* output, const uchar_t* iv,
    int length, const void* key);



int has_iv(const char* mode) {
    // returns 1 if the mode needs an IV, 0 otherwise
    return (strcmp(mode, "cbc") == 0 || strcmp(mode, "cfb") == 0 ||
            strcmp(mode, "ofb") == 0 || strcmp(mode, "ctr") == 0);
}


