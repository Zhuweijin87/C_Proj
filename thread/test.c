#include "threadpool.h"

void *do_req(void *agv)
{
	printf("I am do request\n");
}

void *do_rsp(void *agv)
{
	printf("I am do resposne\n");
	
}

int main()
{
	struct threadpool *pool = NULL;
	if(threadpool_create(&pool, 2, 2) == -1){
		fprintf(stderr, "fail to create threadpool\n");
		return -1;
	}

	printf("create threadpool ok\n");

	while(1){	
		threadpool_jobadd(pool, do_req, NULL);
		threadpool_jobadd(pool, do_rsp, NULL);
	}

	while(1) {}
	return 0;
}
