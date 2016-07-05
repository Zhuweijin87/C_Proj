#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>

#define ITER_FROM_HEAD		1
#define ITER_FROM_TAIL		2

#define TYPE_INT		0x01
#define TYPE_DOUBLE		0x02
#define TYPE_VOID		0x03

typedef struct list_node_s{
	struct list_node_s *  	precv;
	struct list_node_s *	next;
	union	data {
		void 	*vdata;
		int		ldata;
		double	fdata;
	};
} list_node_t;

typedef struct list_s{
	list_node_t *			head;
	list_node_t *			tail;
	int						size;
	int						type;
	int 					lock;
	/* functions */
	int		(*addhead)(list_t *list, void *data);
	int 	(*addtail)(list_t *list, void *data);
} list_t;

typedef struct iterator_s{
	list_node_t *			org;
	list_node_t	*			next;
	int						dir;
} iterator_t;

typedef struct iterator_f{
	list_node_t	*(*iter_next)(iterator_t *iter);
	int			(*iter_rewind)(iterator_t *iter);
}


#define zlmalloc(size)		malloc(size)

#endif
