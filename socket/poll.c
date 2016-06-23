#include "poll.h"

/* static argument */
static int local_pollfd_size;
static int local_maxfd;
static struct pollfd *local_pfdset;

int Tpoll_init(char *sockaddr, int size)
{
	int servfd;
	servfd = socket_create(sockaddr);
	if(servfd == -1){
		return -1;
	}

	local_pollfd_size = size > 0 ? size : DEFAULT_FD_SIZE;
	
	local_pfdset = malloc(sizeof(struct pollfd) * local_pollfd_size);
	for(int i=0; i<local_pollfd_size; i++){
		local_pfdset[i].fd = -1;
	}
	
	return servfd;
}

int tpoll_add(int sockfd)
{
	for(int i=0; i<local_maxfd; i++){
		if(local_pfdset[i].fd == -1){
			local_pfdset[i].fd = sockfd;
			local_pfdset[i].events = POLLIN;
			break;
		}
	}
	return 0;
}

int tpoll_del(int sockfd)
{
	for(int i=0; i<local_maxfd; i++){
		if(local_pfdset[i].fd == sockfd){
			local_pfdset[i].fd == -1;
			close(sockfd);
			break;
		}
	}
	return 0;
}

static int tpoll_accept(int srvfd)
{
	int clientfd;
	clientfd = socket_accept(srvfd);
	if(clientfd == -1)
		return -1;
	return tpoll_add(clientfd);
}

int Tpoll_handle(int servfd, void *(*pread)(void *ags), void *(*pwrite)(void *ags))
{
	int	ret = 0, nfds_for_ready;

	local_pfdset[0].fd = servfd;
	local_pfdset[0].events = POLLIN;
	local_maxfd = servfd;
	
	while(1){
		nfds_for_ready = poll(local_pfdset, local_maxfd + 1, -1);
		if(nfds_for_ready == -1){
			fprintf(stderr, "fail to poll:%s", strerror(errno));
			ret = -1;
			break;
		}

		if(local_pfdset[0].revents & POLLIN){
			/*client request connect*/
			tpoll_accept(servfd);
		}else{
			
		}
	}
	return 0;
}

