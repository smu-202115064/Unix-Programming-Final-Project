#include <stdbool.h>
#include "socket.h"
#include "socket_client.h"
#include "socket_event.h"


void socket_client(t_sock_on_connect on_connect, t_sock_on_recv on_recv) {
    int server_fd;
    t_sock_addr *server_addr = (t_sock_addr *) malloc(sizeof(t_sock_addr));
    char buf[BUFSIZ];

    socket_init_fd(&server_fd);
    socket_init_addr(server_addr);
    socket_connect(server_fd, server_addr);
    on_connect();
    while (true) {
        socket_recv(server_fd, buf, BUFSIZ);
        on_recv(buf);
    }
    free(server_addr);
    exit(EXIT_FAILURE);
}
