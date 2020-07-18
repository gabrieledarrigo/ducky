#include <stdio.h>
#include "../lib/greatest.h"
#include "../src/response.h"

TEST should_convert_a_response_into_a_string(void) {
    char buffer[128];
    response responses[] = {
        { 200, "Lorem ipsum ipsum ipsum" },
        { 201, "STATUS_CREATED" },
        { 500 ,"ERR_UNKNOWN"},
        { 501 ,"ERR_CANNOT_RECV"},
        { 502 ,"ERR_COMMAND_NOT_RECOGNIZED"},
        { 503 ,"ERR_MAX_DATA_SIZE"},
        { 504 ,"ERR_KEY_LENGTH"},
        { 505 ,"ERR_NO_KEY"},
        { 505 ,"ERR_NO_DATA"},
    };

    for (int i = 0; i < sizeof(responses) / sizeof(responses[0]); i++) {
        sprintf(buffer, "%i %s\n", responses[i].status_code, responses[i].data);
        ASSERT_STR_EQ(buffer, response_to_string(responses[i]));
    }

    PASS();
}

SUITE(suite) {
    RUN_TEST(should_convert_a_response_into_a_string);
}

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(suite);
    GREATEST_MAIN_END();
}