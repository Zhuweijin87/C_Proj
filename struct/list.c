#include "list.h"

static struct link_node *link_node_create(DataType dType, size_t size, void *data)
{
	struct link_node *node = lmalloc(sizeof(struct link_node));
	if(null == node) return null;
	node->next = null;
	
	if(TYPE_INT == dType || TYPE_DOUBLE == dType)
		node->data = data;
	if(TYPE_STRING == dType || TYPE_STRUCT == dType){
		node->data = lmalloc(size);
		memcpy(node->data, data, size);
	}
	return node;
}

struct link_list *list_create(DataType type, size_t size)
{
	struct link_list *list = lmalloc(sizeof(struct link_list));
	if(null == list) return null;
	list->data_type = type;
	list->data_size = size;
	list->head = list->tail = NULL;
	return (struct link_list *)list;
}

int list_get_size(struct link_list *list)
{
	int size = 0;
	struct link_node *pnode;

	if(null == list) 
		return size;

	pnode = list->head;
	while(pnode != null){
		size++;
		pnode = pnode->next;
	}
	return size;
}

/*0: represent list head, -1: represent list tail*/
int list_insert_at(struct link_list **list, int pos, void *data)
{
	int index = 1;
	struct link_node *pnode, *plist;

	if(null == *list)	return -1;

	pnode = link_node_create((*list)->data_type, (*list)->data_size, data);
	if(null == pnode)	return -1;

	if(null == (*list)->head){
		(*list)->head = (*list)->tail = pnode;
		return 0;
	}

	if(0 == pos){
		pnode->next = (*list)->head;
		(*list)->head = pnode;
		return 0;
	}
	
	if(-1 == pos){
		(*list)->tail->next = pnode;
		(*list)->tail = pnode;
		return 0;
	}

	if(pos > list_get_size(*list))	return -1;
	
	plist = (*list)->head;
	while(plist != null){
		if(index == pos){
			pnode->next = plist->next;
			plist->next = pnode;
			break;
		}
		plist = plist->next;
		index++;
	}
	
	return 0;
}

/*pos 0: delete from list head, -1 delete from list tail*/
int list_delete_at(struct link_list **list, int pos)
{
	struct link_node *pnode, *preNode;
	
	if(null == *list) return -1;
	if(pos > list_get_size(*list)) return -1;
	
	if(0 == pos){
		pnode = (*list)->head;
		(*list)->head = (*list)->head->next;
		free(pnode);
		pnode = null;
		return 0;
	}

	if(-1 == pos){
		preNode = (*list)->head;
		while(preNode->next != null){
			if(preNode->next->next == null)
				break;
			preNode = preNode->next;
		}

		pnode = (*list)->tail;
		preNode->next = null;
		(*list)->tail = preNode;
		free(pnode);
		pnode = null;
	}

	return 0;
}

int list_reverse(struct link_list **list)
{
	struct link_node *prev, *pnext;

	if(null == list || null == *list) return -1;
	
	pnext = (*list)->head->next;
	(*list)->head->next = null;
	
	while(pnext){
		prev = pnext;
		pnext = pnext->next;
		prev->next = (*list)->head;
		(*list)->head = prev;
	}

	return 0;
}
