#include "../lib/greatest.h"
#include "../src/prime.h"

TEST should_return_true_if_n_is_a_prime_number(void) {
    ASSERT_EQ(1, is_prime(2));
    ASSERT_EQ(1, is_prime(3));
    ASSERT_EQ(1, is_prime(5));
    ASSERT_EQ(1, is_prime(7));
    ASSERT_EQ(1, is_prime(11));
    ASSERT_EQ(1, is_prime(19));

    PASS();
}

TEST should_return_false_if_n_is_a_not_a_prime_number(void) {
    ASSERT_EQ(0, is_prime(-11));
    ASSERT_EQ(0, is_prime(-1));
    ASSERT_EQ(0, is_prime(1));
    ASSERT_EQ(0, is_prime(4));
    ASSERT_EQ(0, is_prime(6));
    ASSERT_EQ(0, is_prime(6));
    ASSERT_EQ(0, is_prime(15));
    PASS();
}

TEST should_return_the_next_prime_number_unless_n_is_a_prime_number(void) {
    ASSERT_EQ(2, next_prime(1));
    ASSERT_EQ(2, next_prime(2));
    ASSERT_EQ(3, next_prime(3));
    ASSERT_EQ(5, next_prime(4));
    ASSERT_EQ(7, next_prime(6));
    ASSERT_EQ(11, next_prime(10));
    PASS();
}

SUITE(suite) {
    RUN_TEST(should_return_true_if_n_is_a_prime_number);
    RUN_TEST(should_return_false_if_n_is_a_not_a_prime_number);
    RUN_TEST(should_return_the_next_prime_number_unless_n_is_a_prime_number);
}

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(suite);
    GREATEST_MAIN_END();
}