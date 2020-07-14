#ifndef DUCKY_COMMAND_H
#define DUCKY_COMMAND_H

#include <stddef.h>

#define MAX_KEY_LENGTH 100
#define MAX_DATA_SIZE 1048576

#define ERR_COMMAND_NOT_RECOGNIZED -1
#define ERR_MAX_DATA_SIZE -2
#define ERR_KEY_LENGTH -3
#define ERR_NO_KEY -4
#define ERR_NO_DATA -5

typedef struct command {
    enum {
        GET, SET
    } command_type;
    char *key;
    char *data;
} command;

int parse_command(char *buffer, command *c);

#endif
