#include <sys/un.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "socket.h"


#define VIRTUAL_PROCESS_TIME (1)


void process_data(char *data);


void run_worker() {
    int sock_fd;
    struct sockaddr_un *sock_addr;
    char *buf;
    size_t bufsize;

    sock_addr = (struct sockaddr_un *) malloc(sizeof(struct sockaddr_un));

    bufsize = BUFSIZ;
    buf = (char *) malloc(bufsize);

    socket_init_fd(&sock_fd);
    socket_init_addr(sock_addr);

    socket_connect(sock_fd, sock_addr);

    while (true) {
        int n = read(sock_fd, buf, bufsize-1);
        buf[n] = '\0';

        if (n == 0) {
            printf("[worker] disconnected from server\n");
            exit(EXIT_FAILURE);
            return;
        }

        process_data(buf);

        if(send(sock_fd, buf, strlen(buf), 0) != strlen(buf) ) {
            perror("[worker] socket send failed");
            exit(EXIT_FAILURE);
            return;
        }
    }
    exit(EXIT_SUCCESS);
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


int main() {
    run_worker();
}
