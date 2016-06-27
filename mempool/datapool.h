#ifndef _DATAPOOL_H
#define _DATAPOOL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_BUFFER_SIZE	256
#define DATA_ELEMENT_SIZE	512

#define STREAM_MAX_SIZE 	256
#define DATA_MAX_SIZE 		512
#define DATA_BASE_KEY 		8000

#define TYPE_LONG			0x00
#define TYPE_DOUBLE		0x01
#define TYPE_STRING		0x02

struct data_element 
{
	int		type;
	int 	tag;
	int		len;
	union   {
		int		longVal;
		double	doubleVal;
		char	strVal[DATA_BUFFER_SIZE];
	};
};

struct data_pool
{
	struct data_element data[DATA_ELEMENT_SIZE];
};

typedef struct data_pool TDataPool;

/* DataPool API */
int DataPoolGetString(TDataPool *dataPool, int did, char *val, int *len);

int DataPoolSetString(TDataPool *dataPool, int did, char *val);

int DataPoolGetLong(TDataPool *dataPool, int did, long *val);

int DataPoolSetLong(TDataPool *dataPool, int did, long val);

int DataPoolGetDouble(TDataPool *dataPool, int did, double *val);

int DataPoolSetDouble(TDataPool *dataPool, int did, double val);
 
#endif
