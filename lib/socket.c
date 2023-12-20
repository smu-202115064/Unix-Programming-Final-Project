#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include "socket.h"

#define SOCKET_NAME "smu.socket"
#define SOCKET_PORT 8080


void socket_init(socket_node_t *socknode) {
    printf("[socket] initializing...\n");
    socknode->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socknode->fd == -1) {
        perror("socket_init");
        exit(EXIT_FAILURE);
    }
    socknode->addr.sin_family = AF_INET;
    socknode->addr.sin_addr.s_addr = INADDR_ANY;
    socknode->addr.sin_port = htons( SOCKET_PORT );
    socknode->addrlen = sizeof(socknode->addr);
    socknode->buf = (char *) calloc(BUFSIZ, sizeof(char));
    socknode->bufsize = BUFSIZ;
}


void socket_connect(const socket_node_t *socknode) {
    printf("[socket] connecting...\n");
    if (connect(socknode->fd, (struct sockaddr *)&socknode->addr, socknode->addrlen) < 0) {
        perror("socket_connect");
        exit(EXIT_FAILURE);
    }
}


void socket_listen(const socket_node_t *socknode, const size_t n_conn) {
    printf("[socket] bind & listening...\n");
    if (bind(socknode->fd, (struct sockaddr *)&socknode->addr, socknode->addrlen) < 0) {
        perror("socket_listen bind");
        exit(EXIT_FAILURE);
    }
    if (listen(socknode->fd, n_conn) == -1) {
        perror("socket_listen listen");
        exit(EXIT_FAILURE);
    }
}


void socket_accept(const socket_node_t *socknode, socket_node_t *new_socknode) {
    if ((new_socknode->fd = accept(socknode->fd, (struct sockaddr *)&new_socknode->addr, &new_socknode->addrlen)) < 0) {
        perror("socket_accept");
        exit(EXIT_FAILURE);
    }
}


void socket_send(const socket_node_t *socknode) {
    if(send(socknode->fd, socknode->buf, strlen(socknode->buf), 0) != strlen(socknode->buf)) {
        perror("socket_send");
        exit(EXIT_FAILURE);
    }
}


void socket_recv(const socket_node_t *socknode) {
    int n = read(socknode->fd, socknode->buf, socknode->bufsize-1);
    if (n == -1) {
        perror("socket_recv");
        exit(EXIT_FAILURE);
    }
    socknode->buf[n] = '\0';
}
