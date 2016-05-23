#include "psi_hashing.h"

void psi_get_64bit_sha256_with_seed(uint8_t seed[16], uint8_t element[16], uint64_t * ret_buffer) {
    uint8_t tmp[16];
    for (uint8_t i = 0; i < 16; i++)
        tmp[i] = element[i]^seed[i];
    psi_get_64bit_sha256(tmp, ret_buffer);
}

void psi_get_64bit_sha256(uint8_t element[16], uint64_t * ret_buffer) {
    uint8_t hash_buffer[SHA256_DIGEST_LENGTH];
    uint64_t reducer[4];

    get_sha256(element, hash_buffer);

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

void get_sha256(uint8_t element[16], uint8_t digest[SHA256_DIGEST_LENGTH]) {
    SHA256_CTX c;
    SHA256_Init(&c);
    SHA256_Update(&c, element, 16);
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