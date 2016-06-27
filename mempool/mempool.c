#include "mempool.h"

int mempool_create_ex(mempool_t **pool, int size, int grow_size)
{	
	mempool_t		*temp;
	if(sizeof(mempool_t) >= size)
		return -1;
	
	temp = malloc(size);
	temp->init_size = size - MEM_POOL_SIZE;
	temp->used_size = 0;
	temp->grow_size = grow_size;
	temp->free_list = null;
	temp->alloc_list = null;
	temp->ptr_init = (void *)temp + MEM_POOL_SIZE;
	temp->ptr_curr = (void *)temp + MEM_POOL_SIZE;

	*pool = temp;

	return 0;	
}

int mempool_create(mempool_t **pool, int size)
{
	return mempool_create_ex(pool, size, 0);
}

static mem_node_t *mem_node_create(mempool_t *pool, int size)
{
	mem_node_t		*node;
	if(size > pool->init_size - pool->used_size)
		return null;

	node = (mem_node_t *)pool->ptr_curr;
	
	node->start = (void *)node + MEM_NODE_SIZE;
	node->size = size;
	node->next = null;
	node->prev = null;

	pool->used_size += size;
	pool->ptr_curr += size;
	
	return node;
}

static int mempool_alloc_list_put(mempool_t *pool, mem_node_t *node)
{
	if(pool == null || node == null)
		return -1;
	
	if(pool->alloc_list == null)
		pool->alloc_list = node;
	else
	{
		pool->alloc_list->prev = node;
		node->next = pool->alloc_list;
		pool->alloc_list = node;
	}
	return 0;
}

static mem_node_t *mempool_alloc_list_get(mempool_t *pool, void *start)
{
	mem_node_t		*temp;

	temp = pool->alloc_list;
	while(temp)
	{
		if(temp->start == start){
			if(temp->prev == null)
				pool->alloc_list = temp->next;
			else{
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
			}
			return temp;
		}
		temp = temp->next;
	}
	
	return null;
}

static int mempool_free_list_put(mempool_t *pool, mem_node_t *node)
{
	if(pool == null || node == null)
		return -1;
	
	if(pool->free_list == null)
		pool->free_list = node;
	else
	{
		pool->free_list->prev = node;
		node->next = pool->free_list;
		pool->free_list = node;
	}
	
	return 0;
}

static mem_node_t *mempool_free_list_get(mempool_t *pool, int size)
{
	mem_node_t		*temp;
	
	temp = pool->free_list;
	while(temp){
		if(temp->size >= size){
			if(temp->prev == null)
				pool->free_list = pool->free_list->next;
			else
				temp->prev->next = temp->next;
		}
		temp = temp->next;
	}
	return null;
}

void *mempool_alloc(mempool_t *pool, int size)
{
	mem_node_t	*pnode = null;
	if(pool == null)
		return null;

	if(pool->free_list != null)
		pnode = mempool_free_list_get(pool, size);

	if(pnode == null)
		pnode = mem_node_create(pool, size);
	
	if(pnode == null)
		return null;
	
	mempool_alloc_list_put(pool, pnode);

	return (void *)pnode->start;
}

void mempool_free(mempool_t *pool, void **node)
{
	mem_node_t 		*temp;

	temp = mempool_alloc_list_get(pool, *node);
	if(temp == null){
		return;
	}
	
	memset(temp->start, 0, temp->size);

	temp->prev = temp->next = null;

	mempool_free_list_put(pool, temp);

	*node = null;

	return ;
}

void showmempool(mempool_t *pool)
{
	mem_node_t *node;
	node = pool->alloc_list;
	printf("alloc info:\n");
	while(node){
		printf("\tnode info: %d : %d\n", node, node->start);
		node = node->next;
	}
	
	printf("free info:\n");
	node = pool->free_list;
	while(node){
		printf("\tnode info: %d : %d\n", node, node->start);
		node = node->next;
	}

	return ;
}

void mempool_clean(mempool_t *pool)
{
	memset(pool->ptr_init, 0, pool->init_size);
	pool->used_size = 0;
	pool->ptr_curr = pool->ptr_init;
	pool->alloc_list = pool->free_list = null;
}

void mempool_destroy(mempool_t **pool)
{
	free(*pool);
	*pool = null;
}
