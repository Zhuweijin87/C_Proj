#ifndef _MEMERY_POOL_H
#define _MEMERY_POOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mem_node_s{
	void	*start;
	int		size;
	struct  mem_node_s *next;
};

typedef struct mem_node_s mem_node_t;

struct mem_pool_s{
	int			init_size;
	int			used_size;
	int			grow_size;
	void		*ptr_init;
	void		*ptr_curr;
	mem_node_t	*free_list;
	mem_node_t	*alloc_list;
};

typedef struct mem_pool_s mempool_t;
 
#define null NULL

#define MEM_NODE_SIZE sizeof(mem_node_t)

#define MEM_POOL_SIZE sizeof(mempool_t)

int mempool_create(mempool_t **pool, int size);

void *mempool_alloc(mempool_t *pool, int size);

#endif

