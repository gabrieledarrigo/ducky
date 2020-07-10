#ifndef DUCKY_COMMAND_H
#define DUCKY_COMMAND_H

#include <stddef.h>

#define MAX_KEY_LENGTH 100
#define ERR_KEY_LENGTH -1
#define ERR_TTL_ZERO -2
#define ERR_NO_KEY -3
//#define ERR_DATA_EMPTY -4
#define ERR_SET_MALFORMED -4

typedef struct command {
    enum {
        GET, SET
    } command_type;
    int ttl;
    char *key;
    char *data;
} command;

int parse_command(char *buffer, command *c);

#endif
