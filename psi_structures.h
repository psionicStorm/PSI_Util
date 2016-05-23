/* 
 * File:   psi_structures.h
 * Author: Oleksandr Tkachenko <oleksandr.tkachenko at crisp-da.de>
 *
 * Created on May 4, 2016, 9:53 PM
 */

#ifndef PSI_STRUCTURES_H
#define PSI_STRUCTURES_H

#include <glib-2.0/glib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct PSI_SIMPLE_INTERSECTION_CTX {
        char path_a[128];
        char path_b[128];
        char ** buffer;
        off_t fsize[2];
        GSList * result;
    } PSI_SIMPLE_INTERSECTION_CTX;

    typedef struct PSI_INTERSECTION_CTX {
        char path_a[128];
        char path_b[128];
        char path_root[128];
        char path_result[128];
        char path_folder_buckets[128];
        gboolean bucketing;
        size_t bucket_n;
        uint8_t ** buffer;
        size_t read_buffer_size;
        size_t queue_buffer_size;
        struct PSI_Queue ** queues;
        GSList * result;
        uint8_t element_pow;
        uint8_t threads;
    } PSI_INTERSECTION_CTX;

    typedef struct PSI_SIMPLE_HASHING_CTX {
        char path_root[128];
        char path_source[128];
        char path_buckets[128];
        size_t read_buffer_size;
        size_t bucket_n;
        size_t queue_buffer_size;
        struct PSI_Queue ** queues;
        uint8_t element_pow;
        uint64_t divisor;
        off_t size_source;
        uint8_t seed[3][16];
        uint8_t hash_n;
        uint8_t threads;
    } PSI_SIMPLE_HASHING_CTX;

    typedef struct PSI_CUCKOO_HASHING_CTX {
        char path_source[128];
        char path_dest[128];
        char path_stash[128];
        uint8_t element_pow;
        double d_mult_size_table;
        size_t size_table;
        struct PSI_Cuckoo_list * l;
        uint64_t divisor;
        off_t size_source;
        uint8_t seed[3][16];
        uint8_t hash_n;
        size_t rec_limit;
        size_t read_buffer_size;
    } PSI_CUCKOO_HASHING_CTX;

    typedef struct PSI_Queue {
        uint8_t ** buffer;
        uint counter;
    } PSI_Queue;

    typedef struct PSI_Cuckoo_element {
        struct PSI_Cuckoo_element * next;
        struct PSI_Cuckoo_element * prev;
        uint8_t * buffer;
        uint64_t hash_val;
    } PSI_Cuckoo_element;

    typedef struct PSI_Cuckoo_list {
        PSI_Cuckoo_element * root;
        size_t size;
    } PSI_Cuckoo_list;

#ifdef __cplusplus
}
#endif

#endif /* PSI_STRUCTURES_H */

