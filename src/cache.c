#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "cache.h"

int hash(const char *string, int prime, int size) {
    long hash = 0;
    int len = (int) strlen(string);

    for (int i = 0; i < len; i++) {
        hash += (long) pow(prime, len - (i + 1) * string[i]); // Compute a large integer number
        hash = hash % size; // Get the reminder
    }

    return (int) hash;
}

static int get_hash(const char *string, int num_nodes, int attempt) {
    int first_hash = hash(string, CACHE_PRIME_1, num_nodes);
    int second_hash = hash(string, CACHE_PRIME_2, num_nodes);

    // Compute a double hash, depending on the number of collisions
    return (first_hash + attempt * (second_hash + 1)) % num_nodes;
}

cache *cache_new() {
    cache *c = malloc(sizeof(cache));

    if (c == NULL) {
        perror("Cannot instantiate Cache data structure");
        exit(EXIT_FAILURE);
    }

    c->size = 1024;
    c->count = 0;
    c->nodes = calloc((size_t) c->size, sizeof(node *));

    if (c->nodes == NULL) {
        free(c);
        perror("Cannot instantiate Cache nodes");
        exit(EXIT_FAILURE);
    }

    return c;
}