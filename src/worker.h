#ifndef	_WORKER_H
#define	_WORKER_H 1

#include <sys/un.h>
#include <sys/types.h>

extern void run_worker(const int worker_id);

extern void worker_init_socket_fd(int *sock_fd);
extern void worker_init_socket_addr(struct sockaddr_un *sock_addr);
extern void worker_connect(const int sock_fd, const struct sockaddr_un *sock_addr);
extern void worker_recv_data(const int sock_fd, char *buf, const size_t bufsize);
extern void worker_proc_data(char *data);
extern void worker_send_data(const int sock_fd, const struct sockaddr_un *sock_addr, const char *data);

#endif
