#include "../lib/greatest.h"
#include "../src/cache.h"

TEST should_create_a_new_cache(void) {
    cache *c = cache_new();

    ASSERT_EQ(INITIAL_CACHE_SIZE, c->size);
    ASSERT_EQ(0, c->count);

    cache_delete(c);
    PASS();
}

TEST should_insert_a_new_node_in_the_cache(void) {
    cache *c = cache_new();

    set(c, "key", "value");

    ASSERT_EQ(1, c->count);

    for (int i = 0; i < c->size; i++) {
        node * current_node = c->nodes[i];

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
    char key[10];
    char value[10];

    for(int i = 0; i < INITIAL_CACHE_SIZE; i++) {
        sprintf(key, "key %i", i);
        sprintf(value, "value %i", i);

        set(c, key, value);
    }

    ASSERT_EQ(INITIAL_CACHE_SIZE, c->count);

    cache_delete(c);
    PASS();
}

TEST should_update_a_value_with_the_same_key(void) {
    cache *c = cache_new();

    set(c, "key", "value1");
    set(c, "key", "value2");

    ASSERT_EQ(1, c->count);

    for (int i = 0; i < c->size; i++) {
        node * current_node = c->nodes[i];

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
    char * value = get(c, "key");

    ASSERT_STR_EQ("value", value);

    cache_delete(c);
    PASS();
}

TEST should_return_NULL_if_the_associated_key_doesnt_exists(void) {
    cache *c = cache_new();
    char * value = get(c, "key");

    ASSERT_EQ(NULL, value);

    cache_delete(c);
    PASS();
}

//TEST should_delete_all_element_in_the_cache(void) {
//    cache *c = cache_new();
//
//    set(c, "key", "value1");
//    set(c, "key", "value2");
//    cache_delete(c);
//
//    ASSERT_EQ(0, c->size);
//    ASSERT_EQ(0, c->count);
//    PASS();
//}

TEST should_return_the_same_hash_given_the_same_value(void) {
    int first = hash("string", CACHE_PRIME_1,1024);
    int second = hash("string", CACHE_PRIME_1,1024);

    ASSERT_EQ(first, second);
    PASS();
}

SUITE(suite) {
    RUN_TEST(should_return_the_same_hash_given_the_same_value);
    RUN_TEST(should_create_a_new_cache);
    RUN_TEST(should_insert_a_new_node_in_the_cache);
    RUN_TEST(should_update_a_value_with_the_same_key);
    RUN_TEST(should_return_the_value_of_the_associated_key_if_it_exists);
    RUN_TEST(should_return_NULL_if_the_associated_key_doesnt_exists);
    RUN_TEST(should_support_the_insertion_of_a_lot_of_entry);
//    RUN_TEST(should_delete_all_element_in_the_cache);
}

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(suite);
    GREATEST_MAIN_END();
}