#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define _YES_  	1
#define _NO_	0

struct job{
	void *argv;
	void *(*pfunc)(void *argv);
};

struct t_thread_pool{
	int th_size;
	int pool_is_close;
	int job_size;
	struct job JobHead;
	struct job JobTail;
	thread_t *pth_vector;
	pthread_mutex_t lock;
	pthread_cond_t 	jobs_is_full;
	pthread_cond_t	jobs_is_empty;
};

#endif
