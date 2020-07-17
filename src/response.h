#ifndef DUCKY_RESPONSE_H
#define DUCKY_RESPONSE_H

typedef enum status_code {
    SUCCESS = 200,
    CREATED = 201,
    ERR_UNKNOWN = 500,
    ERR_CANNOT_RECV = 501,
    ERR_COMMAND_NOT_RECOGNIZED = 502,
    ERR_MAX_DATA_SIZE = 503,
    ERR_KEY_LENGTH = 504,
    ERR_NO_KEY = 505,
    ERR_NO_DATA = 506,
} status_code;

typedef struct response {
    int status_code;
    char *data;
} response;

char *response_to_string(response res);

#endif
