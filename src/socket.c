#include "socket.h"


size_t socket_calc_len(const struct sockaddr_un *sock_addr) {
    return sizeof(sock_addr->sun_family) + strlen(sock_addr->sun_path);
}


void socket_init_fd(int *sock_fd) {
    *sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (*sock_fd == -1) {
        perror("[socket] init failed");
        exit(EXIT_FAILURE);
    }
}


void socket_init_addr(struct sockaddr_un *sock_addr) {
    memset((char *)&sock_addr, 0, sizeof(struct sockaddr_un));
    strcpy(sock_addr->sun_path, SOCKET_NAME);
    sock_addr->sun_family = AF_UNIX;
}


void socket_connect(const int sock_fd, const struct sockaddr_un *sock_addr) {
    if (connect(sock_fd, (struct sockaddr *)&sock_addr, socket_calc_len(sock_addr)) < 0) {
        perror("[socket] connect failed");
        exit(EXIT_FAILURE);
    }
}


void socket_bind(const int sock_fd, const struct sockaddr_un *sock_addr) {
    if (bind(sock_fd, (struct sockaddr *)&sock_addr, socket_calc_len(sock_addr)) < 0) {
        perror("[socket] bind failed");
        exit(EXIT_FAILURE);
    }
}


void socket_listen(const int sock_fd, const int n_connections) {
    if (listen(sock_fd, n_connections) == -1) {
        perror("[socket] listen failed");
        exit(EXIT_FAILURE);
    }
}


int socket_accept(const int sock_fd, const struct sockaddr_un *sock_addr) {
    socklen_t sock_len = socket_calc_len(sock_addr);
    int new_sock_fd = accept(sock_fd, (struct sockaddr *)&sock_addr, &sock_len);
    if (new_sock_fd < 0) {
        perror("[socket] accept failed");
        exit(EXIT_FAILURE);
    }
    return new_sock_fd;
}
