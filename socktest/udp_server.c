#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

int errno;

int socket_create(int port, char *ip)
{
    int sockfd;
    struct sockaddr_in server;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1){
        fprintf(stderr, "fail to socket:(%d:%s)\n", errno, strerror(errno));
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);
    //or server.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1){
        fprintf(stderr, "fail to bind:(%d:%s)\n", errno, strerror(errno));
        return -1;
    }

    return sockfd;    
}

int socket_recv(int sockfd)
{
    int     rsize;
    char    rbuf[512] = {'\0'};
    struct  sockaddr_in  from;
    int     flen = sizeof(from);

    while(1){
        memset(rbuf, 0, 512);
        rsize = recvfrom(sockfd, rbuf, 512, 0, (struct sockaddr *)&from, &flen);
        if(rsize == -1){
            fprintf(stderr, "fail to recvfrom:(%d:%s)\n", errno, strerror(errno));
            return -1;
        }
        fprintf(stderr, "recv: %s\n", rbuf);

        flen = sizeof(from);
        sendto(sockfd, "hello,OK", 10, 0, (struct sockaddr *)&from, flen);
    }

    return 0;
}

int main(int args, char *argv[])
{
    if(args != 3){
        fprintf(stderr, "参数错误\n");
        return -1;
    }

    int sockfd;
    sockfd = socket_create(atoi(argv[2]), (char *)argv[1]);
    if(sockfd == -1)
        return -1;

    socket_recv(sockfd);

    return 0;
}

