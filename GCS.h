#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct s_acceptSocket {
    int clientSocketFd;
    struct sockaddr_in *clientAddress;
    int error;
    bool fullyAccepted;
} t_acceptSocket;


int createIPV4Socket() ;
struct sockaddr_in *createIPV4Address(char *ip, int port) ;
int connectSocket(int sockfd, struct sockaddr_in *addr) ;

t_acceptSocket *acceptSocket(int serverSocketFd) ;
void receiveIncommingRequestAndRespond (int clientSocketFd) ;