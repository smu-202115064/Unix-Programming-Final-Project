#include "socket.h"


size_t socket_calc_len(const t_sock_addr *sock_addr) {
    return sizeof(*sock_addr);
}


void socket_init_fd(int *sock_fd) {
    printf("[socket] initializing socket file descriptor...\n");
    *sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock_fd == -1) {
        perror("[socket] init failed");
        exit(EXIT_FAILURE);
    }
}


void socket_init_addr(t_sock_addr *sock_addr) {
    printf("[socket] initializing socket address...\n");
    sock_addr->sin_family = AF_INET;
    sock_addr->sin_addr.s_addr = INADDR_ANY;
    sock_addr->sin_port = htons( SOCKET_PORT );
}


void socket_init_addr_ipv4(t_sock_addr *sock_addr, const char *ipv4) {
    socket_init_addr(sock_addr);
    if (inet_pton(AF_INET, ipv4, sock_addr->sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }
}


void socket_connect(const int sock_fd, const t_sock_addr *sock_addr) {
    printf("[socket] connecting...\n");
    if (connect(sock_fd, (struct sockaddr *)sock_addr, socket_calc_len(sock_addr)) < 0) {
        perror("[socket] connect failed");
        exit(EXIT_FAILURE);
    }
    printf("[socket] connected successfully\n");
}


void socket_bind(const int sock_fd, const t_sock_addr *sock_addr) {
    printf("[socket] binding...\n");
    if (bind(sock_fd, (struct sockaddr *)sock_addr, socket_calc_len(sock_addr)) < 0) {
        perror("[socket] bind failed");
        exit(EXIT_FAILURE);
    }
    printf("[socket] binded successfully\n");
}


void socket_listen(const int sock_fd, const int n_connections) {
    printf("[socket] listening...\n");
    if (listen(sock_fd, n_connections) == -1) {
        perror("[socket] listen failed");
        exit(EXIT_FAILURE);
    }
    printf("[socket] listening up to %d connections\n", n_connections);
}


int socket_accept(const int sock_fd, const t_sock_addr *sock_addr) {
    socklen_t sock_len = socket_calc_len(sock_addr);
    int new_sock_fd = accept(sock_fd, (struct sockaddr *)&sock_addr, &sock_len);
    if (new_sock_fd < 0) {
        perror("[socket] accept failed");
        exit(EXIT_FAILURE);
    }
    return new_sock_fd;
}
