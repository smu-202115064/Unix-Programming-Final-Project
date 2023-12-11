#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "client.c"
#include "node/master.c"


int main() {
    pid_t pid;
    int fd[2];
    FILE *fp;
    int n;
    char c;

    printf("[main] start process with pid %d...\n", getpid());

    if (pipe(fd) == -1) {
        perror("[main] creating pipe has failed");
        exit(1);
    }

    switch (pid = fork()) {
        case -1: /* fork failed */
            perror("[main] fork master has failed");
            exit(1);
            break;
        case 0: /* master node */
            close(fd[1]);
            fp = fdopen(fd[0], "r");
            if (fp == NULL) {
                perror("[main] fdopen failed");
                exit(1);
            }
            run_master(fp);
            break;
        default: /* client */
            close(fd[0]);
            fp = fdopen(fd[1], "w");
            if (fp == NULL) {
                perror("[main] fdopen failed");
                exit(1);
            }
            run_client(fp);
            break;
    }

    fclose(fp);
    printf("[main] exit process.\n");
    return 0;
}
