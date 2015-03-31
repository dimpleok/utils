#include "kvs.h"
#include "stdio.h"

int main()
{
   KVStore *store = kvs_create();
   //int value = 99999;
   kvs_put(store, "hello", "{\"name\": \"kvstore\"}");
   void *ret = kvs_get(store, "hello");
   //printf("ret=%d\n", *((int*)ret));
   printf("%s\n", (char*)ret);
   kvs_destroy(store);
   return 0;
}
