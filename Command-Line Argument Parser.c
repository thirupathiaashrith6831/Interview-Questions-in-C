#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    bool verbose;
    bool help;
    int port;
    char *filename;
} Config;

void parse_args(int argc, char *argv[], Config *config) {
    config->verbose = false;
    config->help = false;
    config->port = 80;
    config->filename = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            config->verbose = true;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            config->help = true;
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            config->port = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            config->filename = argv[++i];
        }
    }
}

int main(int argc, char *argv[]) {
    // Simulated command line vector for testing
    char *mock_argv[] = {"app", "-v", "-p", "8080", "-f", "data.txt"};
    int mock_argc = 6;

    Config config;
    parse_args(mock_argc, mock_argv, &config);

    printf("--- CLI Argument Parser Results ---\n");
    printf("Verbose Mode : %s\n", config.verbose ? "ENABLED" : "DISABLED");
    printf("Target Port  : %d\n", config.port);
    printf("Input File   : %s\n", config.filename ? config.filename : "NONE");

    return 0;
}
