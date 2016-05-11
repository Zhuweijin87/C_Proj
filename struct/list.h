#ifndef _LIST_H
#define _LIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum node_data_type{
	TYPE_INT = 1,
	TYPE_DOUBLE,
	TYPE_STRING,
	TYPE_STRUCT,
}DataType;

struct link_node{
	void *data;
	struct link_node *next;
};

struct link_list{
	DataType		 data_type;
	size_t			 data_size;
	struct link_node *head;
	struct link_node *tail;
};

#define lmalloc(size) malloc(size)
#define lfree(p)	  free(p)

#define null NULL

struct link_list *list_create(DataType type, size_t size);
#endif
