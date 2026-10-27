#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define LOG_MAGIC 0xA010A010

typedef struct {
    uint32_t magic;
    uint32_t length;
    uint32_t checksum;
} LogHeader;

typedef struct {
    FILE *file;
    char filepath[256];
} AppendOnlyLog;

uint32_t calculate_checksum(const uint8_t *data, uint32_t len) {
    uint32_t sum = 0;
    for (uint32_t i = 0; i < len; i++) {
        sum = (sum * 31) + data[i];
    }
    return sum;
}

AppendOnlyLog* aol_open(const char *path) {
    AppendOnlyLog *log = malloc(sizeof(AppendOnlyLog));
    strncpy(log->filepath, path, sizeof(log->filepath));
    log->file = fopen(path, "w+b"); // Binary read/write mode for simulation
    return log;
}

bool aol_append(AppendOnlyLog *log, const uint8_t *data, uint32_t len) {
    if (!log->file) return false;

    LogHeader header;
    header.magic = LOG_MAGIC;
    header.length = len;
    header.checksum = calculate_checksum(data, len);

    fseek(log->file, 0, SEEK_END);
    fwrite(&header, sizeof(LogHeader), 1, log->file);
    fwrite(data, 1, len, log->file);
    fflush(log->file);
    return true;
}

bool aol_recover_and_verify(AppendOnlyLog *log) {
    fseek(log->file, 0, SEEK_SET);
    LogHeader header;
    int entry_count = 0;

    printf("Starting AOL Recovery Scan:\n");
    while (fread(&header, sizeof(LogHeader), 1, log->file) == 1) {
        if (header.magic != LOG_MAGIC) {
            printf("  [Error] Invalid Magic Number! Corrupted log.\n");
            return false;
        }

        uint8_t *buffer = malloc(header.length);
        fread(buffer, 1, header.length, log->file);

        uint32_t check = calculate_checksum(buffer, header.length);
        if (check != header.checksum) {
            printf("  [Error] Checksum mismatch in entry %d!\n", entry_count);
            free(buffer);
            return false;
        }

        printf("  [Valid] Entry #%d | Size: %u bytes | Data: \"%.*s\"\n", 
               entry_count++, header.length, header.length, buffer);
        free(buffer);
    }
    return true;
}

void aol_close(AppendOnlyLog *log) {
    if (log->file) fclose(log->file);
    free(log);
}

int main(void) {
    const char *path = "test_simulation.log";
    AppendOnlyLog *log = aol_open(path);

    printf("--- Append-Only Log (AOL) Engine ---\n");
    const char *msg1 = "SET user:101 Alice";
    const char *msg2 = "SET user:102 Bob";
    const char *msg3 = "DELETE user:100";

    aol_append(log, (const uint8_t*)msg1, strlen(msg1));
    aol_append(log, (const uint8_t*)msg2, strlen(msg2));
    aol_append(log, (const uint8_t*)msg3, strlen(msg3));

    aol_recover_and_verify(log);

    aol_close(log);
    remove(path); // Cleanup
    return 0;
}
