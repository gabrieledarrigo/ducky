#ifndef DUCKY_CACHE_H
#define DUCKY_CACHE_H

#define CACHE_PRIME_1 151
#define CACHE_PRIME_2 163
#define INITIAL_CACHE_SIZE 128

typedef struct node {
    char *key;
    char *value;
} node;

typedef struct cache {
    int count;
    int size;
    int load;
    node **nodes;
} cache;

int hash(const char * string, int prime, int size);

cache *cache_new();

void cache_delete(cache *c);

char* get(cache *c, const char *key);

void set(cache *c, const char *key, const char *value);

#endif
