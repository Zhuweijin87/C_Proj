#ifndef _LOG_H
#define _LOG_H

#define DEBUG	4
#define WARN	3
#define INFO	2
#define ERROR	1

void loginit(int level, char *path, char *outfile);

void vlog(int level, const char *file, int line, char *fmt, ...);

#define V_INFO(args)  __FILE__,__LINE__
#define log(level, ...) vlog(level, V_INFO(args), __VA_ARGS__)

#endif
