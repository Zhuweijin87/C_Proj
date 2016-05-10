#include "list.h"

void show(struct link_list *list)
{
	if(null == list)
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
	list_insert_at(&list, -1, "Wei");
	list_insert_at(&list, 0, "Jin");

	show(list);	
	list_delete_at(&list, -1);
	show(list);
	list_delete_at(&list, 0);
	show(list);
	list_delete_at(&list, -1);
	show(list);
	return 0;
}
