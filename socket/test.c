#include "epoll.h"

void *OnRecv(void *argv)
{
	printf("recv");
	return (void *)0;
}

void *OnSend(void *argv)
{
	printf("send");
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
	
	Tepoll_handle(servfd, OnRecv, NULL, OnSend, NULL);
	
	epoll_close(servfd);
#endif
	
	
	return 0;
}

