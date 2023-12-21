#include <unistd.h>
#include <sys/select.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>
#include "../lib/socket.h"

#define NUMBER_OF_WORKERS (3)


struct client {
    socket_node_t socket;
    bool busy;
    bool active;
};
typedef struct client client_t;

socket_node_t server;
client_t clients[NUMBER_OF_WORKERS];
fd_set readfds;

void socket_init_all();
void socket_wait_ready();
void socket_accept_client();
void socket_recv_client(client_t *client);
void socket_send_client();


int main() {
    socket_init_all();
    socket_listen(&server, NUMBER_OF_WORKERS);

    printf("Server ready.\n");
    while (true) {
        // 서버/클라이언트 소켓 fd들에 새로운 IO가 오기를 기다린다.
        socket_wait_ready();

        // 소켓 서버 fd로 새로운 입력 요청이 왔다면, 새로운 클라이언트의 접속을 의미한다.
        if (FD_ISSET(server.fd, &readfds)) {
            socket_accept_client();
        }

        // 접속 중인 클라이언트 별로 새로운 I/O 대기가 있는지 검사한다.
        for (int i = 0; i < NUMBER_OF_WORKERS; i++) {
            if (clients[i].active == false) {
                continue;
            }

            if (FD_ISSET(clients[i].socket.fd, &readfds)) {
                socket_recv_client(&clients[i]);
            }
        }

        // 표준 입력으로 부터 들어올 데이터가 있다면
        // 놀고있는 클라이언트에게 일감을 부여한다.
        if (FD_ISSET(0, &readfds)) {
            socket_send_client();
        }
    }


    return 0;
}


void socket_init_all() {
    socket_init(&server);

    for (int i = 0; i < NUMBER_OF_WORKERS; i++) {
        socket_init(&clients[i].socket);
        clients[i].busy = false;
        clients[i].active = false;
    }

    printf("Established server, socket fd is %d.\n", server.fd);
}


void socket_wait_ready() {
    int max_fd;
    bool allow_stdin;

    FD_ZERO(&readfds);
    FD_SET(server.fd, &readfds); // 새로운 커넥션이 생성되는지 지켜볼 것.

    allow_stdin = false;

    // 마지막 fd가 무엇인지 찾는다.
    // 해당 fd까지 select() 함수가 지켜봐 줄 것이다.
    max_fd = server.fd;

    for (int i = 0; i < NUMBER_OF_WORKERS; i++) {
        if (clients[i].active == true) {
            FD_SET(clients[i].socket.fd, &readfds);

            if (clients[i].socket.fd > max_fd) {
                max_fd = clients[i].socket.fd;
            }

            if (clients[i].busy == false) {
                allow_stdin = true;
            }
        }
    }

    if (allow_stdin) {
        FD_SET(0, &readfds); // 새로운 요청이 와도 처리할 수 있음 -> STDIN 에서 입력이 들어오는지 지켜볼 것.
    } else {
        FD_CLR(0, &readfds); // 새로운 요청이 와도 처리할 수 없음 -> STDIN 에서 입력이 들어와도 처리를 하지 않고 기다림.
    }

    // 마지막 fd를 포함하여, 새로운 I/O를 수행할 준비가 된 fd가 있는지를 탐색한다.
    // 새로운 IO 준비가 되지 않았다면, 기다린다.
    // readfds만 탐색하므로, 새로 들어온 입력이 있는지만 본다.
    if (select(max_fd+1, &readfds, NULL, NULL, NULL) < 0 && (errno != EINTR)) {
        perror("select");
        exit(EXIT_FAILURE);
    }
}


void socket_accept_client() {
    printf("[socket_accept_client] A client has connected.\n");

    // 사용 가능한 클라이언트 칸을 탐색
    for (int i = 0; i < NUMBER_OF_WORKERS; i++) {
        if (clients[i].active == true) {
            continue;
        }

        socket_accept(&server, &clients[i].socket);
        clients[i].active = true;
        clients[i].busy = false;
        FD_SET(clients[i].socket.fd, &readfds); // select()에서 감시할 FD set에 추가해준다.

        printf("\tNew connection, socket fd is %d \n", clients[i].socket.fd);
        return;
    }

    printf("\tCould not find a space for a new client.\n");
    exit(EXIT_FAILURE);
}


void socket_recv_client(client_t *client) {
    printf("[socket_recv_client] Detect client fd %d ready.\n", client->socket.fd);

    // 새로 입력이 들어올 준비가 되었다면 받아들인다.
    socket_recv(&client->socket);
    client->busy = false;

    // 만약 IO 준비가 되었지만 들어오는 데이터가 없다면,
    // 접속 종료로 간주한다.
    if (strlen(client->socket.buf) == 0) {
        printf("\tClient %d has disconnected.\n", client->socket.fd);

        FD_CLR(client->socket.fd, &readfds);
        close(client->socket.fd);
        client->active = false;

        return;
    }

    printf("\tReceived: %s\n", client->socket.buf);
    FD_SET(0, &readfds);
}


void socket_send_client() {
    int len;
    printf("[socket_send_client] Try to schedule a client.\n");
    for (int i = 0; i < NUMBER_OF_WORKERS; i++) {
        if (clients[i].active == false || clients[i].busy == true) {
            continue;
        }

        printf("\tFound a client to schedule.\n");
        printf("\tClient fd is %d, size of buffer is %ld.\n", clients[i].socket.fd, sizeof(clients[i].socket.buf));

        do {
            for (len = 0; !feof(stdin) && len < BUFSIZ; len++) {
                if (read(0, clients[i].socket.buf+len, sizeof(char)) == 0) {
                    perror("fread");
                    exit(EXIT_FAILURE);
                }
                if (clients[i].socket.buf[len] == '\n') {
                    break;
                }
            }
            clients[i].socket.buf[len] = '\0';
        } while (len == 0);

        printf("\tReady to send data: \"%s\".\n", clients[i].socket.buf);

        socket_send(&clients[i].socket);
        clients[i].busy = true;
        return;
    }

    printf("\tCould not find current available client to schedule.\n");
    FD_CLR(0, &readfds);
}
