#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>

int createIPV4Socket() ;
struct sockaddr_in *createIPV4Address(void) ;
int connectSocket(int sockfd, struct sockaddr_in *addr) ;