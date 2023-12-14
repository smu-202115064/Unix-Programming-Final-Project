#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "queue.c"
#include "socket_2.c"
#include "socket.h"

#define NUMBER_OF_WORKERS (3)


void run_master(FILE *rfp) {
    pid_t pid;
    char buf[BUFSIZ];
    t_queue *worker_pid_queue;

    printf("[master] start process with pid %d...\n", getpid());

    worker_pid_queue = queue_create();

    // Create worker nodes
    for (int worker_id = 0; worker_id < NUMBER_OF_WORKERS; worker_id++) {
        switch (pid = fork()) {
            case -1: /* fork failed */
                perror("[master] fork worker has failed");
                exit(1);
                break;
            case 0: /* worker node */
                run_worker(worker_id);
                return;
            default: /* master node */
                printf("[master] pid %d created worker %d with pid: %d\n", getpid(), worker_id, pid);
                queue_push(worker_pid_queue, pid);
                break;
        }
    }
    // Receive data from client
    printf("[master] listening client\n");
    for (int length = 0; !feof(rfp); length++) {
        if (fread(buf+length, sizeof(char), 1, rfp) == 0) {
            perror("[master] reading from pipe");
            break;
        }

        if (buf[length] == '\n') {
            buf[length] = '\0';
            printf("[master] received string: %s\n", buf);
            // TODO: Send data to a worker not busy.
            length = -1; // 다음 루프에서 length = 0이 되도록 설정.
        }
    }


    int sock_fd;
    struct sockaddr_un sock_addr;

    fclose(rfp);
    printf("[master] exit process.\n");
    exit(1);
    return;
}
