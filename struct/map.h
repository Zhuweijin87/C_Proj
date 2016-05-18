#ifndef _MAP_H
#define _MAP_H

#define KEY_INT	   0x01
#define KEY_STRING 0x02

#define VAL_INT   0x10
#define VAL_STRING 0x11
#define VAL_STRUCT 0x12

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct map_node{
	union
	{
		int int_key;
		char str_key[65];
	}key;
	void **value;
	struct map_node *next;
};

struct Map{
	int key_type;
	int val_type;
	int data_size;
	struct map_node *head;
};

#define KEY_INT		0x01
#define KEY_STRING 	0x02
#define VAL_INT		0x10
#define VAL_STRING  0x11
#define VAL_STRUCT 	0x12

struct Map *Map_Create(int keyType, int valType, size_t size);
int map_get(struct Map *mapp, void *key, void **val);

#define null NULL
#endif
