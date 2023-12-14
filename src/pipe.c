#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pipe.h"


void pipe_create(FILE *fp[2]) {
    int fd[2];

    if (pipe(fd) == -1) {
        perror("[pipe] create failed");
        exit(EXIT_FAILURE);
        return;
    }

    fp[0] = fdopen(fd[0], "r");
    if (fp[0] == NULL) {
        perror("[pipe] fdopen(r) failed");
        exit(EXIT_FAILURE);
        return;
    }

    fp[1] = fdopen(fd[1], "w");
    if (fp[1] == NULL) {
        perror("[pipe] fdopen(r) failed");
        exit(EXIT_FAILURE);
        return;
    }
}

void pipe_readline(FILE *rfp, char *buf, const size_t bufsize) {
    int length;
    for (length = 0; !feof(rfp) && length < bufsize; length++) {
        if (fread(buf+length, sizeof(char), 1, rfp) == 0) {
            perror("[io] reading failed");
            exit(EXIT_FAILURE);
            return;
        }
        if (buf[length] == '\n') {
            break;
        }
    }
    buf[length] = '\0';
}


void pipe_write(FILE *wfp, const char c) {
    if (fwrite(&c, sizeof(char), 1, wfp) == 0) {
        perror("[io] writing failed");
        exit(EXIT_FAILURE);
        return;
    }

    if (fflush(wfp) == EOF) {
        perror("[io] flush failed");
        exit(EXIT_FAILURE);
        return;
    }
}