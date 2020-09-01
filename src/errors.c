#include "errors.h"

/**
 * Create an error_t struct where the code and message members values depends on the value of the error_code parameter.
 *
 * @param error_code    an integer representing a Ducky internal error code
 * @return              an error_t struct
 */
error_t get_error_t(int error_code) {
    error_t err;

    switch (error_code) {
        case ERR_UNKNOWN:
            err.code = ERR_UNKNOWN;
            err.message = "ERR_UNKNOWN";
            break;
        case ERR_CANNOT_RECV:
            err.code = ERR_CANNOT_RECV;
            err.message = "ERR_CANNOT_RECV";
            break;
        case ERR_CANNOT_SEND:
            err.code = ERR_CANNOT_SEND;
            err.message = "ERR_CANNOT_SEND";
            break;
        case ERR_COMMAND_NOT_RECOGNIZED:
            err.code = ERR_COMMAND_NOT_RECOGNIZED;
            err.message = "ERR_COMMAND_NOT_RECOGNIZED";
            break;
        case ERR_MAX_DATA_SIZE:
            err.code = ERR_MAX_DATA_SIZE;
            err.message = "ERR_MAX_DATA_SIZE";
            break;
        case ERR_KEY_LENGTH:
            err.code = ERR_KEY_LENGTH;
            err.message = "ERR_KEY_LENGTH";
            break;
        case ERR_NO_KEY:
            err.code = ERR_NO_KEY;
            err.message = "ERR_NO_KEY";
            break;
        case ERR_NO_DATA:
            err.code = ERR_NO_DATA;
            err.message = "ERR_NO_DATA";
            break;
        case ERR_NOT_FOUND:
            err.code = ERR_NOT_FOUND;
            err.message = "ERR_NOT_FOUND";
            break;
        default:
            err.code = ERR_UNKNOWN;
            err.message = "ERR_UNKNOWN";
    }

    return err;
}