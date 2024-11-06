#include <stdio.h>
#include <stdlib.h>
#include "server.h"

int main(int argc, char *argv[]) {
    int port = 0;

    if (argc != 3 || strcmp(argv[1], "-p") != 0) {
        fprintf(stderr, "Usage: %s -p <port>\n", argv[0]);
        return 1;
    }

    port = atoi(argv[2]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number.\n");
        return 1;
    }

    start_server(port);

    return 0;
}
