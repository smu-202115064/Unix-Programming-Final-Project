#ifndef	_SOCKET_EVENT_H
#define	_SOCKET_EVENT_H 1


#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include "socket.h"
#include "queue.h"


typedef void (*t_sock_on_connect)();
typedef void (*t_sock_on_recv)(const char *msg);

typedef void (*t_sock_on_accept)(const int sock_fd);
typedef void (*t_sock_on_init)(const int sock_fd);
typedef void (*t_sock_on_join)(const int sock_fd);
typedef void (*t_sock_on_leave)(const int sock_fd);
typedef void (*t_sock_on_receive)(const int sock_fd, const char *buf, const size_t bufsize);


#endif
