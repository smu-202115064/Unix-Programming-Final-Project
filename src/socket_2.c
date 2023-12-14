#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define SOCKET_NAME "smu.socket"
#define TRUE 1
#define FALSE 0

int main_2(int argc, char *argv[]) {
    int opt = TRUE;
    int master_socket, addrlen, new_socket, client_socket[30],
        max_clients = 30, activity, i, valread, sd;
    int max_sd;
    struct sockaddr_un address;

    char buffer[1025]; // data buffer of 1K

    fd_set readfds;

    char *message = "ECHO Daemon v1.0 \r\n";

    for (i = 0; i < max_clients; i++) {
        client_socket[i] = 0;
    }

    if ((master_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("[socket ser] socket");
        exit(1);
    }

    memset((char *)&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_NAME);
    addrlen = sizeof(address);

    if (bind(master_socket, (struct sockaddr *)&address, addrlen) < 0) {
        perror("[socket ser] bind");
        exit(1);
    }

    if (listen(master_socket, 3) < 0) {
        perror("[socket ser] listen");
        exit(1);
    }

    puts("Listener on socket");

    puts("Waiting for connections ...");

    while (TRUE) {
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        for (i = 0; i < max_clients; i++) {
            sd = client_socket[i];

            if (sd > 0)
                FD_SET(sd, &readfds);

            if (sd > max_sd)
                max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            if (errno == EINTR) {
                // EINTR은 시그널에 의한 인터럽트이므로 무시하고 계속 진행
                continue;
            } else {
                fprintf(stderr, "Error in select: %s (%d)\n", strerror(errno), errno);
                // 에러 처리 또는 프로그램 종료 등 추가 작업 수행
                // 예를 들어, exit(EXIT_FAILURE); 등을 사용하여 프로그램 종료
                break;  // 예시로 루프를 빠져나가도록 함
            }
        }

        if (FD_ISSET(master_socket, &readfds)) {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            printf("New connection, socket fd is %d, path: %s\n", new_socket, address.sun_path);

            if (send(new_socket, message, strlen(message), 0) != strlen(message)) {
                perror("send");
            }

            puts("Welcome message sent successfully");

            for (i = 0; i < max_clients; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n", i);
                    break;
                }
            }
        }

        for (i = 0; i < max_clients; i++) {
            sd = client_socket[i];

            if (FD_ISSET(sd, &readfds)) {
                if ((valread = read(sd, buffer, 1024)) == 0) {
                    socklen_t addr_len = sizeof(address);  // Corrected line
                    getpeername(sd, (struct sockaddr *)&address, &addr_len);
                    printf("Host disconnected, path: %s\n", address.sun_path);

                    close(sd);
                    client_socket[i] = 0;
                } else {
                    buffer[valread] = '\0';
                    send(sd, buffer, strlen(buffer), 0);
                }
            }
        }
    }

    return 0;
}
