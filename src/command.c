#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "command.h"

int parse_command(char * buffer, command* c) {
    int i = 0;
    char *delimiter = " ";
    char *token = strtok(buffer, delimiter);
    char **arr = calloc(4, sizeof(char *));

    while (token != NULL) {
        token[strcspn(token, "\n")] = '\0'; // Remove trailing new lines
        arr[i] = calloc(1, strlen(token) + 1);
        strcpy(arr[i], token);
        token = strtok(NULL, delimiter); // Next token
        i++;
    }

    // Parse SET command
    if (strncmp(arr[0], "SET", sizeof("SET")) == 0) {
        int len = -1;
        while (arr[++len] != NULL){}

        if (len < 4) {
            return ERR_SET_MALFORMED;
        }

        int ttl;
        char *endptr;
        ttl = (int) strtol(arr[2], &endptr, 10);

        // Validate key length
        if (strlen(arr[1]) > MAX_KEY_LENGTH) {
            return ERR_KEY_LENGTH;
        }

        // Validate ttl
        if (ttl < 0) {
            return ERR_TTL_ZERO;
        }

        c->command_type = SET;
        c->ttl = ttl;
        c->key = arr[1];
        c->data = arr[3];
    }

    // Parse GET command
    if (strncmp(arr[0], "GET", sizeof("GET")) == 0) {
        if (arr[1] == NULL) {
            return ERR_NO_KEY;
        }

        // Validate key length
        if (strlen(arr[1]) > MAX_KEY_LENGTH) {
            return ERR_KEY_LENGTH;
        }

        c->command_type = GET;
        c->key = arr[1];
    }

    return 0;
}