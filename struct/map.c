#include "map.h"

struct map *map_create(int keyType, int valueType, int dataSize)
{
	struct map *pm = malloc(sizeof(struct map));
	if(null == pm) return null;

	pm->keyType = keyType;
	pm->valueType = valueType;
	pm->dataSize = size;
	pm->maplist = list_create(TYPE_STRUCT, sizeof(struct map_node));
	if(null == pm->maplist) return null;
	return pm;
}

static struct map_node map_node_create(struct map *map, void *key, void *value)
{
	struct map_node *p = malloc(sizeof(struct map_node));
	if(map->keyType == KEY_INT)
		p->key.int_key = *(int *)key;
	if(map->keyType == KEY_STRING)
		strncpy(p->key.str_key, (char *)key, 64);
	memcpy(p->value, value, map->dataSize);
	return p;
}

int map_put(struct map *map, void *key, void *value)
{
	struct map_node *p = map_node_create(map, key, value);
	if(null == p) return -1;
	return link_insert_at(&(pm->maplist), 0, p);
}

void *map_get(struct map *pm, void *key)
{
	struct map_node *p;
	
	return 0;
}
