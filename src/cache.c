#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "cache.h"

int hash(const char *string, int prime, int size) {
    long hash = 0;
    int len = (int) strlen(string);

    for (int i = 0; i < len; i++) {
        hash += (long) pow(prime, len - (i + 1)) * string[i]; // Compute a large integer number
        hash = hash % size; // Get the reminder
    }

    return (int) hash;
}

static int get_hash(const char *string, int num_nodes, int attempt) {
    int first_hash = hash(string, CACHE_PRIME_1, num_nodes);
    int second_hash = hash(string, CACHE_PRIME_2, num_nodes);

    // Compute a double hash, depending on the number of collisions
    return (first_hash + (attempt * (second_hash + 1))) % num_nodes;
}

static cache *cache_new_sized(int size) {
    cache *c = malloc(sizeof(cache));

    if (c == NULL) {
        perror("Cannot instantiate Cache data structure");
        exit(EXIT_FAILURE);
    }

    c->size = size;
    c->count = 0;
    c->nodes = calloc((size_t) c->size, sizeof(node *));

    if (c->nodes == NULL) {
        free(c);
        perror("Cannot instantiate Cache nodes");
        exit(EXIT_FAILURE);
    }

    return c;
}

cache *cache_new() {
    return cache_new_sized(INITIAL_CACHE_SIZE);
}

static void delete_node(node * n) {
    free(n->key);
    free(n->value);
    free(n);
}

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

static void resize(cache *to_resize, int size) {
    if (size < INITIAL_CACHE_SIZE) {
        return;
    }

    // Create a new temporary cache for the resizing
    cache *tmp_cache = cache_new_sized(size);

    //  Insert all nodes into the new cache
    for (int i = 0; i < to_resize->size; i++) {
        node *n = to_resize->nodes[i];

        if (n != NULL) {
            set(tmp_cache, n->key, n->value);
        }
    }

    // Swap between temp and actual cache
    int tmp_size = to_resize->size;
    to_resize->size = tmp_cache->size;
    tmp_cache->size = tmp_size;

    node **tmp_nodes = to_resize->nodes;
    to_resize->nodes = tmp_cache->nodes;
    tmp_cache->nodes = tmp_nodes;

    // Delete the temporary cache
    cache_delete(tmp_cache);
}

char* get(cache *c, const char *key) {
    int index = get_hash(key, c->size, 0);
    node *current_node = c->nodes[index];

    // Search for the node with the same key
    int i = 1;
    while (current_node != NULL) {
        if(strcmp(current_node->key, key) == 0) {
            return current_node->value;
        }

        index = get_hash(key, c->size, i);
        current_node = c->nodes[index];
        i++;
    }

    return NULL;
}

void set(cache *c, const char * key, const char *value) {
    int load = (c->count * 100) / c->size;

    // If the cache load is greater than 70% resize it up to the double
    // of the actual size
    if (load > 70) {
        resize(c, c->size * 2);
    }

    node *n = malloc(sizeof(node));

    if (n == NULL) {
        perror("Cannot instantiate a single node");
        exit(EXIT_FAILURE);
    }

    n->key = strdup(key);
    n->value = strdup(value);
    int index = get_hash(n->key, c->size, 0);
    node * current_node = c->nodes[index];

    // Search for an empty position in case of collision
    int i = 1;
    while (current_node != NULL) {
        // If an element with the same key exists,
        // delete it and insert the new one
        if (strcmp(current_node->key, key) == 0) {
            delete_node(current_node);
            c->nodes[index] = n;
            return;
        }

        index = get_hash(n->key, c->size, i); // Recalculate the hash with a different attempt value
        current_node = c->nodes[index];
        i++;
    }

    c->nodes[index] = n;
    c->count++;
}
