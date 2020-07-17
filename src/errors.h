#ifndef DUCKY_ERRORS_H
#define DUCKY_ERRORS_H

typedef enum error_code {
    ERR_UNKNOWN = -1,
    ERR_CANNOT_RECV = -2,
    ERR_COMMAND_NOT_RECOGNIZED = -3,
    ERR_MAX_DATA_SIZE = -4,
    ERR_KEY_LENGTH = -5,
    ERR_NO_KEY = -6,
    ERR_NO_DATA = -7,
} error_code;

typedef struct error_t {
    error_code code;
    char *message;
} error_t;

error_t get_error_t(int error_code);

#endif
