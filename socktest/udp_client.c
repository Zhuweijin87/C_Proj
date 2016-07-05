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
    struct sockaddr_in to;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1){
        fprintf(stderr, "fail to socket:(%d:%s)\n", errno, strerror(errno));
        return -1;
    }

    to.sin_family = AF_INET;
    to.sin_port = htons(port);
    to.sin_addr.s_addr = inet_addr(ip);
    //or server.sin_addr.s_addr = htonl(INADDR_ANY);

    int     wsize, rsize;
    char    wbuf[512], rbuf[512];
    struct sockaddr_in  from;
    int    flen = sizeof(from);

    while(1){
        fprintf(stderr, "input >: ");
        memset(wbuf, 0, sizeof(wbuf));
        fgets(wbuf, 512, stdin);
        if(strncmp(wbuf, "quit", 4) == 0)
            break;

        wbuf[strlen(wbuf) - 1] = '\0';
        wsize = sendto(sockfd, wbuf, 512, 0, (struct sockaddr *)&to, sizeof(to));
        if(wsize == -1){
            fprintf(stderr, "fail to sendto:(%d:%s)\n", errno, strerror(errno));
        }
        
        memset(rbuf, 0, 512);
        rsize = recvfrom(sockfd, rbuf, 512, 0, (struct sockaddr *)&from, &flen);
        if(rsize <= 0){
            fprintf(stderr, "fail to recvfrom server:(%d:%s)\n", errno, strerror(errno));  
            continue;
        }
        fprintf(stderr, "reply <: %s\n", rbuf);
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

    return 0;
}

