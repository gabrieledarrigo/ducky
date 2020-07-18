#include "../lib/greatest.h"
#include "../src/prime.h"
#include "../src/cache.h"

TEST should_create_a_new_cache(void) {
    cache *c = cache_new();

    ASSERT_EQ(next_prime(INITIAL_CACHE_SIZE), c->size);
    ASSERT_EQ(0, c->count);
    ASSERT_EQ(0, c->load);

    cache_delete(c);
    PASS();
}

TEST should_insert_a_new_node_in_the_cache(void) {
    cache *c = cache_new();

    set(c, "key", "value");

    ASSERT_EQ(1, c->count);

    for (int i = 0; i < c->size; i++) {
        node *current_node = c->nodes[i];

        if (c->nodes[i] != NULL) {
            ASSERT_STR_EQ("key", current_node->key);
            ASSERT_STR_EQ("value", current_node->value);
        }
    }

    cache_delete(c);
    PASS();
}

TEST should_support_the_insertion_of_a_lot_of_entry(void) {
    cache *c = cache_new();
    char key[100];
    char value[100];

    for (int i = 0; i < 100000; i++) {
        sprintf(key, "key %i", i);
        sprintf(value, "value %i", i);

//        clock_t start = clock();
        set(c, key, "value");
//        clock_t end = clock();
//        printf("Insertion %i, elapsed: %f seconds\n", i, (double)(end - start) / CLOCKS_PER_SEC);
    }

    ASSERT_EQ(100000, c->count);

    cache_delete(c);
    PASS();
}

TEST should_update_a_value_with_the_same_key(void) {
    cache *c = cache_new();

    set(c, "key", "value1");
    set(c, "key", "value2");

    ASSERT_EQ(1, c->count);

    for (int i = 0; i < c->size; i++) {
        node *current_node = c->nodes[i];

        if (c->nodes[i] != NULL) {
            ASSERT_STR_EQ("key", current_node->key);
            ASSERT_STR_EQ("value2", current_node->value);
        }
    }

    cache_delete(c);
    PASS();
}

TEST should_return_the_value_of_the_associated_key_if_it_exists(void) {
    cache *c = cache_new();

    set(c, "key", "value");
    char *value = get(c, "key");

    ASSERT_STR_EQ("value", value);

    cache_delete(c);
    PASS();
}

TEST should_return_NULL_if_the_associated_key_doesnt_exists(void) {
    cache *c = cache_new();
    char *value = get(c, "key");

    ASSERT_EQ(NULL, value);

    cache_delete(c);
    PASS();
}

TEST should_resize_up_to_the_double_size_when_the_cache_is_full_at_70_percent(void) {
    cache *c = cache_new();

    char key[10];
    char value[10];
    int fill = 94; // 93 is 70% of computed INITIAL_CACHE_SIZE(which is the next prime number of 128, ie: 131)

    for (int i = 0; i < fill; i++) {
        sprintf(key, "key %i", i);
        sprintf(value, "value %i", i);

        set(c, key, value);
    }

    int expected_size = next_prime(INITIAL_CACHE_SIZE);

    ASSERT_EQ(expected_size, c->size);
    ASSERT_EQ(fill, c->count);
    ASSERT_EQ(70, c->load);

    set(c, "key resize", "value resize");

    ASSERT_EQ(next_prime(expected_size * 2), c->size);
    ASSERT_EQ(++fill, c->count);

    cache_delete(c);
    PASS();
}

TEST should_return_the_same_hash_given_the_same_value(void) {
    int first_hash = hash("string", CACHE_PRIME_1, INITIAL_CACHE_SIZE);
    int second_hash = hash("string", CACHE_PRIME_1, INITIAL_CACHE_SIZE);

    ASSERT_EQ(first_hash, second_hash);
    PASS();
}

SUITE (suite) {
    RUN_TEST(should_create_a_new_cache);
    RUN_TEST(should_insert_a_new_node_in_the_cache);
    RUN_TEST(should_update_a_value_with_the_same_key);
    RUN_TEST(should_return_the_value_of_the_associated_key_if_it_exists);
    RUN_TEST(should_return_NULL_if_the_associated_key_doesnt_exists);
    RUN_TEST(should_support_the_insertion_of_a_lot_of_entry);
    RUN_TEST(should_resize_up_to_the_double_size_when_the_cache_is_full_at_70_percent);
    RUN_TEST(should_return_the_same_hash_given_the_same_value);
}

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(suite);
    GREATEST_MAIN_END();
}