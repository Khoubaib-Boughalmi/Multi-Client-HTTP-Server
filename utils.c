#include "GCS.h"

int createIPV4Socket(void) {
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    return (sockfd);
}

struct sockaddr_in *createIPV4Address(char *ip, int port) {
    struct sockaddr_in *addr = malloc(sizeof(struct sockaddr_in));

    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    if (ip == NULL)
        addr->sin_addr.s_addr = htonl(INADDR_ANY);
    else
        inet_pton(AF_INET, ip, &addr->sin_addr.s_addr);
    return addr;
}

int connectSocket(int sockfd, struct sockaddr_in *addr) {
    int res = connect(sockfd, (struct sockaddr *)addr, sizeof(*addr));
    return (res);
}

t_acceptSocket *acceptSocket(int serverSocketFd) {
    t_acceptSocket *acceptedSocket = malloc(sizeof(t_acceptSocket));
    acceptedSocket->clientAddress = NULL;
    socklen_t clientAddressLen = sizeof(*acceptedSocket->clientAddress);
    acceptedSocket->clientSocketFd = accept(serverSocketFd, (struct sockaddr *)acceptedSocket->clientAddress, &clientAddressLen);

    if (acceptedSocket->clientSocketFd < 0) {
        acceptedSocket->error = acceptedSocket->clientSocketFd;
        acceptedSocket->fullyAccepted = false;
        perror("accept error");
        free(acceptedSocket);
        return (NULL);
    }
    printf("accept success, Client Fd: %d\n", acceptedSocket->clientSocketFd);
    acceptedSocket->error = 0;
    acceptedSocket->fullyAccepted = true;
    return  (acceptedSocket);
}

void receiveIncommingRequestAndRespond (int clientSocketFd) {
    char httpReq[8192] = "";
    char *getResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>GET response</h1></body></html>";
    char *postResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>POST response</h1></body></html>";
    while (1)
    {
        memset(httpReq, 0, sizeof(httpReq));
        int bytesRecv = recv(clientSocketFd, httpReq, sizeof(httpReq), 0);
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
}