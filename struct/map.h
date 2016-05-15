#ifndef _MAP_H
#define _MAP_H

struct map_node{
	union
	{
		int int_key;
		char str_key[65];
	}key;
	void *value;
};

struct map{
	int keyType;
	int valueType;
	int dataSize;
	struct link_list *maplist;
};

#endif
