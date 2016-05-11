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
	list->head = NULL;
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
		struct link_node *ptail = link_get_tail(*list);
		ptail->next = pnode;
		/*
		(*list)->tail->next = pnode;
		(*list)->tail = pnode;
		*/
		return 0;
	}
	
	return 0;
}

struct link_node *list_find_prenode(struct link_list *list, int pos)
{
	if(null == list) return null;
	if(pos > list_get_size(list) || 1 == pos) return null;
	
	int count = 1;
	struct link_node *pnode = list->head;
	while(pnode){
		if(count == pos -1)
			break;
		pnode = pnode->next;
		count++;
	}
	return pnode;
}

struct link_node *list_get_tail(struct link_list *list)
{
	if(null == list || null == list->head) return null;
	struct link_node *pnode;
	pnode = list->head;
	while(pnode){
		if(null == pnode->next)
			break;
		pnode = pnode->next;
	}
	return pnode;
}

/*pos 0: delete from list head, -1 delete from list tail*/
int list_delete_at(struct link_list **list, unsigned int pos)
{
	struct link_node *pnode, *prevNode;
	struct link_list *plist = *list;
	if(null == plist || null == plist->head) return -1;
	
	if(0 == pos){
		pnode = plist->head;
		plist->head = plist->head->next;
		lfree(pnode);
		pnode = null;
		return 0;
	}
	else if(-1 == pos){
		prevNode = list_find_prenode(plist, list_get_size(plist));
		if(null == prevNode){
			free(plist->head);
			plist->head = null;
		}else{
			//printf("tail node:%s\n", prevNode->data);
			pnode = prevNode->next;
			prevNode->next = null;
			lfree(pnode);
			pnode = null;
		}
	}
	else{
		#if 1
		prevNode = list_find_prenode(plist, pos);
		if(null == prevNode){
			return -1;
		}
		pnode = prevNode->next;
		prevNode->next = prevNode->next->next;
		free(pnode);
		pnode = null;
		#endif
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

