#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void run_client(FILE *wfp) {
    int n;
    char c;

    printf("[client] start process with pid %d...\n", getpid());

    while (!feof(stdin)) {
        if (fread(&c, sizeof(char), 1, stdin) == 0) {
            perror("[client] reading from stdin");
            break;
        }

        if (fwrite(&c, sizeof(char), 1, wfp) == 0) {
            perror("[client] writing to pipe");
            break;
        }

        if (fflush(wfp) == EOF) {
            perror("[client] flush pipe");
            break;
        }
    }

    fclose(wfp);
    printf("[client] exit process.\n");
    exit(1);
    return;
}
