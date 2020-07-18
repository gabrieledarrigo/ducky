#ifndef DUCKY_RESPONSE_H
#define DUCKY_RESPONSE_H

typedef enum status_code {
    STATUS_SUCCESS = 200,
    STATUS_CREATED = 201,
    STATUS_ERR_UNKNOWN = 500,
    STATUS_ERR_CANNOT_RECV = 501,
    STATUS_ERR_COMMAND_NOT_RECOGNIZED = 502,
    STATUS_ERR_MAX_DATA_SIZE = 503,
    STATUS_ERR_KEY_LENGTH = 504,
    STATUS_ERR_NO_KEY = 505,
    STATUS_ERR_NO_DATA = 506,
} status_code;

typedef struct response {
    int status_code;
    char *data;
} response;

char *response_to_string(response res);

#endif
