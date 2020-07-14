#ifndef DUCKY_CACHE_H
#define DUCKY_CACHE_H

#define CACHE_PRIME_1 23893
#define CACHE_PRIME_2 42937

typedef struct node {
    char *key;
    char *value;
} node;

typedef struct cache {
    int count;
    int size;
    node **nodes;
} cache;

int hash(const char * string, int prime, int size);

cache *cache_new();

char *get(cache c, const char *key);

char *set(cache c, const char *key, const char *value);

#endif
