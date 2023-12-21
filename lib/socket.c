#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include "socket.h"

#define SOCKET_NAME "smu.socket"
#define SOCKET_PORT 8080


void socket_init(socket_node_t *sock) {
    printf("[socket] initializing...\n");
    sock->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock->fd == -1) {
        perror("socket_init");
        exit(EXIT_FAILURE);
    }
    sock->addr.sin_family = AF_INET;
    sock->addr.sin_addr.s_addr = INADDR_ANY;
    sock->addr.sin_port = htons( SOCKET_PORT );
    sock->addrlen = sizeof(sock->addr);
}


void socket_connect(const socket_node_t *sock) {
    printf("[socket] connecting...\n");
    if (connect(sock->fd, (struct sockaddr *)&sock->addr, sock->addrlen) < 0) {
        perror("socket_connect");
        exit(EXIT_FAILURE);
    }
}


void socket_listen(const socket_node_t *sock, const size_t n_conn) {
    int opt = true;
    printf("[socket] binding...\n");
    if (setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    if (bind(sock->fd, (struct sockaddr *)&sock->addr, sock->addrlen) < 0) {
        perror("socket_listen bind");
        exit(EXIT_FAILURE);
    }
    printf("[socket] listening...\n");
    if (listen(sock->fd, n_conn) == -1) {
        perror("socket_listen listen");
        exit(EXIT_FAILURE);
    }
}


void socket_accept(const socket_node_t *sock, socket_node_t *new_socknode) {
    new_socknode->addrlen = sizeof(new_socknode->addr);
    if ((new_socknode->fd = accept(sock->fd, (struct sockaddr *)&new_socknode->addr, &new_socknode->addrlen)) < 0) {
        perror("socket_accept");
        exit(EXIT_FAILURE);
    }
}


void socket_send(const socket_node_t *sock) {
    if(send(sock->fd, sock->buf, strlen(sock->buf), 0) != strlen(sock->buf)) {
        perror("socket_send");
        exit(EXIT_FAILURE);
    }
    fflush(NULL);
}


void socket_recv(socket_node_t *sock) {
    int n = read(sock->fd, sock->buf, BUFSIZ-1);
    if (n == -1) {
        perror("socket_recv");
        exit(EXIT_FAILURE);
    }
    sock->buf[n] = '\0';
    fflush(NULL);
}
