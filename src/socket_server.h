#ifndef	_SOCKET_SERVER_H
#define	_SOCKET_SERVER_H 1


#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include "socket.h"
#include "queue.h"


typedef void (*t_sock_on_join)(const int sock_fd);
typedef void (*t_sock_on_leave)(const int sock_fd);
typedef void (*t_sock_on_receive)(const int sock_fd, const void *buf, const size_t bufsize);


void socket_host(t_sock_on_join, t_sock_on_leave, t_sock_on_receive, const size_t);


#endif
