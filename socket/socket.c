#include "socket.h"

/* error message */
static char errmsg[256];

/*set net option*/
static int socket_setopt(int sockfd)
{
	int		ret;

	/*reuse socket fd when closing the sock_fd in establish status*/
	int opt = 1;
	ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if(ret < 0){
		fprintf(stderr, "fail to set option to SO_REUSEADDR.(%d:%s)\n", errno, strerror(errno));
		return ret;
	}
	
	return 0;
	/*set send timeout when network exceptions occur*/
	int sndTimeout = 1000;  /* milseconds */
	ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &sndTimeout, sizeof(sndTimeout));
	if(ret < 0){
		fprintf(stderr, "fail to set option to SO_SNDTIMEO.(%d:%s)\n", errno, strerror(errno));
		return ret;
	}

	/*set recv timeout where network exceptions occur*/
	int rcvTimeout = 1000;
	ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &rcvTimeout, sizeof(rcvTimeout));
	if(ret < 0){
		fprintf(stderr, "fail to set option to SO_RCVTIMEO.(%d:%s)\n", errno, strerror(errno));
		return ret;
	}
	
	/*set send buffer size, system default about 8688 bytes, if buffer size is set to 0,
 *  	 buffer will not copy to system cache, it should be set before listen() or connect()*/
	int sndBufSize = 10 * 1024; /* 10KB */
	ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sndBufSize, sizeof(sndBufSize));
	if(ret < -1){
		fprintf(stderr, "fail to set option to SO_SNDBUF.\n");
		return ret;
	}

	/*set recv buffer size, as like above*/
	int rcvBufSize = 10 * 1024;
	ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvBufSize, sizeof(rcvBufSize));
	if(ret < 0){
		fprintf(stderr, "fail to set option to SO_RCVBUF.\n");
		return ret;
	}

	return 0;
}

/*set non block mode*/
int setnonblock(int sockfd)
{
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1) {
        return -1;
    }
    return 0;
}

/* for Server: init socket struct, create socket server
 *  * return : server file descriptor*/
int socket_create(char *addr)
{
	int		ret = 0;
	int		servfd;	
	char    ipaddr[20] = {'\0'};
	int		port;
	struct sockaddr_in	server;;
	ret = sscanf(addr, "%[^:]:%d", ipaddr, &port);
	if(ret != 2){
		fprintf(stderr, "fail to split addr(%s)\n", addr);
		ret = -1;
		goto _END_;
	}
	memset(&server, 0x00, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if(strcmp(ipaddr, "DEFAULT") == 0)
		server.sin_addr.s_addr = htons(INADDR_ANY);
	else
		server.sin_addr.s_addr = inet_addr(ipaddr);

	servfd = socket(AF_INET, SOCK_STREAM, 0);
	if(servfd < 0){
		fprintf(stderr, "fail to socket, Err:%s\n", strerror(errno));
		return -1;
	}

	/*set servfd non block*/
	setnonblock(servfd);

	socket_setopt(servfd);

	ret = bind(servfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
	if(ret < 0){
		fprintf(stderr, "fail to bind [ipaddr:%s, port:%d], Err:%s\n", ipaddr, port, strerror(errno));
		ret = -1;
		goto _END_;
	}

	/*set socket option when nessary*/
	/*default non set*/

	ret = listen(servfd, MAX_LISTENER);
	if(ret < 0){
		fprintf(stderr, "fail to listen, Err:%s\n", strerror(errno));
		ret = -1;
		goto _END_;
	}
	
	ret = servfd;
_END_:
	if(ret == -1)
		close(servfd);
	return ret;
}

#if 0
/*init sockfd*/
int socket_create(char *ipaddr)
{
	int	servfd;
	struct sockaddr_in server;

	servfd = socket(AF_INET, SOCK_STREAM, 0);
	if(servfd == -1){
		fprintf(stderr, "fail to socket:%s\n", strerror(errno));
		return -1;
	}

	char	addr_ip[20] = {'\0'};
	int		addr_port;
	if(sscanf(ipaddr, "%[^:]:%d", addr_ip, &addr_port) != 2){
		fprintf(stderr, "fail to split address & port\n");
		return -1;
	}

	printf("ip: %s, port:%d\n", addr_ip, addr_port);
	server.sin_family = AF_INET;
	server.sin_port = htonl(addr_port);
	server.sin_addr.s_addr = inet_addr(addr_ip);
	if(bind(servfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1){
		fprintf(stderr, "fail to bind:%s\n", strerror(errno));
		return -1;
	}
	
	if(listen(servfd, MAX_LISTENER) == -1){
		fprintf(stderr, "fail to listen:%s\n", strerror(errno));
		return -1;
	}
	return servfd;
}
#endif

/*for client*/
int socket_connect(char *ipaddr)
{
	int connfd;
	struct sockaddr_in client;
	
	connfd = socket(AF_INET, SOCK_STREAM, 0);
	if(connfd == -1){
		fprintf(stderr, "fail to socket:%s\n", strerror(errno));
		//set_error("fail to socket:%s", strerror(errno));
		return -1;
	}

	char	addr_ip[20] = {'\0'};
	int		addr_port;
	if(sscanf(ipaddr, "%[^:]:%d", addr_ip, &addr_port) != 2){
		fprintf(stderr, "split (%s) address & port error\n", ipaddr);
		return -1;
	}

	client.sin_family = AF_INET;
	client.sin_port = htonl(addr_port);
	client.sin_addr.s_addr = inet_addr(addr_ip);

	if(connect(connfd, (struct sockaddr *)&client, sizeof(struct sockaddr)) == -1){
		fprintf(stderr, "fail to connect:%s\n", strerror(errno));
		return -1;
	}

	return 0;
}

int socket_accept(int servfd)
{
	int	clientfd, client_t;
	struct sockaddr_in client;
	
	client_t = sizeof(client);
	memset(&client, 0, sizeof(client));
	clientfd = accept(servfd, (struct sockaddr *)&client, (socklen_t *)&client_t);
	if(clientfd == -1){
		fprintf(stderr, "fail to accept:%s\n", strerror(errno));
		return -1;
	}
	
	#if 1
	fprintf(stderr, "new client request [%s : %d]\n", inet_ntoa(client.sin_addr), client.sin_port);
	#endif
	return clientfd;
}

static int _socket_recv(int sockfd, char *msg, int timeout)
{
	int blen;
	blen = recv(sockfd, msg, MAX_RECV_SIZE, 0);
	if(blen < 0){
		if(errno != EAGAIN && errno != EINTR){
			fprintf(stderr, "fail to recv:%s\n", strerror(errno));
			return -1;
		}else{
			return 0;
		}
	}else if(blen == 0){
		return -1;
	}
	return blen;
}


static int _socket_send(int sockfd, char *msg)
{
	int	ret;
	ret = send(sockfd, msg, strlen(msg)+1, 0);
	if(ret == -1){
		fprintf(stderr, "fail to send:%s\n", strerror(errno));
		return -1;
	}
	return 0;
}

int socket_send(int sockfd, char *sndBuf)
{
	return _socket_send(sockfd, sndBuf);
}

int socket_recv(int sockfd, char *rcvBuf)
{
	return _socket_recv(sockfd, rcvBuf, 0);
}

int socket_sync_req(int sockfd, char *sndBuf, char *rcvBuf, int timeout)
{
	return 0;	
}

/* error information show */
void set_error(char *err)
{
	memset(errmsg, 0, sizeof(errmsg));
	strncpy(errmsg, err, 256);
}

char *get_error()
{
	return errmsg;
}

