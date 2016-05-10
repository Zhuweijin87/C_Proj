#include "socket.h"

static int gls_epollfd;  /*globe localize static*/
static int event_size;
static struct epoll_event *ee;

/*初始化epoll server*/
int epoll_server_create(char *addr, int size)
{
	int	servfd;
	
	servfd = socket_create(addr);
	if(servfd == -1){
		return -1;
	}

	gls_epollfd = epoll_create(size);
	if(-1 == epollfd){
		fprintf(stderr, "fail to epoll create:%s\n", strerror(errno));
		return -1;
	}

	event_size = size;
	ee = malloc(sizeof(struct epoll_event) * size);
	return servfd;
}

int epoll_socket_add(int sockfd)
{
	struct epoll_event ev;
	ev.events = EPOLLLET | EPOLLIN;
	ev.data.fd = sockfd;
	if(epoll_ctl(gls_epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1){
		fprintf(stderr, "fail to epoll operate(add):%s\n", strerror(errno));
		return -1;
	}
	return 0;
}

int epoll_socket_mod(int sockfd, int mode)
{
	struct epoll_event ev;
	ev.data.fd = sockfd;
	if(mode == FD_READABLE)
		ev.events = EPOLLET | EPOLLIN;
	else if(mode == FD_WRITABLE)
		ev.events = EPOLLET | EPOLLOUT;
	if(epoll_ctl(gls_epollfd, EPOLL_CTL_MOD, sockfd, &ev) == -1){
		fprintf(stderr, "fail to epoll operate(modify):%s\n", strerror(errno));
		return -1;
	}
	return 0;
}

int epoll_socket_del(int sockfd)
{
	
}

int epoll_server_accept(int servfd)
{
	int clientfd;
	clientfd = socket_accept(servfd);
	if(clientfd == -1){
		
	}
	
	epoll_socket_add(cleintfd);

	return 0;
}

int epoll_handle(int servfd, void *(*pread)(void *argv), void *argr, void *(*pwrite)(void *argv), void *argw)
{
	int nfds;

	nfds = epoll_wait(epollfd, ee, event_size, -1);
	if(-1 == nfds){
		fprintf(stderr, "fail to epoll wait:%s\n", strerror(errno));
		return -1;
	}
	
	for(int i=0; i<nfds; i++){
		if(servfd == ee[i].data.fd){

		}else if(ee[i].events & EPOLLIN){

		}else if(ee[i].events & EPOLLOUT){

		}
	}
	return 0;
}


