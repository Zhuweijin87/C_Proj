#include "xlist.h"

list_node_t *list_node_create(int type, void *data)
{
	list_node_t		*node;
	node = zlmalloc(sizeof(list_node_t));
	node->prev = node->next = null;
	
	if(type == TYPE_INT)
		node->data.ldata = *(int *)data;
	else if(type == TYPE_DOUBLE)
		node->data.fdata = *(double *)data;
	else
		node->data.vdata = data;
	return node;
}

list_t *list_create(int type)
{
	list_t	*list;
	if(!(type == TYPE_INT || type == TYPE_DOUBLE || type == TYPE_VOID))
		return null;
	list = zlmalloc(sizeof(list_t));
	list->head = null;
	list->tail = null;
	list->type = type;
	
	return list;
}

iterator_t *list_iter(list_t *list)
{
	if(list == null)
		return null;
	
	iterator_t *iter = zlmalloc(sizeof(iterator_t));
	
	iter->next = list->head;
	return iter;
}

static int list_node_add(list_t *list, int pos, int tag, void *data)
{
	list_node_t		*pnode;
	if(list == null)
		return -1;

	pnode = list_node_create(list->type, data);
	if(pnode == null)
		return -1;

	if(pos > list->size)
		return -1;

	if(pos == 0){  /* add to list head */
		if(list->head == null){
			list->head = list->tail = pnode;
		}else{
			pnode->next = list->head;
			list->head->prev = pnode;
			list->head = pnode;
		}
	}else if(pos == -1){  /* add to list tail */
		if(list->tail == null){
			list->tail = list->head = null;
		}else{
			pnode->prev = list->tail;
			list->tail->next = pnode;
			list->tail = pnode;
		}
	}else{ /* add to the pos of list*/
		if(tag == 1){  /* add after */
			;
		}else if(tag == -1){  /* add before */
			;
		}
	}

	list->size++;

	return 0;
}

static int list_node_del(list_t *list, int pos, void *data)
{
	if(list == null)
		return -1;
	
	if(list->size < pos)
		return -1;
	
	return 0;
}

int list_add_head(list_t *list, void *data)
{
	return list_node_add(list, 0, 0, data);
}

int list_add_tail(list_t *list, void *data)
{
	return list_node_add(list, -1, 0, data);
}

