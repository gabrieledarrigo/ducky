#include "../lib/greatest.h"
#include "../src/cache.h"

TEST should_create_a_new_cache(void) {
    cache *c = cache_new();

    ASSERT_EQ(1024, c->size);
    ASSERT_EQ(0, c->count);
}

TEST should_return_the_same_hash_given_the_same_value(void) {
    int first = hash("string", CACHE_PRIME_1,1024);
    int second = hash("string", CACHE_PRIME_1,1024);

    ASSERT_EQ(first, second);
}

SUITE(suite) {
    RUN_TEST(should_return_the_same_hash_given_the_same_value);
//    RUN_TEST(should_create_a_new_cache);
}

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(suite);
    GREATEST_MAIN_END();
}