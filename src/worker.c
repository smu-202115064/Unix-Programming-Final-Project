#include <sys/un.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "socket.h"
#include "worker.h"


void run_worker(const int worker_id) {
    int sock_fd;
    struct sockaddr_un *sock_addr;
    char *buf;
    size_t bufsize;

    printf("[worker#%d] allocating memory...\n", worker_id);
    sock_addr = (struct sockaddr_un *) malloc(sizeof(struct sockaddr_un));

    bufsize = BUFSIZ;
    buf = (char *) malloc(bufsize);

    printf("[worker#%d] initializing...\n", worker_id);
    socket_init_fd(&sock_fd);
    socket_init_addr(sock_addr);
    socket_connect(sock_fd, sock_addr);

    while (true) {
        int n = read(sock_fd, buf, bufsize-1);
        buf[n] = '\0';
        printf("[worker#%d] received data \"%s\"\n", worker_id, buf);

        if (n == 0) {
            break;
        }

        process_data(buf);
        printf("[worker#%d] processed data %s\n", worker_id, buf);

        if(send(sock_fd, buf, strlen(buf), 0) != strlen(buf) ) {
            perror("[worker] socket send failed");
            exit(EXIT_FAILURE);
            return;
        }
    }

    printf("[worker#%d] disconnected from server\n", worker_id);
    free(sock_addr);
    free(buf);
    exit(EXIT_FAILURE);
}


void process_data(char *str) {
    // Convert to uppercase
    while (*str != '\0') {
        if (*str >= 'a' && *str <= 'z') {
            *str = *str + 'A' - 'a';
        }
        str++;
    }
    sleep(VIRTUAL_PROCESS_TIME);
}
