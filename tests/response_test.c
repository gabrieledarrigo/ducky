#include <stdio.h>
#include "../src/response.h"

int main() {
    response res;
    res.status_code = SUCCESS;
    res.data = "Lorem ipsum ipsum ipsum";

    char* str = response_to_string(res);

    printf("%s", str);
}