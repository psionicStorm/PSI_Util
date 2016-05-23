#include "psi_misc.h"

off_t fsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    fprintf(stderr, "Cannot determine size of %s: %s\n",
            filename, strerror(errno));

    return -1;
}

void get_bucket_path(gboolean a, char* buffer, char* folder, uint16_t n) {
    if (a)
        snprintf(buffer, 128, "%sa%u", folder, n);
    else
        snprintf(buffer, 128, "%sb%u", folder, n);
}

void psi_mkdir(char* path) {
    char buffer[128];
    snprintf(buffer, 128, "rm -rf \"%s\"", path);
    system(buffer);
    snprintf(buffer, 128, "mkdir -p \"%s\"", path);
    system(buffer);
}

void slice_alloc_byte_buffer(uint8_t *** buffer, size_t n1, size_t n2) {
    *buffer = (uint8_t**) malloc(sizeof (uint8_t*) * n1);
    for (size_t i = 0; i < n1; i++)
        (*buffer)[i] = g_slice_alloc(n2);
}

void slice_free_byte_buffer(uint8_t *** buffer, size_t n1, size_t n2) {
    for (size_t i = 0; i < n1; i++)
        g_slice_free1(n2, (*buffer)[i]);
    free(*buffer);
}

void slice_alloc_char_buffer(char *** buffer, size_t n1, size_t n2) {
    *buffer = (char**) malloc(sizeof (char*) * n1);
    for (size_t i = 0; i < n1; i++)
        (*buffer)[i] = g_slice_alloc(sizeof(char) * n2);
}

void slice_free_char_buffer(char *** buffer, size_t n1, size_t n2) {
    for (size_t i = 0; i < n1; i++)
        g_slice_free1(sizeof(char) * n2, (*buffer)[i]);
    free(*buffer);
}

/*Array to binary array*/
void atob(char* string, uint8_t* binary) {
    char byte[3];
    byte[2] = '\0';
    for (int i = 0; i < 16; i++) {
        byte[0] = string[i * 2];
        byte[1] = string[i * 2 + 1];
        binary[i] = strtol(byte, NULL, 16);
    }
}