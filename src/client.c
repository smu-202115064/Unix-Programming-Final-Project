#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pipe.h"
#include "master.c"


void redirect_stdin(FILE *wfp) {
    char c;
    while (!feof(stdin)) {
        if (fread(&c, sizeof(char), 1, stdin) == 0) {
            perror("[client] reading from stdin");
            break;
        }
        pipe_write(wfp, c);
    }
}


void run_client() {
    pid_t pid;
    FILE *fps[2];

    printf("[main] start process with pid %d...\n", getpid());
    pipe_create(fps);

    switch (pid = fork()) {
        case -1: /* fork failed */
            perror("[main] fork master has failed");
            exit(EXIT_FAILURE);
            break;
        case 0: /* master node */
            fclose(fps[1]);
            run_master(fps[0]);
            fclose(fps[0]);
            exit(EXIT_SUCCESS);
            break;
        default: /* client */
            fclose(fps[0]);
            while (true) {
                redirect_stdin(fps[1]);
            }
            fclose(fps[1]);
            exit(EXIT_SUCCESS);
            break;
    }
}


int main() {
    run_client();
}
