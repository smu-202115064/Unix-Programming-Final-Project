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
    char *buf;
    size_t bufsize;
};
typedef struct _sock_node socknode_t;

extern void socket_init(socknode_t *sock_node);
extern void socket_connect(const socknode_t *sock_node);
extern void socket_listen(const size_t n_conn, const socknode_t *sock_node);
extern void socket_accept(socknode_t *new_sock_node, const socknode_t *sock_node);
extern void socket_send(const socknode_t *sock_node);
extern void socket_recv(const socknode_t *sock_node);

#endif
