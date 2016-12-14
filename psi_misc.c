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
    if (*buffer = NULL)
        perror("Error by malloc\n");
    for (size_t i = 0; i < n1; i++)
        (*buffer)[i] = g_slice_alloc(n2);
}

uint8_t ** slice_alloc_byte_buffer_new(size_t n1, size_t n2) {
    uint8_t ** buffer = (uint8_t**) malloc(sizeof (*buffer) * n1);
    if (buffer = NULL)
        perror("Error by malloc\n");
    for (size_t i = 0; i < n1; i++)
        buffer[i] = g_slice_alloc(n2);
    return buffer;
}

void slice_free_byte_buffer(uint8_t *** buffer, size_t n1, size_t n2) {
    for (size_t i = 0; i < n1; i++)
        g_slice_free1(n2, (*buffer)[i]);
    free(*buffer);
}

void slice_alloc_char_buffer(char *** buffer, size_t n1, size_t n2) {
    *buffer = (char**) malloc(sizeof (char*) * n1);
    if (*buffer = NULL)
        printf("Error by malloc\n");
    for (size_t i = 0; i < n1; i++)
        (*buffer)[i] = g_slice_alloc(sizeof (char) * n2);
}

void slice_free_char_buffer(char *** buffer, size_t n1, size_t n2) {
    for (size_t i = 0; i < n1; i++)
        g_slice_free1(sizeof (char) * n2, (*buffer)[i]);
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

FILE * psi_try_fopen(char * path, char * settings) {
    FILE * f;
    if (path == NULL || settings == NULL) {
        printf("Trying to fopen NULL\n");
        exit(EXIT_FAILURE);
    } else if (strlen(path) < 1 || strlen(settings) < 1)
        printf("Trying to fopen empty string\n");
    else
        f = fopen(path, settings);
    if (f != NULL)
        return f;
    printf("Error opening %s %s\n", path, settings);
    exit(EXIT_FAILURE);
}

void print_byte_buf(uint8_t * buf, size_t n) {
    for (size_t i = 0; i < n; i++)printf("%02x", buf[i]);
    printf("\n");
}

void psi_reduce_elems_16_to_10_bytes(char * path, PSI_Hashing_Module module) {
    size_t init_extra, extra, init_size, reduced_size, elems = 0, bins = 0;
    FILE * f = psi_try_fopen(path, "rb");
    char path_reduced[strlen(path) + 9];
    sprintf(path_reduced, "%s_reduced", path);
    FILE * f_reduced = psi_try_fopen(path_reduced, "wb");

    if (module == PSI_Cuckoo_Hashing) {
        init_extra = 3;
        extra = 0;
    } else {
        init_extra = extra = 1;
    }

    init_size = 16 + init_extra;
    reduced_size = 10 + extra;

    printf("Element reduction\n");
    printf("Old element : %zu + %zu B\n", init_size - init_extra, init_extra);
    printf("New element : %zu + %zu B\n", reduced_size - extra, extra);

    elems = 0;
    bins = 0;
    uint8_t read_buf[init_size];
    while (fread(read_buf, init_size, 1, f) > 0) {
        uint8_t write_buf[reduced_size];
        xor10_elem(read_buf, write_buf);
        memcpy(write_buf + reduced_size - extra,
                read_buf + init_size - init_extra, extra);
        fwrite(write_buf, reduced_size, 1, f_reduced);
        elems += init_size;
        if (module == PSI_Simple_Hashing && read_buf[init_size - 1]&0x80)
            bins++;
        else if (module == PSI_Cuckoo_Hashing)
            bins++;
        if (module == PSI_Simple_Hashing && read_buf[init_size - 1] !=
                write_buf[reduced_size - 1])
            printf("Error reducing elements : Wrong bin flag\n");
    }

    printf("Reduced %zu elements within %zu bins\n", elems / init_size, bins);
}

//16=>10

void xor10_elem(uint8_t * get, uint8_t * put) {
    put[0] = get[0];
    put[1] = get[1] ^ get[2];
    put[2] = get[3];
    put[3] = get[4] ^ get[5];
    put[4] = get[6] ^ get[7];
    put[5] = get[8];
    put[6] = get[9] ^ get[10];
    put[7] = get[11];
    put[8] = get[12] ^ get[13];
    put[9] = get[14] ^ get[15];
}