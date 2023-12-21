#ifndef	_SOCKET_H
#define	_SOCKET_H 1

#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>

typedef int sockfd_t;
typedef struct sockaddr_in sockaddr_t;

struct _sock_node {
    sockfd_t fd;
    sockaddr_t addr;
    socklen_t addrlen;
    char buf[BUFSIZ];
};
typedef struct _sock_node socket_node_t;

extern void socket_init(socket_node_t *sock_node);
extern void socket_connect(const socket_node_t *sock_node);
extern void socket_listen(const socket_node_t *sock_node, const size_t n_conn);
extern void socket_accept(const socket_node_t *sock_node, socket_node_t *new_sock_node);
extern void socket_send(const socket_node_t *sock_node);
extern void socket_recv(socket_node_t *sock_node);

#endif
