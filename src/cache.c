#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "cache.h"
#include "prime.h"
#include "logger.h"

/**
 * Takes a string as an input and:
 *  Converts it into a large integer number.
 *  Reduces the size of the integer to a fixed range, by taking its remainder mod m, where m is the number of buckets of the hash table.
 *
 * @param string    the string from which the hash is computed
 * @param prime     a prime number that is used to calculate the large integer
 * @param size      the number of nodes in the cache
 * @return          an
 */
int hash(const char *string, int prime, int size) {
    long hash = 0;
    int len = (int) strlen(string);

    for (int i = 0; i < len; i++) {
        hash += (long) pow(prime, len - (i + 1)) * string[i]; // Compute a large integer number
        hash = hash % size; // Get the reminder
    }

    return (int) hash;
}

/**
 * Compute a double hash, where the result depends on the number of collisions happened.
 *
 * @param string        the string from which the hash is computed
 * @param num_nodes     the number of nodes in the cache
 * @param attempt       the number of collided items. It
 * @return              an index that points to the position where the new item of the cache will be stored
 */
static int get_hash(const char *string, int num_nodes, int attempt) {
    int first_hash = hash(string, CACHE_PRIME_1, num_nodes);
    int second_hash = hash(string, CACHE_PRIME_2, num_nodes);

    if (second_hash % num_nodes == 0) {
        second_hash = 1;
    }

    // Compute a double hash, depending on the number of collisions
    return (first_hash + (attempt * (second_hash))) % num_nodes;
}

/**
 * Create a new cache of the desired size.
 *
 * @param size  the size of the cache; ie: the initial number of nodes tha it has.
 * @return      the created cache
 */
static cache *cache_new_sized(int size) {
    cache *c = malloc(sizeof(cache));

    if (c == NULL) {
        logs(LOG_FATAL, "Cannot allocate to create new cache data structure: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    c->size = next_prime(size);
    c->count = 0;
    c->load = 0;
    c->nodes = calloc((size_t) c->size, sizeof(node *));

    if (c->nodes == NULL) {
        free(c);
        logs(LOG_FATAL, "Cannot allocate to create cache nodes: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return c;
}

/**
 * Create a new cache with an initial size equal to 131 (the next prime after INITIAL_CACHE_SIZE)
 *
 * @return a cache struct
 */
cache *cache_new() {
    return cache_new_sized(INITIAL_CACHE_SIZE);
}

/**
 * Free a node from the memory.
 *
 * @param n the node that must be deleted
 */
static void delete_node(node *n) {
    free(n->key);
    free(n->value);
    free(n);
}

/**
 * Delete all nodes from the cache and then free the cache itself from the memory.
 *
 * @param c the cache to be deleted
 */
void cache_delete(cache *c) {
    for (int i = 0; i < c->size; i++) {
        node *n = c->nodes[i];

        if (n != NULL) {
            delete_node(n);
        }
    }

    free(c->nodes);
    free(c);
}

/**
 * Resize the cache to the new size.
 * First create a temporary cache where all old cache nodes are stored.
 * Then create a new cache of the desired size, copy all nodes from the temp cache to the new one, and delete the old cache.
 *
 * @param c     the cache that needs to be resize
 * @param size  the new cache size
 */
static void resize(cache *c, int size) {
    if (size < INITIAL_CACHE_SIZE) {
        return;
    }

    // Create a new temporary cache for the resizing
    cache *tmp_cache = cache_new_sized(size);

    //  Insert all nodes into the new cache
    for (int i = 0; i < c->size; i++) {
        node *n = c->nodes[i];

        if (n != NULL) {
            set(tmp_cache, n->key, n->value);
        }
    }

    // Set new count and load value
    c->count = tmp_cache->count;
    c->load = tmp_cache->load;

    // Swap between temp and actual cache
    int tmp_size = c->size;
    c->size = tmp_cache->size;
    tmp_cache->size = tmp_size;

    node **tmp_nodes = c->nodes;
    c->nodes = tmp_cache->nodes;
    tmp_cache->nodes = tmp_nodes;

    // Delete the temporary cache
    cache_delete(tmp_cache);
}

/**
 * Get a value from the cache related to the key parameter.
 *
 * @param c     the cache where the item is stored
 * @param key   the key associated to the value that a consumer want to retrieve
 * @return      the desired value or NULL if it cannot be found
 */
char *get(cache *c, const char *key) {
    int index = get_hash(key, c->size, 0);
    node *current_node = c->nodes[index];

    // Search for the node with the same key
    int i = 1;
    while (current_node != NULL) {
        if (strcmp(current_node->key, key) == 0) {
            return current_node->value;
        }

        index = get_hash(key, c->size, i);
        current_node = c->nodes[index];
        i++;
    }

    return NULL;
}

/**
 * Store a value, related to key, in the cache.
 * If the cache load is greater than 70% it's automatically doubled up in size.
 *
 * @param c     the cache where the item will be stored
 * @param key   the key associated to the value that a consumer want to store
 * @param value the value that must be stored
 */
void set(cache *c, const char *key, const char *value) {
    // Calculate and set the cache load
    int load = (c->count * 100) / c->size;
    c->load = load;

    // If the cache load is greater than 70% resize it up to the double
    // of the actual size
    if (load > 70) {
        resize(c, c->size * 2);
    }

    node *new_node = malloc(sizeof(node));

    if (new_node == NULL) {
        logs(LOG_FATAL, "Cannot allocate to create a single cache node: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    new_node->key = strdup(key);
    new_node->value = strdup(value);
    int index = get_hash(new_node->key, c->size, 0);
    node *current_node = c->nodes[index];

    // Search for an empty position in case of collision
    int i = 1;
    while (current_node != NULL) {
        // If an element with the same key exists,
        // delete it and insert the new one
        if (strcmp(current_node->key, key) == 0) {
            delete_node(current_node);
            c->nodes[index] = new_node;
            return;
        }

        index = get_hash(new_node->key, c->size, i); // Recalculate the hash with a different attempt value
        current_node = c->nodes[index];
        i++;
    }

    c->nodes[index] = new_node;
    c->count++;
}
