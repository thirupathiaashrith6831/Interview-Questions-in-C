#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    uint8_t *buffer;
    size_t capacity;
    size_t offset;
} SerializerBuffer;

void sb_init(SerializerBuffer *sb, uint8_t *buf, size_t cap) {
    sb->buffer = buf;
    sb->capacity = cap;
    sb->offset = 0;
}

bool sb_write_int32(SerializerBuffer *sb, int32_t val) {
    if (sb->offset + sizeof(int32_t) > sb->capacity) return false;
    // Little-endian serialization
    sb->buffer[sb->offset++] = (val >> 0) & 0xFF;
    sb->buffer[sb->offset++] = (val >> 8) & 0xFF;
    sb->buffer[sb->offset++] = (val >> 16) & 0xFF;
    sb->buffer[sb->offset++] = (val >> 24) & 0xFF;
    return true;
}

bool sb_read_int32(SerializerBuffer *sb, int32_t *val) {
    if (sb->offset + sizeof(int32_t) > sb->capacity) return false;
    *val = (int32_t)(
        ((uint32_t)sb->buffer[sb->offset + 0] << 0) |
        ((uint32_t)sb->buffer[sb->offset + 1] << 8) |
        ((uint32_t)sb->buffer[sb->offset + 2] << 16) |
        ((uint32_t)sb->buffer[sb->offset + 3] << 24)
    );
    sb->offset += 4;
    return true;
}

int main(void) {
    uint8_t raw_storage[64];
    SerializerBuffer writer;
    sb_init(&writer, raw_storage, sizeof(raw_storage));

    printf("--- Zero-Copy Binary Serialization Buffer ---\n");
    sb_write_int32(&writer, 123456789);
    sb_write_int32(&writer, -42);

    printf("Serialized %zu bytes into buffer.\n", writer.offset);

    // Reset offset for zero-copy reading
    SerializerBuffer reader;
    sb_init(&reader, raw_storage, sizeof(raw_storage));

    int32_t v1, v2;
    sb_read_int32(&reader, &v1);
    sb_read_int32(&reader, &v2);

    printf("Deserialized Value 1: %d\n", v1);
    printf("Deserialized Value 2: %d\n", v2);

    return 0;
}
