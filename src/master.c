#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "socket_server.h"
#include "queue.h"
#include "pipe.h"
#include "worker.h"


#define NUMBER_OF_WORKERS (3)


FILE *fp = NULL;
t_queue *worker_queue;

int n_processed_data;
time_t start_time;


void on_init(const int sock_fd);
void on_join(const int sock_fd);
void on_leave(const int sock_fd);
void on_receive(const int sock_fd, const void *buf, const size_t bufsize);

void spawn_worker(const int worker_id);
int find_schedulable_worker();
void schedule_job();
void print_status();


void run_master(FILE *rfp) {
    fp = rfp;
    worker_queue = queue_create();
    n_processed_data = 0;
    start_time = time(NULL);

    printf("[master] hosting socket server...\n");
    socket_host(on_init, on_join, on_leave, on_receive, NUMBER_OF_WORKERS);

    fclose(rfp);
    exit(EXIT_SUCCESS);
    return;
}


void on_init(const int sock_fd) {
    // printf("[master] spawning %d worker(s)...\n", NUMBER_OF_WORKERS);
    // for (int i = 0; i < NUMBER_OF_WORKERS; i++) {
    //     spawn_worker(i);
    // }
    printf("[master] total %d nodes are connected.\n", (int) worker_queue->size);
}


void on_hook() {
    printf("[master] hook called!\n");
    queue_print(worker_queue);
    print_status();
    schedule_job();
}


void on_join(const int sock_fd) {
    printf("[master] worker %d joined.\n", sock_fd);
    queue_push(worker_queue, sock_fd);
    printf("[master] total %d nodes are connected.\n", (int) worker_queue->size);
    schedule_job();
}


void on_leave(const int sock_fd) {
    printf("[master] worker %d leaved.\n", sock_fd);
    if (queue_contains(worker_queue, sock_fd)) {
        queue_remove(worker_queue, sock_fd);
    }
    if (queue_contains(worker_queue, -sock_fd)) {
        queue_remove(worker_queue, -sock_fd);
    }
}


void on_receive(const int sock_fd, const void *buf, const size_t bufsize) {
    printf("[master] recieved processed data from worker (%d): \"%s\"\n", sock_fd, (char *) buf);
    n_processed_data++;

    // mark as not busy
    queue_remove(worker_queue, -sock_fd);
    queue_push(worker_queue, sock_fd);
    queue_print(worker_queue);
}


void schedule_job() {
    static char buf[BUFSIZ];
    static int bufsize = BUFSIZ;
    int sock_fd;

    printf("[master] scheduling job...\n");

    // Search for non-busy worker node
    sock_fd = find_schedulable_worker();
    if (sock_fd == 0) {
        printf("[master] couldn't find schedulable worker\n");
        return;
    }
    printf("[master] found schedulable worker with socket fd %d\n", sock_fd);

    printf("[master] waiting data from client...\n");
    do {
        pipe_readline(fp, buf, bufsize);
    } while (strlen(buf) == 0); // ignore empty line
    printf("[master] received data from client\n");

    if (send(sock_fd, buf, strlen(buf), 0) != strlen(buf) ) {
        perror("[master] socket send failed");
        return;
    }

    printf("[master] scheduling worker (%d) with data \"%s\"\n", sock_fd, buf);

    // mark as busy
    queue_remove(worker_queue, sock_fd);
    queue_push(worker_queue, -sock_fd);
    queue_print(worker_queue);

}


int find_schedulable_worker() {
    // 지금 바쁘지 않은 워커 노드를 찾는다.
    // socket fd가 음수이면 바쁜 것이다.
    for (int i = 0; i < worker_queue->size; i++) {
        if (queue_front(worker_queue) > 0) {
            return queue_front(worker_queue);
        }
        queue_push(worker_queue, queue_pop(worker_queue));
    }
    return 0;
}


void spawn_worker(const int worker_id) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("[master] fork failed");
        exit(EXIT_FAILURE);
        return;
    }
    if (pid == 0) {
        run_worker(worker_id);
        exit(EXIT_SUCCESS);
        return;
    }
    printf("[master] created worker with pid: %d\n", pid);
    return;
}


void print_status() {
    int time_passed = time(NULL) - start_time;
    printf("[master]\n");
    printf("    # of workers         : %d\n", (int) worker_queue->size);
    printf("    # of data processed  : %d\n", n_processed_data);
    printf("    # of data per second : %.2f\n", (float) n_processed_data / time_passed);
    printf("    time passed           : %d sec.\n", time_passed);
    printf("\n");
}
