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

    if (strcmp(arr[0], "SET") != 0 && strcmp(arr[0], "GET") != 0) {
        return ERR_COMMAND_NOT_RECOGNIZED;
    }

    // Parse SET command
    if (strcmp(arr[0], "SET") == 0) {
        if (arr[1] == NULL) {
            return ERR_NO_KEY;
        }

        // Validate key length
        if (strlen(arr[1]) > MAX_KEY_LENGTH) {
            return ERR_KEY_LENGTH;
        }

        // Validate data
        if (arr[3] == NULL) {
            return ERR_NO_DATA;
        }

        c->command_type = SET;
        c->key = arr[1];
        c->data = arr[3];
    }

    // Parse GET command
    if (strcmp(arr[0], "GET") == 0) {
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