#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "errors.h"
#include "command.h"

int parse_command(char *buffer, command *c) {
    char *delimiter = " ";
    char *token = strtok(buffer, delimiter);
    char **arr = calloc(3, sizeof(char *));

    // Iterate 3 times (a command is made of a maximum of 3 tokens)
    for (int i = 0; i < 3; i++) {
        if (token != NULL) {
            // if i > 1 the next token that we are parsing is data;
            // it should not be split by a white space since data can contains white spaces
            char *new_delimiter = i < 1 ? delimiter : "";

            if (strlen(token) > MAX_DATA_SIZE) {
                return ERR_MAX_DATA_SIZE;
            }

            token[strcspn(token, "\n")] = '\0'; // Remove trailing new lines
            arr[i] = calloc(1, strlen(token) + 1);
            strcpy(arr[i], token);
            token = strtok(NULL, new_delimiter); // Next token
        }
    }

    if (strcmp(arr[0], "SET") != 0 && strcmp(arr[0], "GET") != 0) {
        return ERR_COMMAND_NOT_RECOGNIZED;
    }

    // Validate the key
    if (arr[1] == NULL) {
        return ERR_NO_KEY;
    }

    // Validate key length
    if (strlen(arr[1]) > MAX_KEY_LENGTH) {
        return ERR_KEY_LENGTH;
    }

    c->key = arr[1];

    // Parse SET command
    if (strcmp(arr[0], "SET") == 0) {
        // Validate data
        if (arr[2] == NULL) {
            return ERR_NO_DATA;
        }

        c->command_type = SET;
        c->data = arr[2];
    }

    // Parse GET command
    if (strcmp(arr[0], "GET") == 0) {
        c->command_type = GET;
    }

    return 0;
}