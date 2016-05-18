#include "epoll.h"

struct t_epoll_events {
	int 	eSize;
	struct epoll_event *peSet;
};
static int local_epollfd;  /*localize static variable*/
static struct t_epoll_events local_pevent;

/* epoll server */
int Tepoll_init(char *addr, int size)
{
	int	servfd;
	
	servfd = socket_create(addr);
	if(servfd == -1){
		return -1;
	}

	local_epollfd = epoll_create(size);
	if(-1 == local_epollfd){
		fprintf(stderr, "fail to epoll create:%s\n", strerror(errno));
		return -1;
	}

	local_pevent.eSize = size > 0 ? size : 512;
	local_pevent.peSet = malloc(sizeof(struct epoll_event) * local_pevent.eSize);
	if(null == local_pevent.peSet)
		return -1;

	if(epoll_fd_add(servfd) == -1)
		return -1;
	return servfd;
}

void epoll_close(int servfd)
{
	close(servfd);
	close(local_epollfd);
	free(local_pevent.peSet);
}

/* add fd to epoll */
int epoll_fd_add(int sockfd)
{
	struct epoll_event ev;
	ev.events = EPOLLET | EPOLLIN;
	ev.data.fd = sockfd;
	if(epoll_ctl(local_epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1){
		fprintf(stderr, "fail to epoll operate(add):%s\n", strerror(errno));
		return -1;
	}
	return 0;
}

/* modify epoll fd status */
int epoll_fd_mod(int sockfd, int mode)
{
	struct epoll_event ev;
	ev.data.fd = sockfd;
	if(mode == FD_READABLE)
		ev.events = EPOLLET | EPOLLIN;
	else if(mode == FD_WRITABLE)
		ev.events = EPOLLET | EPOLLOUT;
	if(epoll_ctl(local_epollfd, EPOLL_CTL_MOD, sockfd, &ev) == -1){
		fprintf(stderr, "fail to epoll operate(modify):%s\n", strerror(errno));
		return -1;
	}
	return 0;
}

/* delete epoll fd*/
int epoll_fd_del(int sockfd)
{
	struct epoll_event ee;
	ee.events = EPOLLIN | EPOLLET;
	ee.data.fd = sockfd;
	if(epoll_ctl(local_epollfd, EPOLL_CTL_DEL, sockfd, &ee) == -1){
		fprintf(stderr, "fail to epoll operate(delete):%s\n", strerror(errno));
		return -1;
	}
	return 0;
}

int epoll_accept(int servfd)
{
	int clientfd;
	clientfd = socket_accept(servfd);
	if(clientfd == -1){
		return -1;
	}
	return epoll_fd_add(clientfd);
	//return 0;
}

int epoll_read(int sockfd, char *recvBuff)
{
	return socket_recv(sockfd, recvBuff);	
}

int epoll_write(int sockfd, char *sendBuff)
{
	return socket_send(sockfd, sendBuff);
}

int Tepoll_handle(int servfd, void *(*pread)(void *argv), void *argr, void *(*pwrite)(void *argv), void *argw)
{
	int nfds, sockfd;
	struct epoll_event *pe = local_pevent.peSet;

EPOLL_CYCLE_WATCH:
	nfds = epoll_wait(local_epollfd, pe, local_pevent.eSize, -1);
	if(-1 == nfds){
		fprintf(stderr, "fail to epoll wait:%s\n", strerror(errno));
		return -1;
	}
	
	for(int i=0; i<nfds; i++){
		if(servfd == pe[i].data.fd){
			/*handle new client epoll*/
			sockfd = pe[i].data.fd;
			epoll_accept(sockfd);
		}else if(pe[i].events & EPOLLIN){
			/*handle read from epoll fd*/
			fprintf(stderr, "read fd message\n");
			sockfd = pe[i].data.fd;
			(*pread)(&sockfd);
		}else if(pe[i].events & EPOLLOUT){
			/*handle write to epoll fd*/
			fprintf(stderr, "write fd message\n");
			(*pwrite)(argw);
		}
	}
	goto EPOLL_CYCLE_WATCH;

	return 0;
}


