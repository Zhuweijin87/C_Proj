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

void sigerr_handler(int signo)
{
    fprintf(stderr, "server has been disconnected\n");
}

int socket_connect(int port, char *ip)
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

    if(connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1){
        fprintf(stderr, "fail to conenct: %s\n", strerror(errno));
        return -1;
    }

    return sockfd;
}

int socket_write(int sockfd)
{
    int     wsize, rsize;
    char    writebuf[1024];
    char    readbuf[512];
    while(1){
        fprintf(stderr, "Write >: ");
        memset(writebuf, 0, 1024);
        fgets(writebuf, 1024, stdin);
        if(strncmp(writebuf, "quit", 4) == 0){
            fprintf(stdout, "quit connect\n");
            close(sockfd);
            return 0;
        }
        writebuf[strlen(writebuf)-1] = '\0';
        wsize = write(sockfd, writebuf, 1024);
        /*等价于 : wsize = send(sockfd, writebuf, 1024, 0) */
        if(wsize == -1){
            fprintf(stderr, "fail to write stream: %s\n", strerror(errno));
        }
        
        if(wsize > 0){
            memset(readbuf, 0, 512);
            //rsize = read(sockfd, readbuf, 512);
            rsize = recv(sockfd, readbuf, 100, 0);
            if(rsize > 0)
                fprintf(stderr, "reply <: %s\n", readbuf);
        }
    }

    return -1;
}

int main(int args, char *argv[])
{
    int  sockfd;
    
    if(args != 3){
        fprintf(stderr, "参数错误\n");
        return -1;
    }
  
    /*如果服务端断开, 捕捉该信号*/ 
    signal(SIGPIPE, sigerr_handler);
 
    sockfd = socket_connect(atoi(argv[2]), (char *)argv[1]);
    if(sockfd == -1){
        return -1;
    }

    if(socket_write(sockfd) == -1){
        return -1;
    }

    return 0;
}

