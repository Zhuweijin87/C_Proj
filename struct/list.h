#ifndef _LIST_H
#define _LIST_H

#include <stdio.h>
#include <string.h>

typedef struct link_node{
	void *data;
	struct list_node *next;
}LinkNode;

typedef struct _list_{
	struct link_node *head;
	struct link_node *tail;
}List;
#endif
