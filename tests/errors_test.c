#include "../lib/greatest.h"
#include "../src/errors.h"

TEST should_return_the_related_error_t(void) {
    ASSERT_EQ(ERR_UNKNOWN, get_error_t(-1).code);
    ASSERT_STR_EQ("ERR_UNKNOWN", get_error_t(-1).message);

    ASSERT_EQ(ERR_CANNOT_RECV,  get_error_t(-2).code);
    ASSERT_STR_EQ("ERR_CANNOT_RECV",  get_error_t(-2).message);

    ASSERT_EQ(ERR_CANNOT_SEND,  get_error_t(-3).code);
    ASSERT_STR_EQ("ERR_CANNOT_SEND",  get_error_t(-3).message);

    ASSERT_EQ(ERR_COMMAND_NOT_RECOGNIZED,  get_error_t(-4).code);
    ASSERT_STR_EQ("ERR_COMMAND_NOT_RECOGNIZED",  get_error_t(-4).message);

    ASSERT_EQ(ERR_MAX_DATA_SIZE,  get_error_t(-5).code);
    ASSERT_STR_EQ("ERR_MAX_DATA_SIZE",  get_error_t(-5).message);

    ASSERT_EQ(ERR_KEY_LENGTH, get_error_t(-6).code);
    ASSERT_STR_EQ("ERR_KEY_LENGTH", get_error_t(-6).message);

    ASSERT_EQ(ERR_NO_KEY, get_error_t(-7).code);
    ASSERT_STR_EQ("ERR_NO_KEY", get_error_t(-7).message);

    ASSERT_EQ(ERR_NO_DATA, get_error_t(-8).code);
    ASSERT_STR_EQ("ERR_NO_DATA", get_error_t(-8).message);

    ASSERT_EQ(ERR_NOT_FOUND, get_error_t(-9).code);
    ASSERT_STR_EQ("ERR_NOT_FOUND", get_error_t(-9).message);
    PASS();
}

TEST should_return_ERR_UNKNOWN_if_error_code_is_not_recognized(void) {
    ASSERT_EQ(ERR_UNKNOWN, get_error_t(345).code);
    ASSERT_STR_EQ("ERR_UNKNOWN", get_error_t(345).message);
    PASS();
}

SUITE(suite) {
    RUN_TEST(should_return_the_related_error_t);
    RUN_TEST(should_return_ERR_UNKNOWN_if_error_code_is_not_recognized);
}

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(suite);
    GREATEST_MAIN_END();
}