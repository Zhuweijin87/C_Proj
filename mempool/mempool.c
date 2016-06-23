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

	pool->used_size += size;
	pool->ptr_curr += size;
	
	return node;
}

static void mempool_node_list_add(mem_node_t *nodelist, mem_node_t *node)
{
	mem_node_t		*temp;
	if(nodelist == null || node == null)
		return ;
	
	if(nodelist == null){
		nodelist = node;
	}else{
		temp = nodelist;
		node->next = temp;
		temp = node;
	}

	return ;
}

static mem_node_t *mempool_alloc_list_get(mem_node_t *alloc_list, void *start)
{
	mem_node_t		*temp;

	temp = alloc_list;
	while(temp){
		if(temp->start == start){
			return temp;
		}
		temp = temp->next;
	}
	return null;
}

static mem_node_t *mempool_free_list_get(mem_node_t *free_list, int size)
{
	mem_node_t		*temp;
	temp = free_list;
	while(temp){
		if(temp->size <= size){
			return temp;
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
		pnode = mempool_free_list_get(pool->free_list, size);

	if(pnode == null)
		pnode = mem_node_create(pool, size);
	
	if(pnode == null)
		return null;
	
	mempool_node_list_add(pool->alloc_list, pnode);
	
	return (void *)pnode->start;
}

void mempool_clean(mempool_t *pool)
{
	memset(pool->ptr_init, 0, pool->size);
	pool->used_size = 0;
	pool->ptr_curr = pool->ptr_init;
	pool->alloc_list = pool->free_list = null;
}

void mempool_free(mempool_t *pool, void *node)
{
	mem_node_t 		*temp;
	temp = mempool_alloc_list_get(pool->alloc_list, node);
	if(temp == null)
		return;
	
	memset(temp->start, 0, temp->size);
	mempool_node_list_add(pool->free_list, node);
	return ;
}

void mempool_destroy(mempool_t **pool)
{
	free(*pool);
	*pool = null;
}
