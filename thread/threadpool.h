#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define _YES_  	1
#define _NO_	0

struct job{
	void *argv;
	void *(*pfunc)(void *argv);
	struct job *next;
};

struct threadpool{
	int thread_size;
	int job_max_size;
	int job_count;
	struct job *pJobHead;
	struct job *pJobTail;
	pthread_t *pthread_vector;
	pthread_mutex_t lock;
	pthread_cond_t 	jobq_is_full;
	pthread_cond_t	jobq_is_not_full;
	pthread_cond_t	jobq_is_not_empty;
};

#endif
