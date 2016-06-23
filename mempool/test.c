#include "mempool.h"

int main()
{
	int  		ret;
	mempool_t	*pool;
	
	ret = mempool_create(&pool, 0x2000);
	if(ret)
		return -1;
	
	char *v = (char *)mempool_alloc(pool, 1000);
	
	char *p = (char *)mempool_alloc(pool, 2500);

	return 0;
}
