#include "socket.h"

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

	if(bind(servfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1){
		fprintf(stderr, "fail to bind:%s\n");
		return -1;
	}
	
	if(listen(servfd, MAX_LISTENER) == -1){
		fprintf(stderr, "fail to listen:%s\n", strerror(errno));
		return -1;
	}
	return servfd;
}

/*for client*/
int socket_connect(char *ipaddr)
{
	int connfd;
	struct sockaddr_in client;
	
	connfd = socket(AF_INET, SOCK_STREAM, 0);
	if(connfd == -1){
		fprintf(stderr, "fail to socket:%s\n", strerror(errno));
		return -1;
	}

	char	addr_ip[20] = {'\0'};
	int		addr_port;
	if(sscanf(ipaddr, "%[^:]:%d", addr_ip, addr_port) != 2){
		fprintf(stderr, "split (%s) address & port error\n", ipaddr);
		return -1;
	}

	client.sin_family = AF_INET;
	client.sin_port = htonl(addr_port);
	client.sin_addr.s_addr = addr_inet(addr_ip);

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
	return _socket_send(sockfd, sndBuf, 0);
}

int socket_recv(int sockfd, char *rcvBuf)
{
	return _socket_recv(sockfd, rcvBuf);
}

int socket_sync_req(int sockfd, char *sndBuf, char *rcvBuf, int timeout)
{
	
}

