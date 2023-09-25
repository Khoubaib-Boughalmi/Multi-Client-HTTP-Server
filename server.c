#include "GCS.h"

int main() {

    int serverSocketFd = createIPV4Socket();
    if (serverSocketFd < 0) {
        perror("socket error");
        return (-1);
    }
    struct sockaddr_in *serverAddress = createIPV4Address(NULL, 8080);
    socklen_t serverAddressLen = sizeof(*serverAddress);
  
    //binding
    int bindRes = bind(serverSocketFd, (struct sockaddr *)serverAddress, serverAddressLen);
    if (bindRes < 0) {
        free(serverAddress);
        perror("bind error");
        return (-1);
    }
    //listening
    int listenRes = listen(serverSocketFd, 10);
    if (listenRes < 0) {
        perror("listen error");
        return (-1);
    }
    //accepting
    startAcceptingIncomingConnections(serverSocketFd);
    
    shutdown(serverSocketFd, SHUT_RDWR);
    free(serverAddress);
    return (0); 
}
