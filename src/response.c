#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "response.h"

char *response_to_string(response res) {
    // Allocate the len of data + 3 for the status code + 1 white space + 1 \n + 1 for the null terminator
    size_t to_allocate = (strlen(res.data) + 6) * sizeof(char);
    char *buffer = malloc(to_allocate);
    snprintf(buffer, to_allocate, "%i %s\n", res.status_code, res.data);

    return buffer;
}