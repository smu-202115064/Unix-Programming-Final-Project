#include <sys/un.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#define VIRTUAL_PROCESS_TIME (1)
#define VIRTUAL_SUDDENDIE_RATE (20)
void process_data(char *data);
void suicide_node();
void send_dead_node_info(int sock_fd);

void run_worker() {
    int sock_fd;
    struct sockaddr_un *sock_addr;
    char *buf;
    size_t bufsize;

    sock_addr = (struct sockaddr_un *)malloc(sizeof(struct sockaddr_un));

    bufsize = BUFSIZ;
    buf = (char *)malloc(bufsize);

    socket_init_fd(&sock_fd);
    socket_init_addr(sock_addr);

    socket_connect(sock_fd, sock_addr);

    //여기서 이제 난수 발생하여 더 random한 값 나옴
    srand((unsigned int)time(NULL)); // GOOOD!

    while (true) {
        int n = read(sock_fd, buf, bufsize - 1);
        buf[n - 1] = '\0';

        if (n == 0) {
            printf("[worker] disconnected from server\n");
            // break
            free(sock_addr);
            free(buf);
            exit(EXIT_FAILURE);
        }

        process_data(buf);

        if (send(sock_fd, buf, strlen(buf), 0) != strlen(buf)) {
            perror("[worker] socket send failed");
            free(sock_addr);
            free(buf);
            exit(EXIT_FAILURE);
        }

        if (rand() % 100 < VIRTUAL_SUDDENDIE_RATE) { // 20% 확률로 개복치사
            send_dead_node_info(sock_fd);
            suicide_node();
        }
    }

    free(sock_addr);
    free(buf);
    exit(EXIT_SUCCESS);
}

void process_data(char *str) {
    // Convert to uppercase
    while (*str != '\0') {
        if (*str >= 'a' && *str <= 'z') {
            *str = *str + 'A' - 'a';
        }
        str++;
    }
    sleep(VIRTUAL_PROCESS_TIME);
}

void suicide_node() {
    perror("[worker] Node is committing suicide");
    exit(EXIT_FAILURE);
}

void send_dead_node_info(int sock_fd) {
    char message[] = "[worker] Node has died!";
    if (send(sock_fd, message, strlen(message), 0) != strlen(message)) {
        perror("[worker] socket send failed");
        exit(EXIT_FAILURE);
    }
}
