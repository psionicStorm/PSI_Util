#include "psi_hashing.h"
#include "psi_misc.h" 

void psi_get_64bit_sha256_with_seed(uint8_t * seed, uint8_t * element, uint64_t * ret_buffer, uint n) {
    uint8_t tmp[16];
    for (uint8_t i = 0; i < 16; i++)
        tmp[i] = element[i]^seed[i];
    psi_get_64bit_sha256(tmp, ret_buffer, n);
}

void psi_get_64bit_sha256(uint8_t * element, uint64_t * ret_buffer, uint n) {
    uint8_t hash_buffer[SHA256_DIGEST_LENGTH];
    uint64_t reducer[4];

    get_sha256(element, hash_buffer, n);

    for (uint8_t i = 0; i < 4; i++)
        reducer[i] = 0;

    for (uint8_t i = 0; i < 8; i++) {
        reducer[0] += hash_buffer[i];
        reducer[1] += hash_buffer[8 + i];
        reducer[2] += hash_buffer[16 + i];
        reducer[3] += hash_buffer[24 + i];

        if (i != 7) {
            reducer[0] <<= 8;
            reducer[1] <<= 8;
            reducer[2] <<= 8;
            reducer[3] <<= 8;
        }
    }

    reducer[0] ^= reducer[2];
    reducer[1] ^= reducer[3];

    *ret_buffer = reducer[0] ^ reducer[1];
}

void get_sha256(uint8_t * element, uint8_t digest[SHA256_DIGEST_LENGTH], uint n) {
    SHA256_CTX c;
    SHA256_Init(&c);
    SHA256_Update(&c, element, n);
    SHA256_Final(digest, &c);
}

void bytes_to_chars(uint8_t * byte_array, char * buffer, uint length) {
    if (length % 8 == 0 && length < 256) {
        for (uint8_t i = 0; i < length - 1; i += 8) {
            sprintf(buffer + i * 2, "%02x%02x%02x%02x%02x%02x%02x%02x",
                    byte_array[i], byte_array[i + 1], byte_array[i + 2],
                    byte_array[i + 3], byte_array[i + 4], byte_array[i + 5],
                    byte_array[i + 6], byte_array[i + 7]);
        }
    } else
        for (size_t i = 0; i < length; i++)
            sprintf(buffer + i * 2, "%02x", byte_array[i]);

    buffer[length * 2] = '\0';
}

void print_hash(uint8_t hash[32]) {
    for (uint8_t i = 0; i < 32; i++)
        printf("%02x", hash[i]);
    printf("\n");
}

void get_16_byte_sha256(uint8_t * elem, uint8_t * hash, uint n) {
    uint8_t buf[SHA256_DIGEST_LENGTH];
    get_sha256(elem, buf, n);
    for (int i = 0; i < 16; i++)
        hash[i] = buf[i]^buf[i + 16];
}

void get_10_byte_sha256(uint8_t * elem, uint8_t * hash, uint n) {
    uint8_t tmp[SHA256_DIGEST_LENGTH];
    get_16_byte_sha256(elem, tmp, n);
    xor10_elem(tmp, hash);
}

void get_10_byte_sha256_with_seed(uint8_t * seed, uint8_t * elem, uint8_t * hash, uint n) {
    for (size_t i = 0; i < 16; i++)
        elem[i] ^= seed[i];
    get_10_byte_sha256(elem, hash, n);
}