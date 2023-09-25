#include "GCS.h"

t_serverInfo *serverInfo;
t_acceptSocket *acceptedSocket = NULL;

// handle SIGINT => shutdown(serverSocketFd, SHUT_RDWR) & free(serverAddress)

void cleanUpAndExit(int serverSocketFd, struct sockaddr_in *serverAddress) {
    shutdown(serverSocketFd, SHUT_RDWR);
    free(serverAddress);
    free(serverInfo);
    free(acceptedSocket->clientAddress);
    free(acceptedSocket);
    exit(1);
}

void handle_sigint(int sig) {
    printf("Server Has Been Terminated %d\n", sig);
    cleanUpAndExit(serverInfo->serverSocketFd, serverInfo->serverAddress);
}

int main() {
    serverInfo = malloc(sizeof(t_serverInfo));
    serverInfo->serverSocketFd = createIPV4Socket();
    if (serverInfo->serverSocketFd < 0) {
        perror("socket error");
        return (-1);
    }
    
    serverInfo->serverAddress = createIPV4Address(NULL, 8080);
    socklen_t serverAddressLen = sizeof(*serverInfo->serverAddress);
    //signal handling for SIGINT
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        perror("Signal handler installation failed");
        cleanUpAndExit(serverInfo->serverSocketFd, serverInfo->serverAddress);
    }
  
    //binding
    int bindRes = bind(serverInfo->serverSocketFd, (struct sockaddr *)serverInfo->serverAddress, serverAddressLen);
    if (bindRes < 0) {
        free(serverInfo->serverAddress);
        perror("bind error");
        return (-1);
    }
    //listening
    int listenRes = listen(serverInfo->serverSocketFd, 10);
    if (listenRes < 0) {
        perror("listen error");
        return (-1);
    }
    //accepting
    startAcceptingIncomingConnections(serverInfo->serverSocketFd);
    
    shutdown(serverInfo->serverSocketFd, SHUT_RDWR);
    free(serverInfo->serverAddress);
    free(serverInfo);
    return (0); 
}
