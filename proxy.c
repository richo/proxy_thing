#include <stdlib.h>
#include <stdio.h>

void usage(void) {
    fprintf(stderr, "Usage: proxy <client port> <server port>\n");
}

int main(int argc, char** argv) {
    int client, server;

    if (argc < 2) {
        usage();
        exit(1);
    }

    client = atoi(argv[1]);
    server = atoi(argv[2]);

    if (client == 0 || server == 0) {
        usage();
        exit(2);
    }

    return 0;
}
