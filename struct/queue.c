#include "queue.h"

static int queue_max_size;

Queue *Queue_Create(int size)
{
	Queue *q = tmalloc(sizeof(Queue));
	if(NULL == q) return NULL;
	
	q->qsize = 0;
	q->is_full = _NO_;
	q->is_empty = _YES_;
	q->head = q->tail = NULL;
	queue_max_size = size;
	return q;
}

static QNode *queue_node_create(char *data)
{
	QNode *qnode = tmalloc(sizeof(QNode));
	if(NULL == qnode) return NULL;

	strcpy(qnode->data, data);
	qnode->next = NULL;
	return qnode;
}

int Queue_Push(Queue **q, char *buffer)
{
	Queue *pq = *q;
	if(NULL == pq) return -1;
	
	QNode *qnode = queue_node_create(buffer);

	if(NULL == qnode) return -1;

	if(NULL == pq->head){
		pq->head = pq->tail = qnode;
		pq->is_empty = _NO_;
		return 0;
	}
	
	if(queue_max_size == pq->qsize + 1) return -1;

	pq->tail->next = qnode;
	pq->tail = qnode;
	pq->qsize++;
	return 0;
}

int Queue_Pop(Queue **q, char *buffer)
{
	Queue *pq = *q;
	QNode *qnode = NULL;
	
	if(NULL == pq)	return -1;
	
	if(NULL == pq->tail) return -1;

	qnode = pq->head;
	pq->head = pq->head->next;
	
	strncpy(buffer, qnode->data, strlen(qnode->data));
	free(qnode);
	qnode = NULL;
	return 0;
}


