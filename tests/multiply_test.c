#include "../lib/greatest.h"
#include "../src/multiply.h"

TEST should_multiply_two_numbers() {
    int a = 2;
    int b = 2;

    ASSERT_EQ(multiply(a, b), 4);
    PASS();
}

SUITE(suite) {
    RUN_TEST(should_multiply_two_numbers);
}

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(suite);
    GREATEST_MAIN_END();
}