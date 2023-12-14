#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "worker.h"
#include "socket.h"


void run_worker(const int worker_id) {
    int sock_fd;
    struct sockaddr_un sock_addr;
    char buffer[BUFSIZ];
    worker_init_socket_fd(&sock_fd);
    worker_init_socket_addr(&sock_addr);
    worker_connect(sock_fd, &sock_addr);
    worker_send_client_hello(sock_fd, &sock_addr);
    while (true) {
        worker_recv_data(sock_fd, buffer, BUFSIZ-1);
        worker_proc_data(buffer);
        worker_send_data(sock_fd, &sock_addr, buffer);
    }
    exit(EXIT_SUCCESS);
}


void worker_init_socket_fd(int *sock_fd) {
    printf("[worker] creating socket\n");
    if ((*sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("[worker] socket creation failed");
        exit(EXIT_FAILURE);
    }
}


void worker_init_socket_addr(struct sockaddr_un *sock_addr) {
    memset((char *)sock_addr, 0, sizeof(*sock_addr));
    strcpy(sock_addr->sun_path, SOCKET_NAME);
    sock_addr->sun_family = AF_UNIX;
}


void worker_connect(const int sock_fd, const struct sockaddr_un *sock_addr) {
    printf("[worker] connecting to master\n");
    size_t len = sizeof(sock_addr->sun_family) + strlen(sock_addr->sun_path);
    if (connect(sock_fd, (struct sockaddr *)sock_addr, len) < 0) {
        perror("[worker] bind");
        exit(EXIT_FAILURE);
    }
    // Let master node know that worker is connected
    send(sock_fd, SOCKET_CLIENT_HELLO, (struct sockaddr *)sock_addr, len);
}


void worker_recv_data(const int sock_fd, char *buf, const size_t bufsize) {
    int n;
    if ((n = recv(sock_fd, buf, bufsize-1, 0)) == -1) {
        perror("[worker] could not receive data");
        exit(errno);
    }
    buf[n] = '\0';
}


void worker_proc_data(char *str) {
    // Convert to uppercase
    while (*str != '\0') {
        if (*str >= 'a' && *str <= 'z') {
            *str = *str + 'A' - 'a';
        }
        str++;
    }
    sleep(1);
}


void worker_send_data(const int sock_fd, const struct sockaddr_un *sock_addr, const char *data) {
    char buffer[BUFSIZ];
    size_t len = sizeof(sock_addr->sun_family) + strlen(sock_addr->sun_path);
    sprintf(buffer, SOCKET_DATA, data);
    send(sock_fd, buffer, (struct sockaddr *)sock_addr, len);
}
