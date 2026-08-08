#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned char value;
    unsigned char count;
} RLEChunk;

int rle_compress(const unsigned char *src, int src_len, RLEChunk *dest) {
    if (src_len == 0) return 0;

    int out_idx = 0;
    for (int i = 0; i < src_len; ) {
        unsigned char current = src[i];
        unsigned char count = 1;

        // Count consecutive identical bytes up to 255
        while (i + count < src_len && src[i + count] == current && count < 255) {
            count++;
        }

        dest[out_idx].value = current;
        dest[out_idx].count = count;
        out_idx++;
        i += count;
    }
    return out_idx;
}

int rle_decompress(const RLEChunk *src, int chunk_count, unsigned char *dest) {
    int out_idx = 0;
    for (int i = 0; i < chunk_count; i++) {
        for (int j = 0; j < src[i].count; j++) {
            dest[out_idx++] = src[i].value;
        }
    }
    return out_idx;
}

int main(void) {
    const unsigned char original[] = "AAAAABBBCCDAAAAAAAEEEEEEEEE";
    int orig_len = sizeof(original) - 1;

    RLEChunk compressed[100];
    int chunk_count = rle_compress(original, orig_len, compressed);

    printf("Original String  : %s (%d bytes)\n", original, orig_len);
    printf("Compressed Units : %d chunks (%zu bytes total)\n", 
           chunk_count, chunk_count * sizeof(RLEChunk));

    printf("Compressed Output: ");
    for (int i = 0; i < chunk_count; i++) {
        printf("[%c:%d] ", compressed[i].value, compressed[i].count);
    }
    printf("\n");

    unsigned char decompressed[100] = {0};
    rle_decompress(compressed, chunk_count, decompressed);
    printf("Decompressed Text: %s\n", decompressed);

    return 0;
}
