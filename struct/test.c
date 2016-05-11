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

	show(list);
#if 1	
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
