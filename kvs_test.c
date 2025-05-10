#include <stdio.h>
#include "kvs.h"

int main()
{
   KVStore *store = kvs_create();
   kvs_put(store, "hello", "{\"name\": \"kvstore\"}");
   void *ret = kvs_get(store, "hello");
   printf("%s\n", (char*)ret);
   kvs_destroy(store);
   return 0;
}
