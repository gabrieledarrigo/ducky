#include "../lib/greatest.h"
#include "../src/command.h"

TEST should_parse_a_SET_command_from_a_string(void) {
    command c;
    char buffer[] = "SET key 10 string";
    int result = parse_command(buffer, &c);

    ASSERT_EQ(0, result);
    ASSERT_EQ(SET, c.command_type);
    ASSERT_EQ(10, c.ttl);
    ASSERT_STR_EQ("key", c.key);
    ASSERT_STR_EQ("string", c.data);
    PASS();
}

TEST should_return_an_error_if_the_SET_command_is_malformed(void) {
    command c;
    char buffer[] = "SET key 10";
    int result = parse_command(buffer, &c);

    printf("result %i", result);

    ASSERT_EQ(ERR_SET_MALFORMED, result);
    PASS();
}

TEST should_return_an_error_if_ttl_is_lower_than_0(void) {
    command c;
    char buffer[] = "SET key -1 string";
    int result = parse_command(buffer, &c);

    ASSERT_EQ(ERR_TTL_ZERO, result);
    PASS();
}

//TEST should_return_an_error_if_data_is_empty(void) {
//    command c;
//    char buffer[] = "SET key 10 ";
//    int result = parse_command(buffer, &c);
//
//    ASSERT_EQ(ERR_DATA_EMPTY, result);
//    PASS();
//}

TEST should_parse_a_GET_command_from_a_string(void) {
    command c;
    char buffer[] = "GET key";
    int result = parse_command(buffer, &c);

    ASSERT_EQ(0, result);
    ASSERT_EQ(GET, c.command_type);
    ASSERT_STR_EQ("key", c.key);
    PASS();
}

TEST should_return_an_error_if_the_GET_command_has_not_the_associated_key(void) {
    command get;
    char buffer[] = "GET";
    int result = parse_command(buffer, &get);

    ASSERT_EQ(ERR_NO_KEY, result);
    PASS();
}

TEST should_return_an_error_if_the_GET_or_SET_key_length_is_greater_than_100_chars(void) {
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

SUITE(suite) {
    RUN_TEST(should_parse_a_SET_command_from_a_string);
    RUN_TEST(should_return_an_error_if_the_SET_command_is_malformed);
    RUN_TEST(should_return_an_error_if_ttl_is_lower_than_0);
//    RUN_TEST(should_return_an_error_if_data_is_empty);
    RUN_TEST(should_parse_a_GET_command_from_a_string);
    RUN_TEST(should_return_an_error_if_the_GET_or_SET_key_length_is_greater_than_100_chars);
    RUN_TEST(should_return_an_error_if_the_GET_command_has_not_the_associated_key);
}

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(suite);
    GREATEST_MAIN_END();
}