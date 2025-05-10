#include "kvs.h"
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <math.h>

/* test only*/
const size_t INIT_POOL_SIZE = 7927;

static KVPair *makePair(const void *key, void *value)
{
    KVPair *pair = (KVPair*)malloc(sizeof(KVPair));
    pair->key = key;
    pair->value = value;
    pair->next = NULL;
    return pair;
}

static size_t hashKey(const void* key, size_t capacity)
{
    size_t hashVal = 0;
    size_t n = strlen(key);
    int i = 0;
    if (n > 25)
        n = 25;
    for (i = 0; i < n; i++) {
        hashVal = (hashVal << 5) - hashVal;
        hashVal += ((size_t*)key)[i];
    }
    return hashVal % capacity;
}

/* Find the next prime number larger than p */
static int prime (int p)
{
  int limit;
  p += (p & 1) + 1;
  limit = (int)sqrt((double) p) + 1;
  do {
      int j;
      while (limit * limit < p)
         limit++;
      for (j = 3; j <= limit && p % j; j += 2)
      ;
      if (j > limit)
         break;
      p += 2;
  } while (1);
  return p;
}

KVStore *kvs_create(void)
{
    KVStore *store = (KVStore*)malloc(sizeof(KVStore));
   
    store->pairs = (PKVPair)malloc(sizeof(KVPair) * INIT_POOL_SIZE);
    memset(store->pairs, 0, sizeof(KVPair) * INIT_POOL_SIZE);
    store->capacity = INIT_POOL_SIZE;
    store->used = 0;
    return store;
}

void rehash(KVStore *store)
{
    int i = 0; 
    PKVPair newPairs = (PKVPair)malloc(sizeof(KVPair) * INIT_POOL_SIZE);
    size_t newCapacity = prime(store->capacity << 1); 
    for (i = 0; i < store->capacity; i++) {
        if (store->pairs[i].key != NULL) {
            size_t hashVal = hashKey(store->pairs[i].key, newCapacity);
            newPairs[hashVal].key = store->pairs[i].key;
            newPairs[hashVal].value = store->pairs[i].value;
        }
    }
    free(store->pairs);
    store->pairs = newPairs;
    store->capacity = newCapacity;
}

int kvs_put(KVStore *store, const void *key, void *value)
{
    size_t hashVal = hashKey(key, store->capacity);
    KVPair *head = &store->pairs[hashVal];
    if (head->key != NULL) {
       KVPair *current = head;
       KVPair *prev = NULL;
       while (current != NULL) {
          prev = current;
          current = current->next;
       }
       prev->next = makePair(key, value);
    } else {
       store->pairs[hashVal].key = key;
       store->pairs[hashVal].value = value;
       store->used++;
       if(store->used > (store->capacity >> 1)) {
          rehash(store);
       }
    }
   return 0;
}

void *kvs_get(KVStore *store, const void *key)
{
    size_t hashVal = hashKey(key, store->capacity);
    KVPair *head = &store->pairs[hashVal];
    if (head->key != NULL) {
       KVPair *current = head;
       size_t n = strlen(key);
       while (current != NULL && memcmp(current->key, current->key, n) != 0)
          current = current->next;
       return current != NULL ? current->value : NULL;
    }
    return NULL;
}

void kvs_destroy(KVStore *store)
{
    free(store->pairs);
    free(store);
}
