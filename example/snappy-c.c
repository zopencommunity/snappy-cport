#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "snappy.h"

int main() {
    const char *input = "This is a test string to be compressed using snappy.";
    size_t input_length = strlen(input);

    // Step 1: Init env
    struct snappy_env env;
    if (snappy_init_env(&env) != 0) {
        fprintf(stderr, "Failed to initialize snappy_env\n");
        return 1;
    }

    // Step 2: Allocate buffer for compression
    size_t max_compressed_length = snappy_max_compressed_length(input_length);
    char *compressed = (char *)malloc(max_compressed_length);
    if (!compressed) {
        fprintf(stderr, "Failed to allocate compressed buffer\n");
        snappy_free_env(&env);
        return 1;
    }

    size_t compressed_length = max_compressed_length;

    // Step 3: Compress
    if (snappy_compress(&env, input, input_length, compressed, &compressed_length) != 0) {
        fprintf(stderr, "Compression failed\n");
        free(compressed);
        snappy_free_env(&env);
        return 1;
    }

    printf("Original: %s\n", input);
    printf("Compressed size: %zu bytes\n", compressed_length);

    // Step 4: Prepare buffer for uncompressed data
    char *uncompressed = (char *)malloc(input_length + 1); // +1 for null terminator
    if (!uncompressed) {
        fprintf(stderr, "Failed to allocate uncompressed buffer\n");
        free(compressed);
        snappy_free_env(&env);
        return 1;
    }

    // Step 5: Uncompress
    if (snappy_uncompress(compressed, compressed_length, uncompressed) != 0) {
        fprintf(stderr, "Uncompression failed\n");
        free(compressed);
        free(uncompressed);
        snappy_free_env(&env);
        return 1;
    }

    // Null-terminate for printing
    uncompressed[input_length] = '\0';

    printf("Uncompressed: %s\n", uncompressed);

    // Step 6: Cleanup
    free(compressed);
    free(uncompressed);
    snappy_free_env(&env);

    return 0;
}

