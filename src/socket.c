#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int socket_create_server(int number_of_clients) {
    int sd, len;
    struct sockaddr_un ser, cli;

    if ((sd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("[socket ser] socket");
        exit(1);
    }

    memset((char *)&ser, 0, sizeof(ser));
    ser.sun_family = AF_UNIX;
    strcpy(ser.sun_path, SOCKET_NAME);
    len = sizeof(ser.sun_family)+strlen(ser.sun_path);

    if (bind(sd, (struct sockaddr *)&ser, len) < 0) {
        perror("[socket ser] bind");
        exit(1);
    }

    if (listen(sd, number_of_clients) == -1) {
        perror("[socket ser] listen");
        exit(1);
    }

    return sd;
}


int socket_create_client() {
    int sd, len;
    struct sockaddr_un ser;

    if ((sd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("[socket cli] socket");
        exit(1);
    }

    memset((char *)&ser, 0, sizeof(ser));
    ser.sun_family = AF_UNIX;
    strcpy(ser.sun_path, SOCKET_NAME);

    len = sizeof(ser.sun_family)+strlen(ser.sun_path);
    if (connect(sd, (struct sockaddr *)&ser, len) < 0) {
        perror("[socket cli] bind");
        exit(1);
    }

    return sd;
}
