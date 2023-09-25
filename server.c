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
    char *getResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>GET response</h1></body></html>";
    char *postResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>POST response</h1></body></html>";
    t_acceptSocket *acceptedSocket = acceptSocket(serverSocketFd);
    if(!acceptedSocket) {
        free(serverAddress);
        shutdown(serverSocketFd, SHUT_RDWR);
        perror("accept error");
        return (-1);
    }
    char httpReq[8192] = "";
    while (1)
    {
        memset(httpReq, 0, sizeof(httpReq));
        int bytesRecv = recv(acceptedSocket->ClientSocketFd, httpReq, sizeof(httpReq), 0);
        if(!bytesRecv || bytesRecv < 0) {
            if(bytesRecv < 0)
                perror("recv error");
            else
                printf("Client closed connection\n");
            break;
        }
        printf("Client's Request: %s", httpReq);
        char *getReq = strnstr(httpReq, "GET", 3);
        char *postReq = strnstr(httpReq, "POST", 4);
        int bytesSent = 0;
        if(getReq != NULL) {
            bytesSent = send(acceptedSocket->ClientSocketFd, getResponse, strlen(getResponse), 0);
        }
        else if(postReq != NULL) {
            bytesSent = send(acceptedSocket->ClientSocketFd, postResponse, strlen(postResponse), 0);
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
    free(serverAddress);
    close(acceptedSocket->ClientSocketFd);
    shutdown(serverSocketFd, SHUT_RDWR);
    return (0); 
}
