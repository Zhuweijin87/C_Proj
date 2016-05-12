#ifndef _HASH_H
#define _HASH_H

#include "list.h"

struct hash_tbl{
	int hash_size;
	struct link_list *list;
};

#endif
