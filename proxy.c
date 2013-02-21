#include <stdlib.h>
#include <stdio.h>

void usage(void) {
    fprintf(stderr, "Usage: proxy <client port> <server port>\n");
}

int main(int argc, char** argv) {
    int client_port, server_port;

    if (argc < 2) {
        usage();
        exit(1);
    }

    client_port = atoi(argv[1]);
    server_port = atoi(argv[2]);

    if (client_port == 0 || server_port == 0) {
        usage();
        exit(2);
    }

    return 0;
}
