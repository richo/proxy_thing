#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
        error("Couldn't create server socket");

    serv_sock_addr.sin_family = AF_INET;
    serv_sock_addr.sin_addr.s_addr = INADDR_ANY;
    serv_sock_addr.sin_port = htons(server_port);
    if (bind(server_sock, (struct sockaddr *) &serv_sock_addr, sizeof(serv_sock_addr)) < 0) {
        error("Couldn't bind server socket");
    }
    if (listen(server_sock, LISTEN_BACKLOG) < 0)
        error("Couldn't listen on server socket");

    /* For now we're just using the first socket, but this'll be a rotating pool */

    /* TODO select(2) loop goes here */

    int _client, _client_len, n;
    char buffer[1024];

    clients[0] = malloc(sizeof(struct sockaddr_in));
    _client_len = sizeof(clients[0]);

    _client = accept(server_sock, (struct sockaddr *) &clients[0], &_client_len);
    if (_client < 0)
        error("Couldn't accept test client");

    memset(buffer, 0, 1024);

    n = read(_client, buffer, 1023);
    if (n < 0)
        error("Couldn't read bytes from test socket");

    printf(">> %s\n", buffer);
    fflush(stdout);

    close(_client);
    close(server_sock);
    return 0;
}
