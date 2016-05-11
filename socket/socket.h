#ifndef _SOCKET_H
#define _SOCKET_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define MAX_LISTENER 20

#define MAX_RECV_SIZE 1024
#define null NULL

extern int errno;

#endif
