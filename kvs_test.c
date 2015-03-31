#include "kvs.h"
#include "stdio.h"

int main()
{
   KVStore *store = kvs_create();
   int value = 99999;
   kvs_put(store, "hello", &value);
   kvs_put(store, 1, "wanjiuhi");
   void *ret = kvs_get(store, "hello");
   printf("ret=%d\n", *((int*)ret));
   kvs_destroy(store);
   return 0;
}
