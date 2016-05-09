#include "poll.h"

/* static argument */
static int LOC_POLLFD_SIZE;
static int LOC_MAX_FD;
static struct pollfd *clientfds;

int t_poll_create(char *sockaddr, int size)
{
	int servfd;
	servfd = socket_create(sockaddr);
	if(servfd == -1){
		return -1;
	}

	LOC_POLLFD_SIZE = size > 0 ? size : DEFAULT_FD_SIZE;
	
	clientfds = malloc(sizeof(struct pollfd) * LOC_POLLFD_SIZE);
	for(int i=0; i<LOC_POLL_FD; i++){
		clientfds[i] = -1;
	}
	
	return servfd;
}

static int poll_fd_add(int sockfd)
{
	int i;
	for(i=0; i<LOC_POLLFD_SIZE; i++){
		if(clientfds[i] == -1){
			clientfds[i] = sockfd;
			break;
		}
	}
	return 0;
}

static int poll_fd_del(int sockfd)
{
	int i;
	for(i=0; i<LOC_POLLFD_SIZE; i++){
		if(clientfd[i] == sockfd){
			clientfd[i] = -1;
			break;
		}
	}
	return 0;
}

int t_poll_accept(int srvfd)
{
	int clientfd;
	clientfd = socket_accept(srvfd);
	if(clientfd == -1)
		return -1;
	return 0;
}

int t_poll_handle(int servfd, void *(*pread)(void *ag_r), void *(*pwrite)(void *ag_w))
{
	int	ret = 0, nfds_for_ready;

	clientfds[0].fd = servfd;
	clientfds[0].events = POLLIN;
	LOC_MAX_FD = servfd;
	
	while(1){
		nfds_for_ready = poll(clientfds, LOC_MAX_FD + 1, INFTIM);
		if(nfds_for_ready == -1){
			fprintf(stderr, "fail to poll:%s", strerror(errno));
			ret = -1;
			break;
		}

		if(clientfds[0].revents & POLLIN){
			
		}else{

		}
	}
	return 0;
}
