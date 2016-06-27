#include "datapool.h"

static int datapool_get_element_val(TDataPool *dataPool, int dtype, int did, void *val, int *len)
{
	if(did > DATA_ELEMENT_SIZE)
		return -2;
	if(dtype == TYPE_LONG)
	{
		if(dataPool->data[did].type != TYPE_LONG)
			return -1;
		else
		{
			long  *a = (long *)val;
			*a = dataPool->data[did].longVal;
			val = (void *)a;
		}
	}
	else if(dtype == TYPE_DOUBLE)
	{
		if(dataPool->data[did].type != TYPE_DOUBLE)
			return -1;
		else
		{
			double	*b = (double *)val;
			*b = dataPool->data[did].doubleVal;
			val = (void *)b;
		}
	}
	else if(dtype == TYPE_STRING)
	{
		if(dataPool->data[did].type != TYPE_STRING)
			return -1;
		else
		{
			if(*len > dataPool->data[did].len)
				*len = dataPool->data[did].len;
			memcpy(val, (void *)dataPool->data[did].strVal, *len);
		}
	}
	else
	{
		;
	}
	return 0;
}

static int datapool_set_element_val(TDataPool *dataPool, int dtype, int did, int tag, void *val)
{
	if(did > DATA_ELEMENT_SIZE)
		return -2;
	if(dtype == TYPE_LONG)
	{
		dataPool->data[did].type = TYPE_LONG;
		dataPool->data[did].len = 1;
		dataPool->data[did].tag = 1;
		dataPool->data[did].longVal = *(long *)val;
		//printf("set long: %d - %d\n", did, dataPool->data[did].longVal);
	}
	else if(dtype == TYPE_DOUBLE)
	{
		dataPool->data[did].type = TYPE_DOUBLE;
		dataPool->data[did].len = 1;
		dataPool->data[did].tag = 1;
		dataPool->data[did].doubleVal = *(double *)val;	
	}
	else if(dtype == TYPE_STRING)
	{
		dataPool->data[did].type = TYPE_STRING;
		dataPool->data[did].len = strlen((char *)val);
		dataPool->data[did].tag = 1;
		strncpy(dataPool->data[did].strVal, (char *)val, dataPool->data[did].len);
	}
	else
	{
		;
	}
	return 0;
}

/* DataPool Fucntion API */
int DataPoolGetString(TDataPool *dataPool, int did, char *val, int *len)
{
	/* get value of type string from datapool */
	return datapool_get_element_val(dataPool, TYPE_STRING, did, val, len);
}

int DataPoolSetString(TDataPool *dataPool, int did, char *val)
{
	/* set value of type string to datapool */
	return datapool_set_element_val(dataPool, TYPE_STRING, did, 0, (void *)val);
}

int DataPoolGetLong(TDataPool *dataPool, int did, long *val)
{
	/* get value of type long from datapool */
	return datapool_get_element_val(dataPool, TYPE_LONG, did, val, NULL);
}

int DataPoolSetLong(TDataPool *dataPool, int did, long val)
{
	/* set value of type long to datapool */
	return datapool_set_element_val(dataPool, TYPE_LONG, did, 0, (void *)&val);
}

int DataPoolGetDouble(TDataPool *dataPool, int did, double *val)
{
	/* get value of type double from datapool */
	return datapool_get_element_val(dataPool, TYPE_DOUBLE, did, val, NULL);
}

int DataPoolSetDouble(TDataPool *dataPool, int did, double val)
{
	/* set value of type double to datapool */
	return datapool_set_element_val(dataPool, TYPE_DOUBLE, did, 0, (void *)&val);
}
