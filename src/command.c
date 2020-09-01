#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "errors.h"
#include "command.h"

/**
 * Try to parse the buffer parameter into a command that can be recognized by Ducky.
 * A command is made up of the command name, optional command parameters, and the structured data that clients want to store or retrieve from Ducky.
 * For example a GET command is the following:
 *
 * GET key
 *
 * While a SET command has this form:
 *
 * SET key data
 *
 * If the function is unable to parse the input buffer returns an error code.
 *
 * @param buffer    the string the must be parsed into a valid Ducky command
 * @param c         the command struct
 * @return          a parsed command or an error code if the buffer cannot be parsed
 */
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