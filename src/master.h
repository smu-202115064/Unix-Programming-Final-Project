#ifndef	_WORKER_H
#define	_WORKER_H 1

#include <sys/un.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>

struct worker_info {
    pid_t pid;
    int sock_fd;
    bool busy;
};

typedef struct worker_info t_worker_info;


extern void run_master(const FILE *rfp);

extern t_worker_info *master_spwn_worker(const int worker_id);

extern void master_init_socket_fd(int *sock_fd);
extern void master_init_socket_addr(struct sockaddr_un *sock_addr);
extern void master_bind(const int sock_fd, const struct sockaddr_un *sock_addr);
extern void master_listen(const int sock_fd, const int n_connections);

extern void master_send_data(const int sock_fd, const struct sockaddr_un *sock_addr, const char *data);
extern void master_recv_data(const int sock_fd, char *buf, const size_t bufsize);

#endif
