#include "mempool.h"

int main()
{
	int  		ret;
	mempool_t	*pool;

	ret = mempool_create(&pool, 0x5000);
	if(ret)
		return -1;
	
	printf("init: %p\n", pool->ptr_init);
	
	char *a = (char *)mempool_alloc(pool, 1000);	
	char *b = (char *)mempool_alloc(pool, 2500);
	char *c = (char *)mempool_alloc(pool, 1200);
	char *d = (char *)mempool_alloc(pool, 800);	
	showmempool(pool);

	mempool_free(pool, &b);
	printf("delete d: %p\n", b);

	showmempool(pool);

	return 0;
}
