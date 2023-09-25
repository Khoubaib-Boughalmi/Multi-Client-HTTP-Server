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
    int clientSocketFd = accept(serverSocketFd, (struct sockaddr *)serverAddress, &serverAddressLen);
    if (clientSocketFd < 0) {
        perror("accept error");
        return (-1);
    }
    printf("accept success, Client Fd: %d\n", clientSocketFd);
    char buf[5024] = "";
    int bytesRecv = recv(clientSocketFd, buf, sizeof(buf), 0);
    if(bytesRecv < 0) {
        perror("recv error");
        return (-1);
    }
    printf("Client's Request: %s", buf);
    char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello World</h1></body></html>";
    int bytesSent = send(clientSocketFd, response, strlen(response), 0);
    if (bytesSent < 0) {
        perror("send error");
        return (-1);
    }
    printf("send success\n");
    free(serverAddress);
    close(clientSocketFd);
    close(serverSocketFd);
    return (0); 
}
