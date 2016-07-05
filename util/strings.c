#include <stdio.h>
#include <string.h>

#define null NULL

char *cvt_to_upper(char *str)
{
	char	*p = str;
	while(*p != '\0'){
		if('a' <= *p && *p <= 'z'){
			*p = *p - 32;
		}
		*p++;
	}
	return str;
}

char *cvt_to_lower(char *str)
{
	char *p = str;
	while(*p != '\0'){
		if('A' <= *p && *p <= 'Z')
			*p = *p + 32;
		*p++;
	}
	return str;
}

int string_pattern(const char *orgstr, char *pattern)
{
	if(null == orgstr || null == pattern)
		return -1;
	
	return 0;
}

