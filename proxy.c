#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

#include "debug.h"

#define MAX_CONCURRENCY 512
#define LISTEN_BACKLOG 16

void error(const char *msg) {
    perror(msg);
    exit(3);
}

void usage(void) {
    fprintf(stderr, "Usage: proxy <client port> <server port>\n");
}

int main(int argc, char** argv) {
    fd_set rfds;
    int client_port, server_port;
    int client_sock, server_sock;

    struct cli_addr* clients[MAX_CONCURRENCY];

    struct sockaddr_in serv_sock_addr;

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

    log_debug("Creating server socket");
    FD_ZERO(&rfds);

    int i
    for (i = 0; i < MAX_CONCURRENCY; i++)
        FD_SET(clients[i], &rfds);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
        error("Couldn't create server socket");

    log_debug("Binding socket");
    serv_sock_addr.sin_family = AF_INET;
    serv_sock_addr.sin_addr.s_addr = INADDR_ANY;
    serv_sock_addr.sin_port = htons(server_port);
    if (bind(server_sock, (struct sockaddr *) &serv_sock_addr, sizeof(serv_sock_addr)) < 0) {
        error("Couldn't bind server socket");
    }
    if (listen(server_sock, LISTEN_BACKLOG) < 0)
        error("Couldn't listen on server socket");

    /* TODO try putting the following code in a loop
     *          to try re-entrant client select;
     *          might also want to randomly selecting
     *          nodes that are part of rfds;
     *          how would this work?
     */

    /* TODO fix bugs; this hasn't been tested yet... */

    struct timeval tv;
    int _client, _client_len, n, num;
    char buffer[1024];

    tv = {5, 0};
    num = select(MAX_CONCURRENCY, clients, NULL, NULL, &tv);

    if (num == -1)
        error("Couldn't select client socket");
    else if (num)
    {
        for (i = 0; i < MAX_CONCURRENCY; ++i)
        {
            if (FD_ISSET(clients[i], &rfds))
            {
                clients[i] = malloc(sizeof(struct sockaddr_in));
                _client_len = sizeof(clients[i]);
                _client = accept(server_sock, (struct sockaddr *) &clients[i], &_client_len);
                if (_client < 0)
                    error("Couldn't accept test client");
                memset(buffer, 0, 1024);

                n = read(_client, buffer, 1023);
                if (n < 0)
                    error("Couldn't read bytes from test socket");
            }
        }
    }
    else fprintf(stdout, "did not produce any data\n");

    printf(">> %s\n", buffer);
    fflush(stdout);

    close(_client);
    close(server_sock);
    return 0;
}
