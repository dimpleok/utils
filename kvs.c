#include "kvs.h"
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
/* test only*/
const size_t INIT_POOL_SIZE = 1024;

static KVPair *makePair(const void *key, void *value)
{
    KVPair *pair = (KVPair*)malloc(sizeof(KVPair));
    pair->key = key;
    pair->value = value;
    pair->next = NULL;
    return pair;
}

KVStore *kvs_create(void)
{
    KVStore *store = (KVStore*)malloc(sizeof(KVStore));
   
    store->pairs = (PKVPair*)malloc(sizeof(PKVPair) * INIT_POOL_SIZE);
    memset(store->pairs, 0, sizeof(PKVPair) * INIT_POOL_SIZE);
    store->capacity = INIT_POOL_SIZE;
    return store;
}

// fIX ME
static size_t hashKey(const void* key, size_t capacity)
{
    size_t hash = 0;
    size_t n = strlen(key);
    int i = 0;
    for(i = 0; i < n; i++) {
        hash *= 23;
        hash += ((size_t*)key)[i];
    }
    return hash % capacity;
}

static void insertPair(KVStore *store, KVPair *pair)
{
    size_t hash = hashKey(pair->key, store->capacity);
    KVPair *head = store->pairs[hash];
    if(head) {
       KVPair *current = head;
       KVPair *prev = NULL;
       while(current) {
          prev = current;
          current = current->next;
       } 
       prev->next = pair;
    } else {
       store->pairs[hash] = pair;
    }
}

int kvs_put(KVStore *store, const void *key, void *value)
{
   KVPair *pair = makePair(key, value);
   insertPair(store, pair);
   return 0;
}

void *kvs_get(KVStore *store, const void *key)
{
    size_t hash = hashKey(key, store->capacity);
    KVPair *head = store->pairs[hash];
    if(head) {
       KVPair *current = head;
       size_t n = strlen(key);
       while(current && memcmp(current->key, current->key, n) != 0)
          current = current->next; 
       if(current == NULL) {
          return NULL;
       } else {
          return current->value;
       }
    } else {
        return NULL;
    }
}

void kvs_destroy(KVStore *store)
{
    int i = 0;
    for(i=0; i < store->capacity; i++) {
        KVPair *p =  store->pairs[i];
        while(p) {
           KVPair *current = p;
           p = p->next;
           free(current);
        }
    }
    free(store);
}
