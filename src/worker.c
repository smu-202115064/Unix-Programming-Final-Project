#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "../lib/socket.h"

#define VIRTUAL_PROCESS_TIME (3)


int main() {
    socket_node_t sock;

    socket_init(&sock);
    socket_connect(&sock);

    strcpy(sock.buf, "Hello, world!");
    socket_send(&sock);

    printf("Worker ready.\n");
    while (true) {
        printf("Listening for next data...\n");
        socket_recv(&sock);

        if (strlen(sock.buf) == 0) {
            printf("\tNo data received.\n");
            break;
        }

        printf("\tReceived data: \"%s\"\n\n", sock.buf);
        // START PROCESSING DATA
        for (int i = 0; i < BUFSIZ; i++) {
            sock.buf[i] = toupper(sock.buf[i]);
        }
        sleep(VIRTUAL_PROCESS_TIME);
        // END PROCESSING DATA
        socket_send(&sock);
        printf("\tSent data: \"%s\"\n\n", sock.buf);
    }

    return 0;
}
