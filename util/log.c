#include "log.h"

#define null	NULL

#define MAX_LINE_SIZE		1024

struct loginfo_t{
	int		fd;
	FILE	*fp;
	int 	level;
	int		fstat;
	char	logfile[65];
	char	datetime[21];
	char	date[9];
	char	*homed;
};

static struct loginfo_t loginfo;
static char	  wstream[MAX_LINE_SIZE];

inline int OFFSET(const char *s)
{
	int i = 0;
	while(*s++ != '\0')
		i++;
	return i;
} 

void getdatetime()
{
	time_t	tms;
	struct tm *p;

	memset(loginfo.datetime, 0, 21);
	
	time(&tms);
	p = localtime(&tms);
	snprintf(loginfo.datetime, 20, "%04d-%02d-%02d %02d:%02d:%02d", p->tm_year+1900, p->tm_mon+1, p->tm_mday, 
			p->tm_hour, p->tm_min, p->tm_sec);

	snprintf(loginfo.date, 9, "%04d%02d%02d", p->tm_year+1900, p->tm_mon+1, p->tm_mday);

	return ;
}

void gethomepath()
{
	if(loginfo.homed == null)
		loginfo.homed = (char *)getenv("HOME");
}

void openlog()
{
	if(loginfo.logfile[0] == '\0'){
		if(logdircheck("/log") != 0){
			loginfo.fstat = -1;
			return ;
		}
		sprintf(loginfo.logfile, "%s/log/SYS-%s.log", (char *)getenv("HOME"), loginfo.date);
	}

	fprintf(stderr, "log_file: %s\n", loginfo.logfile);
#if 0
	fd = open(log_file, O_CREAY | O_APPEND | O_RDWR , 0644);
	if(fd < 0);
		return ;
#endif
	loginfo.fp = fopen(loginfo.logfile, "a+");
	if(loginfo.fp == null){
		loginfo.fstat = -1;
		return ;
	}
	loginfo.fstat = 1;
}

int logdircheck(char *path)
{
	int		i = 0;
	char	dir[65] = {'\0'};

	strcpy(dir, loginfo.homed);
	i = OFFSET(dir);
	while(*path != '\0'){
		if(*path == '/'){
			dir[i++] = *path++;
		}
		while(*path != '/' && *path != '\0')
			dir[i++] = *path++;
		
		if(access(dir, 0) != 0){
			if(mkdir(dir, 0755) != 0){
				fprintf(stderr, "Create log file path fail\n");
				return -1;
			}
		}
	}
	return 0;
}

void loginit(int level, char *path, char *outfile)
{
	int offset;
	if(level > DEBUG || level < ERROR)
		loginfo.level = ERROR;

	gethomepath();

	if(path == null){
		offset = sprintf(loginfo.logfile, "%s/log", loginfo.homed);
	}
	else{
		offset = sprintf(loginfo.logfile, "%s%s", loginfo.homed, path);	
	}
	
	if(logdircheck(path) != 0)
		return ;

	getdatetime();

	if(outfile == null){
		sprintf(loginfo.logfile + offset, "/SYS-%s.log", loginfo.date);
	}
	else{
		sprintf(loginfo.logfile + offset, "/%s-%s.log", outfile, loginfo.date);
	}

	loginfo.level = level;
	
	return ;		
}

void vlog(int level, const char *file, int line, char *fmt, ...)
{
	char	*pstr = null;
	if(level > DEBUG || level < ERROR)
		return ;

	if(level > loginfo.level && loginfo.level != 0)
		return ;

	getdatetime();
	
	if(loginfo.fp == null)
		openlog();

	if(loginfo.fstat == -1)
		return ;

	memset(wstream, 0 , MAX_LINE_SIZE);

	pstr = wstream;
	
	sprintf(pstr, "[%s][%d][%s:%d]", loginfo.datetime, getpid(), file, line);

	pstr = pstr + OFFSET(pstr);

	va_list argvs;
	va_start(argvs, fmt);
	vsnprintf(pstr, MAX_LINE_SIZE, fmt, argvs);
	va_end(argvs);

	printf("write buffer: %s\n", wstream);
	
	fprintf(loginfo.fp, "%s\n", wstream);

	return ;
}



