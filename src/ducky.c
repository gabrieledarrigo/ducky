#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <netinet/in.h>
#include "command.h"
#include "cache.h"

#define PORT 20017
#define BUFFER_SIZE 1024

int make_socket(int port, int reuse) {
    int sockfd;
    struct sockaddr_in sockaddr;

    bzero(&sockaddr, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr.s_addr = htons(INADDR_ANY);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Cannot create socket");
        exit(EXIT_FAILURE);
    }

    if (reuse == 1) {
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(int));
    }

    if (bind(sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1) {
        perror("Cannot create socket");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

int receive(int sockfd, char *buffer, size_t size, struct sockaddr *sockaddr, socklen_t *sockaddr_len) {
    int received = recvfrom(sockfd, buffer, size, 0, (struct sockaddr *) &sockaddr, sockaddr_len);

    if (received == -1) {
        return -1;
    }

    buffer[received] = '\0';
    return received;
}

void handle_connection(int sockfd, cache *memory) {
    char buffer[BUFFER_SIZE] = {0};
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    int received = receive(sockfd, buffer, BUFFER_SIZE, (struct sockaddr *) &client_address, &client_address_len);

    if (received == -1) {
        // @TODO returns 5xx with the error
        perror("Error while receiveing data");
    }

    printf("Received %s\n", buffer);

    command c;
    bzero(&c, sizeof(c));

    int result;
    if ((result = parse_command(buffer, &c)) < 0) {
        fprintf(stderr, "Error, cannot parse format: %i\n", result);

        // @TODO returns 5xx with the error
        return;
    }

    printf("Command is type: %i, key: %s, data: %s\n", c.command_type, c.key, c.data);

    if (c.command_type == GET) {
        char *data = get(memory, c.key);
        printf("Data is %s\n", data);

        // @TODO returns 200 data
        return;
    }

    if (c.command_type == SET) {
        set(memory, c.key, c.data);

        // @TODO returns 201 data
        return;
    }
}

int main() {
    int sockfd, maxfd;
    fd_set active_fds;
    fd_set read_fds;
    fd_set write_fds;

    cache *memory = cache_new();
    sockfd = make_socket(PORT, 1);
    maxfd = sockfd;

    FD_ZERO(&active_fds);
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_SET(sockfd, &active_fds);

    while (1) {
        read_fds = active_fds;

        if (select(maxfd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("Error during select operation");
            exit(1);
        }

        for (int i = 0; i <= maxfd; i++) {
            if (FD_ISSET(i, &read_fds)) {
                handle_connection(sockfd, memory);
            }

            if (FD_ISSET(i, &write_fds)) {
                printf("I can write");
            }
        }
    }
}