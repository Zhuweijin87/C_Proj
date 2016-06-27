#include "map.h"

struct Map *Map_Create(int keyType, int valType, size_t size)
{
	struct Map *map = malloc(sizeof(struct Map));
	map->key_type = keyType;
	map->val_type = valType;
	map->data_size = size;
	map->head = null;
	return map;
}

void Map_Close(struct Map **map)
{
	if(null == *map || null == (*map)->head)
		return ;

	while((*map)->head){
		free((*map)->head);
		(*map)->head = (*map)->head->next;
	}
	free((*map));
	*map = null;
	return ;
}

struct map_node *map_node_create(int keyType, void *key, void *val, size_t valSize)
{
	struct map_node *mapnode = malloc(sizeof(struct map_node));
	if(keyType == KEY_INT)
		mapnode->key.int_key = *(int *)key;
	if(keyType == KEY_STRING)
		strncpy(mapnode->key.str_key, (char *)key, 64);
	/*
	mapnode->value = malloc(valSize);
	memcpy(mapnode->value, val, valSize);
	*/
	mapnode->value = val;
	mapnode->next = null;
	return mapnode;
}

int map_put(struct Map *map, void *key, void *val)
{
	struct map_node *pnode = map_node_create(map->key_type, key, val, map->data_size);
	if(null == map->head){
		map->head = pnode;
	}else{
		pnode->next = map->head;
		map->head = pnode;
	}
	return 0;
}

int map_get(struct Map *map, void *key, void **val)
{
	if(null == map || null == map->head){
		*val = null;
		return -1;
	}
	
	struct map_node *p = map->head;
	while(p){
		if( (map->key_type == KEY_INT && p->key.int_key == *(int *)key) ||
			(map->key_type == KEY_STRING && strcmp(p->key.str_key, (char *)key) == 0)){
			break;
		}
		p = p->next;
	}
	if(null == p){
		return -1;
	}

	/* Note Memory Leak if size of val is out of range*/
	//memcpy(val, p->value, 20);
	*val = p->value;
	return 0;
}

