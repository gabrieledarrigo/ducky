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
        { 502 ,"ERR_CANNOT_SEND"},
        { 503 ,"ERR_COMMAND_NOT_RECOGNIZED"},
        { 504 ,"ERR_MAX_DATA_SIZE"},
        { 505 ,"ERR_KEY_LENGTH"},
        { 506 ,"ERR_NO_KEY"},
        { 507 ,"ERR_NO_DATA"},
    };

    for (int i = 0; i < sizeof(responses) / sizeof(responses[0]); i++) {
        snprintf(buffer, 128, "%i %s\n", responses[i].status_code, responses[i].data);
        ASSERT_STR_EQ(buffer, response_to_string(responses[i]));
    }

    PASS();
}

TEST should_return_a_response_built_from_an_error(void) {
    char buffer[128];

    struct expected {
        error_t err;
        status_code code;
    };

    struct expected expected_items[] = {
        {{ ERR_UNKNOWN ,"ERR_UNKNOWN"}, STATUS_ERR_UNKNOWN},
        {{ ERR_CANNOT_RECV ,"ERR_CANNOT_RECV"}, STATUS_ERR_CANNOT_RECV},
        {{ ERR_CANNOT_SEND ,"ERR_CANNOT_SEND"}, STATUS_ERR_CANNOT_SEND},
        {{ ERR_COMMAND_NOT_RECOGNIZED ,"ERR_COMMAND_NOT_RECOGNIZED"}, STATUS_ERR_COMMAND_NOT_RECOGNIZED},
        {{ ERR_MAX_DATA_SIZE ,"ERR_MAX_DATA_SIZE"}, STATUS_ERR_MAX_DATA_SIZE},
        {{ ERR_KEY_LENGTH ,"ERR_KEY_LENGTH"}, STATUS_ERR_KEY_LENGTH},
        {{ ERR_NO_KEY ,"ERR_NO_KEY"}, STATUS_ERR_NO_KEY},
        {{ ERR_NO_DATA ,"ERR_NO_DATA"}, STATUS_ERR_NO_DATA},
        {{ ERR_NOT_FOUND ,"ERR_NOT_FOUND"}, STATUS_ERR_NOT_FOUND}
    };

    for (int i = 0; i < sizeof(expected_items) / sizeof(expected_items[0]); i++) {
        response res = errort_to_response(expected_items[i].err);

        int a = 1;
        ASSERT_EQ(expected_items[i].code, res.status_code);
    }

    PASS();
}

SUITE(suite) {
    RUN_TEST(should_convert_a_response_into_a_string);
    RUN_TEST(should_return_a_response_built_from_an_error);
}

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(suite);
    GREATEST_MAIN_END();
}