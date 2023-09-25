#include "GCS.h"

int main() {

    int serverSocketFd = createIPV4Socket();
    if (serverSocketFd < 0) {
        perror("socket error");
        return (-1);
    }
    struct sockaddr_in *serverAddress = createIPV4Address(NULL, 8080);
    socklen_t serverAddressLen = sizeof(*serverAddress);
  
    int bindRes = bind(serverSocketFd, (struct sockaddr *)serverAddress, serverAddressLen);
    if (bindRes < 0) {
        perror("bind error");
        return (-1);
    }
    int listenRes = listen(serverSocketFd, 10);
    if (listenRes < 0) {
        perror("listen error");
        return (-1);
    }
    t_acceptSocket *acceptedSocket = acceptSocket(serverSocketFd);
    receiveIncommingRequestAndRespond (acceptedSocket->clientSocketFd);
    if(!acceptedSocket) {
        free(serverAddress);
        shutdown(serverSocketFd, SHUT_RDWR);
        perror("accept error");
        return (-1);
    }
    
    free(serverAddress);
    close(acceptedSocket->clientSocketFd);
    shutdown(serverSocketFd, SHUT_RDWR);
    return (0); 
}
