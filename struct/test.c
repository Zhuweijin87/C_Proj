#include "list.h"

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
int main()
{
	struct link_list *list;
	char	data[6] = {'\0'};

	list = list_create(TYPE_STRING, 100);
#if 0	
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
	return 0;
}
