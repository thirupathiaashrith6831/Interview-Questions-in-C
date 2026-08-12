#include <stdio.h>
#include <stdint.h>
#include <string.h>

static uint32_t crc32_table[256];

void init_crc32_table(void) {
    uint32_t polynomial = 0xEDB88320;
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i;
        for (int j = 0; j < 8; j++) {
            if (c & 1) {
                c = polynomial ^ (c >> 1);
            } else {
                c >>= 1;
            }
        }
        crc32_table[i] = c;
    }
}

uint32_t calculate_crc32(const uint8_t *data, size_t length) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; i++) {
        uint8_t table_index = (crc ^ data[i]) & 0xFF;
        crc = (crc >> 8) ^ crc32_table[table_index];
    }
    return crc ^ 0xFFFFFFFF;
}

int main(void) {
    init_crc32_table();
    const char *text = "The quick brown fox jumps over the lazy dog";
    uint32_t checksum = calculate_crc32((const uint8_t *)text, strlen(text));

    printf("Input    : \"%s\"\n", text);
    printf("CRC32    : 0x%08X\n", checksum);
    return 0;
}
