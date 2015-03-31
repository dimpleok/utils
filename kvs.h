#ifndef __KVS_H
#define __KVS_H
#include <stdio.h>

typedef struct KVPair {
    const void *key;
    void *value;
    struct KVPair* next;
}KVPair, *PKVPair;

typedef struct {
    PKVPair *pairs;
    size_t capacity;
    size_t used;
}KVStore;

KVStore *kvs_create(void);
int kvs_put(KVStore *store, const void *key, void *value);
void *kvs_get(KVStore *store, const void *key);
void kvs_destroy(KVStore *store);

#endif
