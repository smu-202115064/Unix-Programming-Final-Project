#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include "socket.h"

#define VIRTUAL_PROCESS_TIME (1)


int main() {
    socknode_t sock;

    socket_init(&sock);
    socket_connect(&sock);

    while (true) {
        socket_recv(&sock);
        // START PROCESSING DATA
        for (int i = 0; i < sock.bufsize; i++) {
            sock.buf[i] = toupper(sock.buf[i]);
        }
        sleep(VIRTUAL_PROCESS_TIME);
        // END PROCESSING DATA
        socket_send(&sock);
    }

    return 0;
}
