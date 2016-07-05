#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>

int errno;

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
    struct sockaddr_in client;
    socklen_t   client_len;
    int     size;
    char    readbuf[1024];
    client_len = sizeof(client);
    
    while(1){
        connfd = accept(sockfd, (struct sockaddr *)&client, &client_len);
        if(connfd == -1){
            fprintf(stderr, "fail to accept\n");
            continue;
        }

        while(1){
            memset(readbuf, 0, 1024);
            //size = read(connfd, readbuf, 1024);
            size = recv(connfd, readbuf, 1024, MSG_WAITALL);
            if(size == -1){
                if(errno == EINTR)
                    fprintf(stderr, "接受客户端中断信号");
                else
                    fprintf(stderr, "fail to read: (%d:%s)\n", errno, strerror(errno));
                close(connfd);
                break;
            }

            if(size == 0){
                fprintf(stderr, "client disconnect\n");
                close(connfd);
                break;
            }
            fprintf(stderr, "read: %s\n", readbuf);

            write(connfd, "Hello You are online.", 30);
        }

    }
    
    return 0;
}

/* 使用多缓存区接受发数据
    函数 readv / writev 
         recvmsg / sendmsg */
int socket_read(int sockfd)
{
    ;
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

    socket_accept(sockfd);

    return 0;
}

