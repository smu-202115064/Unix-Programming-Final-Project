#ifndef	_SOCKET_H
#define	_SOCKET_H 1

#include "socket_types.h"

#define SOCKET_NAME "smu.socket"
#define SOCKET_PORT 8080

extern void socket_init(socknode_t *sock_node);
extern void socket_connect(const socknode_t *sock_node, const sockcallback_t callback);
extern void socket_listen(const size_t n_conn, const socknode_t *sock_node, const sockcallback_t callback);
extern void socket_accept(socknode_t *new_sock_node, const socknode_t *sock_node, const sockcallback_t callback);
extern void socket_send(const socknode_t *sock_node, const sockcallback_t callback);
extern void socket_recv(const socknode_t *sock_node, const sockcallback_t callback);

#endif
