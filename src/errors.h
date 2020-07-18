#ifndef DUCKY_ERRORS_H
#define DUCKY_ERRORS_H

typedef enum error_code {
    ERR_UNKNOWN = -1,
    ERR_CANNOT_RECV = -2,
    ERR_CANNOT_SEND = -3,
    ERR_COMMAND_NOT_RECOGNIZED = -4,
    ERR_MAX_DATA_SIZE = -5,
    ERR_KEY_LENGTH = -6,
    ERR_NO_KEY = -7,
    ERR_NO_DATA = -8,
    ERR_NOT_FOUND = -9,
} error_code;

typedef struct error_t {
    error_code code;
    char *message;
} error_t;

error_t get_error_t(int error_code);

#endif
