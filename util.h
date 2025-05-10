#ifndef __UTIL_H_
#define __UTIL_H_
#include <stdbool.h>

#define CHUNK_SIZE 1024*1024*16

typedef struct _MemChunk
{
    size_t freeSize;
    size_t totalSize;
    char  *mem;
}MemChunk;

typedef struct keyVal
{
    char *key;
    char *value;
    struct keyVal *next;
}KeyVal;

extern void  destroyChunk();
extern void *_malloc(size_t size);
extern char *_strdup(const char *src);

extern int addKeyValue(KeyVal **head, const char *key, const char *val) ;
extern char *getValue(KeyVal *head, const char *key);
extern int containsKey(KeyVal *head, const char *key);

extern void getArguments(int argc, char* argv[], int keyc, const char* keyMap[], KeyVal **keyValues);
/*
    KeyVal *kv = NULL;
    const char* keyMap[] = {"u:userName", "p:password", "d:domainName"};
    getArguments(argc, argv, 3, keyMap, &kv);
    if( !containsKey(kv, "userName")  || !containsKey(kv, "password") || !containsKey(kv, "domainName")) {
        printf("Usage: program -u USERNAME -p PASSWORD -d DOMAINNAME\n");
        goto CLEANUP;
        return 0;
    }
*/

extern MemChunk *g_chunk;

#endif
