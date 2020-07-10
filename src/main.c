#include <stdio.h>
#include <strings.h>
#include "command.h"

void main() {
    char buffer[1024];
    char ** arr;
    command c;
    bzero(&c, sizeof(c));

    printf("Enter a command: \n");
    fgets(buffer, 1024, stdin);

    printf("Buffer is %s", buffer);
    parse_command(buffer, &c);
}