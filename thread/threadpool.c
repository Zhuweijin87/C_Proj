#include "threadpool.h"

int threadpool_create(struct threadpool *tpool, int thread_size, int max_jobs)
{
	tpool = malloc(sizeof(struct threadpool));
	if(NULL == tpool) return -1;

	tpool->th_size = thread_size;
	tpool->job_size = max_jobs;
	
	if(pthread_mutex_init(tpool->lock, NULL) == -1){
		return -1;
	}

	if(pthread_cond_init(tpool->job_is_full, NULL) == -1){
		return -1;
	}

	if(pthread_cond_init(tpool->job_is_empty, NULL) == -1){
		return -1;
	}
}
