#include "list.h"
#include "map.h"

void show(struct link_list *list)
{
	if(null == list || null == list->head)
		return;
	struct link_node *pnode;
	pnode=list->head;
	
	while(pnode != null){
		fprintf(stderr, "%s\t", pnode->data);
		pnode = pnode->next;
	}
	printf("\n");
}

int setMapVal(struct Map *map)
{
	map_put(map, "accept", "Microsoft Nt 1.2.0");
	map_put(map, "link-cache", "256-bit, /*cat 9001");
	map_put(map, "host", "192.168.1.46");
	return 0;
}

int main()
{
#if 0
	struct link_list *list;
	char	data[6] = {'\0'};

	list = list_create(TYPE_STRING, 100);
	
	list_insert_at(&list, 0, "Zhu");
	list_insert_at(&list, 0, "Wei");
	list_insert_at(&list, 0, "Jin");
	list_insert_at(&list, 0, "ni");
	list_insert_at(&list, 0, "hao");
	list_insert_at(&list, 0, "wo");
	list_insert_at(&list, 0, "shi");
	list_insert_at(&list, 0, "jing");
	list_insert_at(&list, 0, "cai");
#endif

#if 0
	for(int i = 0; i<100; i++){
		memset(data, 0 ,6);
		sprintf(data, "%05d", i);
		list_insert_at(&list, 0, data);
	}
	show(list);

	printf("delete element from list:\n");
	list_delete_at(&list, -1);
	list_delete_at(&list, 0);
	list_delete_at(&list, -1);
	list_delete_at(&list, 35);
	show(list);

	printf("list reverse:\n");
	list_reverse(&list);
	show(list);
#endif

#if 0
	list_delete_at(&list, -1);
	show(list);
	list_delete_at(&list, -1);
	show(list);
	list_delete_at(&list, -1);
	show(list);
	list_delete_at(&list, 3);
	show(list);
#endif

#if 1
	struct Map *map = Map_Create(KEY_STRING, VAL_STRING, 256);
	printf("create OK\n");
	map_put(map, "method", "GET / HTTP1.0");
	map_put(map, "connect", "localhost:9090");
	setMapVal(map);

	char *s = NULL;
	map_get(map, "connect", (void **)&s);
	printf("ip:%s\n", s);
	
	map_get(map, "accept", (void **)&s);
	printf("ip:%s\n", s);
	
	map_get(map, "host", (void **)&s);
	printf("host: %s\n", s);
#endif
	Map_Close(&map);
	map_get(map, "link-cache", (void **)&s);
	printf("link-cache: %s\n", s);
	return 0;
}
