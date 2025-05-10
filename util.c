#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "util.h"

MemChunk *g_chunk = NULL;

static void initialChunk(MemChunk** chunk, size_t totalSize)
{
    *chunk = (MemChunk*) malloc(sizeof(MemChunk));
    if (*chunk == NULL )
        return;

    (*chunk)->mem = malloc(totalSize);
    if ((*chunk)->mem != NULL) {
        memset( (*chunk)->mem, '\x0', totalSize);
        (*chunk)->totalSize = (*chunk)->freeSize = totalSize;
    } else {
        (*chunk)->totalSize = (*chunk)->freeSize = 0;
    }
} 

// This method should be called to reclaim memory
void destroyChunk()
{
    MemChunk **chunk = &g_chunk;

    if (*chunk == NULL)
        return;
    if ((*chunk)->mem != NULL) {
        free((*chunk)->mem);
        (*chunk)->mem = NULL;
        //printf("%d bytes reclaimed\n", (*chunk)->totalSize);
        //printf("%d bytes used\n", (*chunk)->totalSize - (*chunk)->freeSize);
        free(*chunk);
        *chunk = NULL;
    }
}

static void* requestMem(MemChunk **chunk, size_t size)
{
    // Initial the chunk if required
    if (*chunk == NULL) {
        initialChunk(chunk, CHUNK_SIZE);
        if( *chunk == NULL) {
           return NULL;
        }
    }

    // Only for small memory allocation
    if (size > 1024) {
        fprintf(stderr, "Only support small memory (<1024 Bytes) allocation\n");
        return NULL;
    }

    // Acquire memory from chunk
    if (size > (*chunk)->freeSize) {
        return NULL;
    } else {
        size_t offset = (*chunk)->totalSize - (*chunk)->freeSize;
        char *p = &(*chunk)->mem[offset];
        (*chunk)->freeSize -= size;
        return (void*)p;
    } 
}

void* _malloc(size_t size)
{
    return requestMem(&g_chunk, size);
}

char* _strdup(const char* src)
{
    int n = strlen(src);
    char *p = (char*)_malloc(n+1);
    strncpy(p, src, n); 
    return p;
}

int addKeyValue(KeyVal **head, const char *key, const char *val) 
{
    // New node containing key and value
    KeyVal *kv = (KeyVal*)_malloc(sizeof(KeyVal));
    memset(kv, '\x0', sizeof(KeyVal));
    kv->key = _strdup(key);
    kv->value = _strdup(val);

    // Put the node into list
    if (*head == NULL) {
        *head = kv;
    } else {
        kv->next = *head;
        *head = kv;
    } 
    return 0;
}

int containsKey(KeyVal *head, const char* key)
{
    // Iterate the list and find the key
    while (head != NULL) {
        if (strcmp(head->key, key) == 0) {
           return 1;
        } else {
           head = head->next;
        }
    }
    return 0;
}

char* getValue(KeyVal *head, const char* key)
{
    // Iterate the list and find the key
    char *value = NULL;
    while (head != NULL) {
        if (strcmp(head->key, key) == 0) {
           value = head->value; 
           break;
        } else {
           head = head->next;
        }
    }
    return value;
}

void getArguments(int argc, char* argv[], int keyc, const char* keyMap[], KeyVal **keyValues)
{
    int opt;
    int i = 0;
    char optstring[128];
    KeyVal *kv = NULL;
    char key[2];

    key[1] = '\x0';
    memset(optstring, '\x0', sizeof(optstring));
    for (i = 0; i < keyc; i++) {
        snprintf(&optstring[i*2], 3, "%c:", keyMap[i][0]);
        key[0] = keyMap[i][0];
        addKeyValue(&kv, key, keyMap[i] + 2);
    }
    while ((opt = getopt(argc, argv, optstring)) != -1) {
        key[0] = opt;
        if (containsKey(kv, key)) {
            addKeyValue(keyValues, getValue(kv, key), optarg);
        }
    }
}
