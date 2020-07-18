#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "response.h"

char *response_to_string(response res) {
    // Allocate the len of data + 3 for the status code + 1 for the null terminator
    char *buffer = malloc(strlen(res.data) + 4);
    sprintf(buffer, "%i %s\n", res.status_code, res.data);
    buffer[strlen(buffer)] = '\0';

    return buffer;
}