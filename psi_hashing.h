/* 
 * File:   psi_hashing.h
 * Author: Oleksandr Tkachenko
 */

#ifndef PSI_HASHING_H 
#define PSI_HASHING_H 

#include <glib-2.0/glib.h> 
#include <stdint.h> 
#include <openssl/sha.h> 
#include <string.h> 
#include <stdio.h> 
#include <inttypes.h> 
#include <stdint.h> 

#ifdef __cplusplus 
extern "C" {
#endif 

    void psi_get_64bit_sha256_with_seed(uint8_t seed[16], uint8_t element[16], uint64_t * ret_buffer);
    void psi_get_64bit_sha256(uint8_t element[16], uint64_t * buffer);
    void get_sha256(uint8_t element[16], uint8_t digest[SHA256_DIGEST_LENGTH]);
    void bytes_to_chars(uint8_t * byte_array, char * buffer, uint length);
    void print_hash(uint8_t hash[32]);
    void get_16_bit_sha256(uint8_t * elem, uint8_t * hash);


#ifdef __cplusplus 
}
#endif 

#endif /* PSI_HASHING_H */

