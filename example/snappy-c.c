#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <snappy.h>

int main() {
    const char *input = "This is a test string for snappy compression.";
    size_t input_length = strlen(input);

    struct snappy_env env;
    snappy_init_env(&env);

    // Allocate max compressed length
    size_t max_compressed_length = snappy_max_compressed_length(input_length);
    char *compressed = malloc(max_compressed_length);
    size_t compressed_length = max_compressed_length;

    // Compress
    int result = snappy_compress(&env, input, input_length, compressed, &compressed_length);
    if (result != 0) {
        fprintf(stderr, "Compression failed with code %d\n", result);
        return 1;
    }

    printf("Compression successful! Compressed size: %zu bytes\n", compressed_length);

    // Allocate space for decompression
    char *decompressed = malloc(input_length + 1);  // +1 for null-terminator
    memset(decompressed, 0, input_length + 1);

    result = snappy_uncompress(compressed, compressed_length, decompressed);
    if (result != 0) {
        fprintf(stderr, "Decompression failed with code %d\n", result);
        return 1;
    }

    printf("Decompression successful! Output: %s\n", decompressed);

    // Clean up
    snappy_free_env(&env);
    free(compressed);
    free(decompressed);

    return 0;
}

