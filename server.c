#include "GCS.h"

int main() {

    int serverSocketFd = createIPV4Socket();
    if (serverSocketFd < 0) {
        perror("socket error");
        return (-1);
    }
    struct sockaddr_in *ClientAddress = NULL;
    struct sockaddr_in *serverAddress = createIPV4Address(NULL, 8080);
    socklen_t serverAddressLen = sizeof(*serverAddress);
    socklen_t ClientAddressLen = sizeof(*ClientAddress);
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
    char *getResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>GET response</h1></body></html>";
    char *postResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>POST response</h1></body></html>";
    int clientSocketFd = accept(serverSocketFd, (struct sockaddr *)ClientAddress, &ClientAddressLen);
    if (clientSocketFd < 0) {
        perror("accept error");
        return (-1);
    }
    printf("accept success, Client Fd: %d\n", clientSocketFd);
    char httpReq[8192] = "";
    while (1)
    {
        memset(httpReq, 0, sizeof(httpReq));
        int bytesRecv = recv(clientSocketFd, httpReq, sizeof(httpReq), 0);
        if(bytesRecv <= 0) {
            perror("recv error");
            break;
        }
        printf("Client's Request: %s", httpReq);
        char *getReq = strnstr(httpReq, "GET", 3);
        char *postReq = strnstr(httpReq, "POST", 4);
        int bytesSent = 0;
        if(getReq != NULL) {
            bytesSent = send(clientSocketFd, getResponse, strlen(getResponse), 0);
        }
        else if(postReq != NULL) {
            bytesSent = send(clientSocketFd, postResponse, strlen(postResponse), 0);
        }
        else {
            printf("Unknown Request\n");
        }
        if(bytesSent < 0) {
            perror("send error");
            break;
        }
        printf("send success\n");
    }
    close(clientSocketFd);
    free(serverAddress);
    shutdown(serverSocketFd, SHUT_RDWR);
    return (0); 
}
