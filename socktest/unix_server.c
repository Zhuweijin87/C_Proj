#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>   /* unix */
#include <stdlib.h>
#include <errno.h>

int socket_create()
{
    int     sockfd;
    struct sockaddr_un server;
    
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd == -1){
        fprintf(stderr, "fail to socket:(%d:%s)\n", errno, strerror(errno));
        return -1;
    }

    unlink();

}
