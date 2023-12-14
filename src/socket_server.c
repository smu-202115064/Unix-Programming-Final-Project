#include "socket_server.h"


int get_max_fd(t_queue *queue, int default_fd) {
    int fd;
    for (int i = 0; i < queue->size; i++) {
        fd = queue_pop(queue);
        if (fd > default_fd) {
            default_fd = fd;
        }
        queue_push(queue, fd);
    }
    return default_fd;
}


void socket_host(t_sock_on_init on_init, t_sock_on_join on_join, t_sock_on_join on_leave, t_sock_on_receive on_receive, const size_t n_connections) {
    int server_fd;
    int client_fd;
    char *buf;
    size_t bufsize;
    struct sockaddr_un *server_addr;
    t_queue *client_queue;
    fd_set fdset;

    client_queue = queue_create();
    server_addr = (struct sockaddr_un *) malloc(sizeof(struct sockaddr_un));

    bufsize = 1024;
    buf = (char *) malloc(bufsize);

    remove(SOCKET_NAME);

    socket_init_fd(&server_fd);
    socket_init_addr(server_addr);

    socket_bind(server_fd, server_addr);
    socket_listen(server_fd, n_connections);

    FD_ZERO(&fdset);
    FD_SET(server_fd, &fdset);

    on_init(server_fd);

    while (true) {
        int activity = select(get_max_fd(client_queue, server_fd)+1, &fdset, NULL, NULL, NULL);
        if (activity < 0 && (errno != EINTR)) {
            perror("[server] select error");
            continue;
        }

        // 새로운 클라이언트가 접속했는지 확인.
        if (FD_ISSET(server_fd, &fdset)) {
            client_fd = socket_accept(server_fd, server_addr);
            queue_push(client_queue, client_fd);
            FD_SET(client_fd, &fdset);
            on_join(client_fd);
        }

        // 각 클라이언트 별로 접속을 종료했거나 메시지를 보냈는지 확인.
        for (int i = 0; i < client_queue->size; i++) {
            client_fd = queue_pop(client_queue);
            if (!FD_ISSET(client_fd, &fdset)) {
                continue;
            }
            int n = read(client_fd, buf, bufsize-1);
            buf[n] = '\0';
            if (n == 0) {
                FD_CLR(client_fd, &fdset);
                close(client_fd);
                // 워커 노드의 연결이 해제됨.
                on_leave(client_fd);
                continue;
            }
            queue_push(client_queue, client_fd);
            on_receive(client_fd, buf, n);
        }
    }
    close(server_fd);
    free(client_queue);
    free(server_addr);
    free(buf);
}
