#include "map.h"

struct map *map_create(int keyType, int valueType)
{
	struct map *pm = malloc(sizeof(struct map));
	if(null == pm) return null;

	pm->keyType = keyType;
	pm->valueType = valueType;
	pm->maplist = list_create(TYPE_STRUCT, sizeof(struct map_node));
	if(null == pm->maplist) return null;
	return pm;
}

int map_put(struct map *pm, void *key, void *value)
{
	struct map_node *p = malloc(sizeof(struct map_node));
	if(pm->keyType == KEY_INT)
		p->key = *(int *)key;
	if(pm->keyType == KEY_STRING)
		strncpy((char *)p->key, (char *)key, 64);
	
	p->value = value;
	
	return link_insert_at(&(pm->maplist), 0, p);
}

void *map_get(struct map *pm, void *key)
{
	struct map_node *p;
	
	return 0;
}
