#include "threadpool.h"

int threadpool_create(struct threadpool **pool, int thread_size, int max_jobs)
{
	struct threadpool *tpool = malloc(sizeof(struct threadpool));
	if(NULL == tpool) return -1;

	tpool->thread_size = thread_size;
	tpool->job_max_size = max_jobs;
	
	if(pthread_mutex_init(tpool->lock, NULL) == -1){
		return -1;
	}

	if(pthread_cond_init(tpool->jobq_is_full, NULL) == -1){
		return -1;
	}

	if(pthread_conf_init(tpool->jobq_is_not_full, NULL) == -1){
		return -1;
	}

	if(pthread_cond_init(tpool->jobq_is_not_empty, NULL) == -1){
		return -1;
	}

	tpool->pthread_vector = malloc(sizeof(pthread_t) * thread_size);
	for(int i=0; i<thread_size; i++){
		if(pthread_create(&tpool->pthread_vector[i], NULL, callFunction, NULL) == -1){
			return -1;
		}
	}
	
	tpool->job_count = 0;
	tpool->pJobHead = tpool->pJobTail = NULL;
	return 0;
}

void *callFunction(void *argv)
{
	struct threadpool *tpool = (struct threadpool *)argv;
	struct job	*pJob = NULL;
	
	while(1){
		pthread_mutex_lock(&tpool->lock);
		while(tpool->job_count == 0){
			pthread_cond_wait(&tpool->jobq_is_not_empty, &tpool->lock);
		}
		
		pJob = tpool->pJobHead;
		tpool->pJobHead = pJobHead->next;
		tpool->job_count--;
		
		pthread_mutex_unlock(&lock);
		pJob->pfunc(pJob->argv);
		free(pJob);
		pJob = NULL;
	}
	
	return (void *)0;
}

int threadpool_jobadd(struct threadpool *tpool, void *(*handleFunc)(void *argv), void *argv)
{
	struct job *pJob;

	pthread_mutex_lock(&tpool->lock);
	while(tpool->job_count == tpool->job_max_size){
		pthread_cond_wait(&(tpool->jobq_is_not_full), &(tpool->lock));
	}

	pJob->argv = argv;
	pJob->pfunc = handleFunc;
	pJob->next = NULL;
	
	if(tpool->job_count == 0){
		tpool->pJobHead = tpool->pJobTail = pJob;
		pthread_cond_broadcast(&(tpool->jobq_is_not_empty), &(tpool->lock));
	}else{
		tpool->pJobTail->next = pJob;
		tpool->pJobTail = pJob;
	}
	pJob->job_count++;

	pthread_mutex_unlock(&tpool->lock);
}

void threadpool_destroy(struct threadpool *pool)
{

}
