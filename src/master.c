#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "socket_server.h"
#include "queue.h"
#include "pipe.h"


#define NUMBER_OF_WORKERS (3)


FILE *fp = NULL;
t_queue *worker_queue;


void on_join(const int sock_fd);
void on_leave(const int sock_fd);
void on_receive(const int sock_fd, const void *buf, const size_t bufsize);

void spawn_client();
void spawn_worker();
int find_schedulable_worker();
void schedule_job();
extern void run_worker();


void run_master(FILE *rfp) {
    fp = rfp;

    printf("[master] spawning %d worker(s)...\n", NUMBER_OF_WORKERS);
    for (int i = 0; i < NUMBER_OF_WORKERS; i++) {
        spawn_worker(i);
    }

    printf("[master] hosting socket server...\n");
    socket_host(on_join, on_leave, on_receive, NUMBER_OF_WORKERS);

    fclose(rfp);
    exit(EXIT_SUCCESS);
    return;
}


void on_join(const int sock_fd) {
    queue_push(worker_queue, sock_fd);
}


void on_leave(const int sock_fd) {
    if (queue_contains(worker_queue, sock_fd)) {
        queue_remove(worker_queue, sock_fd);
    }
    if (queue_contains(worker_queue, -sock_fd)) {
        queue_remove(worker_queue, -sock_fd);
    }
}


void on_receive(const int sock_fd, const void *buf, const size_t bufsize) {
    printf("[master] recieved processed data from worker: %s\n", (char *) buf);

    if (!queue_contains(worker_queue, -sock_fd)) {
        printf("[master] could not find sock_fd %d (busy) from queue\n", sock_fd);
        exit(EXIT_FAILURE);
        return;
    }

    // mark as not busy
    queue_remove(worker_queue, -sock_fd);
    queue_push(worker_queue, sock_fd);

    schedule_job();
}


void schedule_job() {
    static char buf[BUFSIZ];
    static int bufsize = BUFSIZ;
    int sock_fd;

    printf("[master] scheduling job...\n");

    pipe_readline(fp, buf, bufsize);
    printf("[master] received data from client: %s\n", buf);

    sock_fd = find_schedulable_worker();
    printf("[master] found schedulable worker with socket fd %d\n", sock_fd);

    if (send(sock_fd, buf, strlen(buf), 0) != strlen(buf) ) {
        perror("[master] socket send failed");
        return;
    }

    // 바쁜 것으로 표시 (음수 소켓 설명자로 변경)
    queue_remove(worker_queue, sock_fd);
    queue_push(worker_queue, -sock_fd);
    printf("[master] scheduled worker %d with data %s\n", sock_fd, buf);
}


int find_schedulable_worker() {
    // 지금 바쁘지 않은 워커 노드를 찾는다.
    // socket fd가 음수이면 바쁜 것이다.
    while (queue_front(worker_queue) < 0) {
        queue_push(worker_queue, queue_pop(worker_queue));
    }
    return queue_front(worker_queue);
}


void spawn_worker() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("[master] fork failed");
        exit(EXIT_FAILURE);
        return;
    }
    if (pid == 0) {
        run_worker();
        exit(EXIT_SUCCESS);
        return;
    }
    printf("[master] created worker with pid: %d\n", pid);
    return;
}
