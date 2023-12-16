#ifndef	_SOCKET_TYPES_H
#define	_SOCKET_TYPES_H 1

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

typedef void (*sockcallback_t)(const socknode_t *);

#endif
