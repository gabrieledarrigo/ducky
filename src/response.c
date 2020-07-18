#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "errors.h"
#include "logger.h"
#include "response.h"

char *response_to_string(response res) {
    // Allocate the len of data + 3 for the status code + 1 white space + 1 \n + 1 for the null terminator
    size_t to_allocate = (strlen(res.data) + 6) * sizeof(char);
    char *buffer = malloc(to_allocate);

    if (buffer == NULL) {
        logs(LOG_ERROR, "Cannot allocate for string response: ", strerror(errno));
    }

    snprintf(buffer, to_allocate, "%i %s\n", res.status_code, res.data);

    return buffer;
}

response errort_to_response(error_t error) {
    response res;

    switch (error.code) {
        case ERR_UNKNOWN:
            res.status_code = STATUS_ERR_UNKNOWN;
            break;
        case ERR_CANNOT_RECV:
            res.status_code = STATUS_ERR_CANNOT_RECV;
            break;
        case ERR_CANNOT_SEND:
            res.status_code = STATUS_ERR_CANNOT_SEND;
            break;
        case ERR_COMMAND_NOT_RECOGNIZED:
            res.status_code = STATUS_ERR_COMMAND_NOT_RECOGNIZED;
            break;
        case ERR_MAX_DATA_SIZE:
            res.status_code = STATUS_ERR_MAX_DATA_SIZE;
            break;
        case ERR_KEY_LENGTH:
            res.status_code = STATUS_ERR_KEY_LENGTH;
            break;
        case ERR_NO_KEY:
            res.status_code = STATUS_ERR_NO_KEY;
            break;
        case ERR_NO_DATA:
            res.status_code = STATUS_ERR_NO_DATA;
            break;
        case ERR_NOT_FOUND:
            res.status_code = STATUS_ERR_NOT_FOUND;
            break;
        default:
            res.status_code = STATUS_ERR_UNKNOWN;
    }

    res.data = error.message;
    return res;
}