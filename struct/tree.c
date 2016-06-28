#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct data_s{
	int  did;
	void *data;
}data_t;

typedef struct tree_node_s{
	data_t	data;
	struct tree_node_s *parent;
	struct tree_node_s *lchild;
	struct tree_node_s *rchild;
}tree_t;

tree_t *tree_create()
{
	tree_t *tree;
	tree = malloc(sizeof(tree));
	tree->parent = null;
	tree->lchild = tree->rchild = NULL; 
	return tree;
}

int tree_add(tree_t *tree, )
{

}

int main()
{
 			
	return 0;
}

