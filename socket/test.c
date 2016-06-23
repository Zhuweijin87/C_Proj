#include "epoll.h"

void write_html(char *ohtml);

int Get_Buffer_Line(char *buffer, char *line, int *offset)
{
	int pos = *offset;
	char *d = line;
	char *p = buffer + pos;
	//printf("p -- : %s\n", p);
	if(*p == '\r' || *(p+1) == '\n')
		return -1;
	while(*p != '\n')
	{
		*d++ = *p++;
		pos++;
	} 
	*offset = pos + 1;
	
	printf("line : [%d] %s\n", *offset, line);
	
	return 0;
}

void *OnRecv(void *argv)
{
	char	recvMsg[4096] = {'\0'};
	int sockfd = *(int *)argv;
	epoll_read(sockfd, recvMsg);

	printf("recv: %s\n", recvMsg);
#if 0
	int     offset = 0;
	char	line[256] = {'\0'};
	while( Get_Buffer_Line(recvMsg, line, &offset) == 0 )
	{
		memset(line, 0 , 256);		
	}
	printf("parse end\n");
#endif	
	char	outHtml[2048] = {'\0'};
	write_html(outHtml);
	
	epoll_write(sockfd, outHtml);

	return (void *)0;
}

void write_html(char *ohtml)
{
	strcat(ohtml, "<!doctype html>\r\n");
	strcat(ohtml, "<html>\r\n");
	strcat(ohtml, "<head>\r\n");
	strcat(ohtml, "<meta charset = \"utf-8\">\r\n");
	strcat(ohtml, "<tilte> Uploader </title>\r\n");
	strcat(ohtml, "</head>\r\n");
	strcat(ohtml, "<body>\r\n");
	strcat(ohtml, "<form method=\"post\" action=\"/upload\" enctype=\"multipart/form-data\">");
	strcat(ohtml, "Choose an image to upload: <input name=\"image\" type=\"text\" />");
	strcat(ohtml, "<input type=\"submit\" value=\"Upload\" />\r\n");
	strcat(ohtml, "</form>\r\n");
	strcat(ohtml, "</body>\r\n");
	strcat(ohtml, "</html>\r\n");
}

void *OnSend(void *argv)
{
	printf("send\n");
	
	return (void *)0;
}

int main()
{
	int servfd;
#if 1
	servfd = Tepoll_init("192.168.1.46:8003", 10);
	if(servfd == -1)
		return -1;
	printf("epoll create ok: %d\n", servfd);
	
	Tepoll_handle(servfd, &OnRecv, NULL, &OnSend, NULL);
	
	epoll_close(servfd);
#endif
	
	
	return 0;
}

