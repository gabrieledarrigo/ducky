#include "../lib/greatest.h"
#include "../src/sum.h"

TEST should_add_two_numbers(void) {
    int a = 1;
    int b = 2;

    ASSERT_EQ(sum(a, b), 3);
    PASS();
}

SUITE(suite) {
    RUN_TEST(should_add_two_numbers);
}

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(suite);
    GREATEST_MAIN_END();
}