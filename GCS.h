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
#include <signal.h>

typedef struct s_acceptSocket {
    int error;
    int clientSocketFd;
    bool fullyAccepted;
    struct sockaddr_in *clientAddress;
} t_acceptSocket;

extern t_acceptSocket *acceptedSocket;

typedef struct s_serverInfo {
    int serverSocketFd;
    struct sockaddr_in  *serverAddress;
} t_serverInfo;


int createIPV4Socket() ;
struct sockaddr_in *createIPV4Address(char *ip, int port) ;
int connectSocket(int sockfd, struct sockaddr_in *addr) ;

t_acceptSocket *acceptSocket(int serverSocketFd) ;
void receiveIncommingRequestAndRespond (int clientSocketFd) ;
void *startAcceptingIncomingConnections(int serverSocketFd) ;
void *receiveAndRespond(void *clientSocketFd) ;
