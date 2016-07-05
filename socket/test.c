#include "epoll.h"

void *OnRecv(void *arg)
{
	char msgbuf[1024] = {'\0'};
	int fd = *(int *)arg;
	
	int ret = epoll_read(fd, msgbuf);
	if(ret)
	{
		return (void *)-1;
	}
	printf("recv: %s\n", msgbuf);	

	epoll_fd_mod(fd, FD_WRITABLE);

	return (void *)0;
}

void *OnSend(void *arg)
{
	int fd = *(int *)arg;
	epoll_write(fd, "Hello Great Wall");
	
	epoll_fd_mod(fd, FD_READABLE);
	
	return (void *)0;
}

int main()
{
	int servfd;

	servfd = Tepoll_init("192.168.1.46:8003", 10);
	if(servfd == -1)
		return -1;
	printf("epoll create ok: %d\n", servfd);
	
	Tepoll_handle(servfd, &OnRecv, NULL, &OnSend, NULL);
	
	epoll_close(servfd);
	
	return 0;
}

