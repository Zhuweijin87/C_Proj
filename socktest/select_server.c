#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>

int errno;

#define SEL_FD_SIZE  20

static int fdset[SEL_FD_SIZE];

void sigerrhandler(int signo)
{
    fprintf(stderr, "handler signal exception\n");
}

int socket_create(int port, char *ip)
{
    int     sockfd;
    struct sockaddr_in server;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        fprintf(stderr, "fail to create socket:%s\n", strerror(errno));
        return -1;
    }

    memset(&server, 0x00, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);

    if(bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1){
        fprintf(stderr, "fail to bind: %s\n", strerror(errno));
        return -1;
    }

    if(listen(sockfd, 10) == -1){
        fprintf(stderr, "fail to listen:%s\n", strerror(errno));
        return -1;
    }

    return sockfd;
}

int socket_accept(int sockfd)
{
    int     connfd;
    struct  sockaddr_in client;
    socklen_t  clilen;
    
    connfd = accept(sockfd, (struct sockaddr *)&client, &clilen);
    if(connfd == -1){
        fprintf(stderr, "fail to accept:(%d:%s)\n", errno, strerror(errno));
        return -1;
    }
    return connfd;
}

int socket_recv(int connfd)
{
    int  rsize, wsize;
    char rbuf[512] = {'\0'}, wbuf[512] ={'\0'};

    rsize = read(connfd, rbuf, 512);
    if(rsize == -1){
        fprintf(stderr, "fial to read:(%d:%s)\n", errno, strerror(errno));
        return -1;
    }
    
    if(rsize == 0){
        fprintf(stderr, "client disconnect\n");
        return -1;
    }

    fprintf(stderr, "recv: %s\n", rbuf);

    return 0;
}

int socket_select(int sockfd)
{
    int     i, maxfd, nfds, ret, connfd;
    fd_set  rfds;
    struct timeval tv;
    
    for(i=0; i<SEL_FD_SIZE; i++)
        fdset[i] = -1;

    maxfd = sockfd;
    while(1){
        FD_ZERO(&rfds);
        FD_SET(sockfd, &rfds);
        maxfd = sockfd;
        for(i=0; i<SEL_FD_SIZE; i++){
            if(fdset[i] > 0){
                FD_SET(fdset[i], &rfds);
                maxfd = maxfd > fdset[i] ? maxfd : fdset[i];
            }
        }
        
        /* select fd every 5 sec */
        tv.tv_sec = 5;
        tv.tv_usec = 0;
    
        nfds = select(maxfd + 1, &rfds, NULL, NULL, &tv);
        if(nfds == -1){
            fprintf(stderr, "fail to select: (%d:%s)\n", errno, strerror(errno));
            return -1;
        }
        
        if(nfds == 0){
            fprintf(stderr, "select timeout\n");
            continue;
        }

        if(FD_ISSET(sockfd, &rfds))
        {
            connfd = socket_accept(sockfd);
            if(connfd == -1)
                continue;

            for(i=0; i<SEL_FD_SIZE; i++){
                if(fdset[i] == -1)
                    fdset[i] = connfd;
            }
            FD_SET(connfd, &rfds);
        }
        else
        {
            for(i=0; i<SEL_FD_SIZE; i++){
                if(FD_ISSET(fdset[i], &rfds)){
                    ret = socket_recv(fdset[i]);
                    if(ret){
                        FD_CLR(fdset[i], &rfds);
                        fdset[i] = -1;
                    }
                }
            }
        }
        ;
    }

    return 0;
}

int main(int args, char *argv[])
{
    int  sockfd;
    
    if(args != 3){
        fprintf(stderr, "参数错误\n");
        return -1;
    }
   
    signal(SIGPIPE, sigerrhandler);

    sockfd = socket_create(atoi(argv[2]), (char *)argv[1]);
    if(sockfd == -1){
        return -1;
    }

    socket_select(sockfd);

    return 0;
}

