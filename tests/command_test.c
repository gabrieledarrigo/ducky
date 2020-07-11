#include "../lib/greatest.h"
#include "../src/command.h"

TEST should_parse_a_SET_command_from_a_string(void) {
    command c;
    char buffer[] = "SET key 10 string";
    int result = parse_command(buffer, &c);

    ASSERT_EQ(0, result);
    ASSERT_EQ(SET, c.command_type);
    ASSERT_STR_EQ("key", c.key);
    ASSERT_STR_EQ("string", c.data);
    PASS();
}

TEST should_return_ERR_NO_KEY_if_the_SET_command_has_not_an_associated_key(void) {
    command c;
    char buffer[] = "SET";
    int result = parse_command(buffer, &c);

    ASSERT_EQ(ERR_NO_KEY, result);
    PASS();
}

TEST should_return_ERR_NO_DATA_if_the_SET_command_has_no_data_associated(void) {
    command c;
    char buffer[] = "SET key 10 ";
    int result = parse_command(buffer, &c);

    ASSERT_EQ(ERR_NO_DATA, result);
    PASS();
}

TEST should_parse_a_GET_command_from_a_string(void) {
    command c;
    char buffer[] = "GET key";
    int result = parse_command(buffer, &c);

    ASSERT_EQ(0, result);
    ASSERT_EQ(GET, c.command_type);
    ASSERT_STR_EQ("key", c.key);
    PASS();
}

TEST should_return_ERR_NO_KEY_if_the_GET_command_has_not_an_associated_key(void) {
    command get;
    char buffer[] = "GET";
    int result = parse_command(buffer, &get);

        ASSERT_EQ(ERR_NO_KEY, result);
        PASS();
}

TEST should_return_ERR_KEY_LENGTH_if_the_GET_or_SET_key_length_is_greater_than_100_chars(void) {
    int result;
    command get, set;
    char get_buffer[] = "GET abcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdebcde 10 string";
    char set_buffer[] = "SET abcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdebcde 10 string";

    result = parse_command(get_buffer, &get);

    ASSERT_EQ(ERR_KEY_LENGTH, result);

    result = parse_command(set_buffer, &set);

    ASSERT_EQ(ERR_KEY_LENGTH, result);
    PASS();
}

TEST should_return_ERR_COMMAND_NOT_RECOGNIZED_if_the_command_is_not_recognized() {
    command c;
    char buffer[] = "FOO BAR key 10 20";
    int result = parse_command(buffer, &c);

        ASSERT_EQ(ERR_COMMAND_NOT_RECOGNIZED, result);
        PASS();
}

SUITE(suite) {
    RUN_TEST(should_parse_a_SET_command_from_a_string);
    RUN_TEST(should_return_ERR_NO_KEY_if_the_SET_command_has_not_an_associated_key);
    RUN_TEST(should_return_ERR_NO_DATA_if_the_SET_command_has_no_data_associated);
    RUN_TEST(should_parse_a_GET_command_from_a_string);
    RUN_TEST(should_return_ERR_NO_KEY_if_the_GET_command_has_not_an_associated_key);
    RUN_TEST(should_return_ERR_KEY_LENGTH_if_the_GET_or_SET_key_length_is_greater_than_100_chars);
    RUN_TEST(should_return_ERR_COMMAND_NOT_RECOGNIZED_if_the_command_is_not_recognized);
}

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(suite);
    GREATEST_MAIN_END();
}