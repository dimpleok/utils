#ifndef __KVS_H
#define __KVS_H

typedef unsigned long size_t;

typedef struct KVPair {
    const void *key;
    void *value;
    struct KVPair* next;
}KVPair, *PKVPair;

typedef struct {
    PKVPair *pairs;
    size_t capacity;
}KVStore;

KVStore *kvs_create(void);
int kvs_put(KVStore *store, const void *key, void *value);
void *kvs_get(KVStore *store, const void *key);
void kvs_destroy(KVStore *store);

#endif
