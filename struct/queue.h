#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _YES_ 0
#define _NO_  1

struct queue_node{
	char data[512];
	struct queue_node *next;
};
typedef struct queue_node QNode;

struct t_queue{
	int qsize;
	int is_full;
	int is_empty;
	struct queue_node *head;
	struct queue_node *tail;
};
typedef struct t_queue Queue;

#define tmalloc(size) malloc(size)

Queue *Queue_Create(int size);
int Queue_Push(Queue **q, char *data);
int Queue_Pop(Queue **q, char *data);

#endif
