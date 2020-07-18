#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include "errors.h"
#include "response.h"
#include "command.h"
#include "cache.h"

#define PORT 20017
#define BUFFER_SIZE 1024 * 1024

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
    int received = recvfrom(sockfd, buffer, size, 0, sockaddr, sockaddr_len);

    if (received == -1) {
        return -1;
    }

    buffer[received] = '\0';
    return received;
}

int send_response(int sockfd, struct sockaddr *sockaddr, socklen_t sockaddr_len, response res) {
    int sent;
    char *str = response_to_string(res);

    if ((sent = sendto(sockfd, str, strlen(str), 0, sockaddr, sockaddr_len)) == -1) {
        perror("Cannot send response");
    }

    printf("Sent %i byte, response is %s\n", sent, res.data);

    return sent;
}

void handle_connection(int sockfd, cache *memory) {
    char buffer[BUFFER_SIZE] = {0};
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    bzero(&client_address, client_address_len);

    int received = receive(sockfd, buffer, BUFFER_SIZE, (struct sockaddr *) &client_address, &client_address_len);

    if (received == -1) {
        perror("Error while receiving data");
        error_t err = get_error_t(ERR_CANNOT_RECV);
        send_response(sockfd, (struct sockaddr *) &client_address, client_address_len, errort_to_response(err));

        return;
    }

    printf("Received buffer with len: %lu, %s\n", strlen(buffer), buffer);

    command c;
    bzero(&c, sizeof(c));

    int parse_result;
    if ((parse_result = parse_command(buffer, &c)) < 0) {
        fprintf(stderr, "Error, cannot parse format: %i\n", parse_result);

        error_t err = get_error_t(parse_result);
        send_response(sockfd, (struct sockaddr *) &client_address, client_address_len, errort_to_response(err));

        return;
    }

    printf("Command is type: %i, key: %s, data: %s\n", c.command_type, c.key, c.data);

    if (c.command_type == GET) {
        char *data = get(memory, c.key);

        if (data == NULL) {
            error_t err = get_error_t(ERR_NO_DATA);
            send_response(sockfd, (struct sockaddr *) &client_address, client_address_len, errort_to_response(err));

            return;
        }

        response res = { STATUS_SUCCESS, data };
        send_response(sockfd,(struct sockaddr *) &client_address, client_address_len, res);

        return;
    }

    if (c.command_type == SET) {
        set(memory, c.key, c.data);
        response res = { STATUS_CREATED, "CREATED" };
        send_response(sockfd, (struct sockaddr *) &client_address, client_address_len, res);

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
                handle_connection(i, memory);
            }

            if (FD_ISSET(i, &write_fds)) {
                printf("I can write");
            }
        }
    }
}
