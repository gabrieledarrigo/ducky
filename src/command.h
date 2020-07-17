#ifndef DUCKY_COMMAND_H
#define DUCKY_COMMAND_H

#include <stddef.h>

#define MAX_KEY_LENGTH 100
#define MAX_DATA_SIZE 1048576

typedef struct command {
    enum {
        GET, SET
    } command_type;
    char *key;
    char *data;
} command;

int parse_command(char *buffer, command *c);
#endif
