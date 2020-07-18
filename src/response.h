#ifndef DUCKY_RESPONSE_H
#define DUCKY_RESPONSE_H

#include "errors.h"

typedef enum status_code {
    STATUS_SUCCESS = 200,
    STATUS_CREATED = 201,
    STATUS_ERR_UNKNOWN = 500,
    STATUS_ERR_CANNOT_RECV = 501,
    STATUS_ERR_CANNOT_SEND = 502,
    STATUS_ERR_COMMAND_NOT_RECOGNIZED = 503,
    STATUS_ERR_MAX_DATA_SIZE = 504,
    STATUS_ERR_KEY_LENGTH = 505,
    STATUS_ERR_NO_KEY = 506,
    STATUS_ERR_NO_DATA = 507,
    STATUS_ERR_NOT_FOUND = 508,
} status_code;

typedef struct response {
    int status_code;
    char *data;
} response;

char *response_to_string(response res);
response errort_to_response(error_t error);

#endif
