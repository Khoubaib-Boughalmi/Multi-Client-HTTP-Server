#include "GCS.h"

t_acceptSocket *acceptSocket(int serverSocketFd) {
    t_acceptSocket *localAcceptedSocket = malloc(sizeof(t_acceptSocket));
    localAcceptedSocket->clientAddress = NULL;
    socklen_t clientAddressLen = sizeof(*localAcceptedSocket->clientAddress);
    localAcceptedSocket->clientSocketFd = accept(serverSocketFd, (struct sockaddr *)localAcceptedSocket->clientAddress, &clientAddressLen);

    if (localAcceptedSocket->clientSocketFd < 0) {
        localAcceptedSocket->error = localAcceptedSocket->clientSocketFd;
        localAcceptedSocket->fullyAccepted = false;
        perror("accept error");
        free(localAcceptedSocket);
        return (NULL);
    }
    printf("accept success, Client Fd: %d\n", localAcceptedSocket->clientSocketFd);
    localAcceptedSocket->error = 0;
    localAcceptedSocket->fullyAccepted = true;
    return  (localAcceptedSocket);
}

void receiveIncommingRequestAndRespond (int clientSocketFd) {
    char httpReq[32768] = "";
    char *getResponse = "HTTP/1.1 200 OK\r\nDate: Sat, 24 Sep 2023 12:00:00 GMT\r\nContent-Type: text/html\r\nConnection: keep-alive\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>Sample Page</title>\r\n<style>body {background-color: #f0f0f0;margin: 0;padding: 0;}h1 {color: blue;}p {color: red;}</style>\r\n</head>\r\n<body>\r\n<h1>Hello, World!</h1>\r\n<p>This is a sample page.</p>\r\n</body>\r\n</html>\r\n";
    char *postResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>POST response</h1></body></html>";

    memset(httpReq, 0, sizeof(httpReq));
    int bytesRecv = recv(clientSocketFd, httpReq, sizeof(httpReq), 0);
    if(!bytesRecv || bytesRecv < 0) {
        if(bytesRecv < 0)
        {
            printf("bytesRecv: %d\n", bytesRecv);
            perror("recv error");
        }
        else
        {
            close(clientSocketFd);
            printf("Client closed connection\n");
        }
        return ;
    }
    printf("Client FD %d\n", clientSocketFd);
    printf("Client's Request: %s", httpReq);
    char *getReq = strstr(httpReq, "GET");
    char *postReq = strstr(httpReq, "POST");
    int bytesSent = 0;
    if(getReq != NULL) {
        bytesSent = send(clientSocketFd, getResponse, strlen(getResponse), 0);
    }
    else if(postReq != NULL) {
        bytesSent = send(clientSocketFd, postResponse, strlen(postResponse), 0);
    }
    else {
        printf("Unknown Request\n");
        bytesSent = -1;
    }
    if(bytesSent < 0) {
        perror("send error");
        return;
    }
    printf("send success\n");
    close(clientSocketFd);
}

void *startAcceptingIncomingConnections(int serverSocketFd) {
    while (1)
    {
        acceptedSocket = acceptSocket(serverSocketFd);
        if(!acceptedSocket) {
            shutdown(serverSocketFd, SHUT_RDWR);
            perror("accept error");
            return (NULL);   
        }
        printf("Client FD %d\n", acceptedSocket->clientSocketFd);
        pthread_t thread_id;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        // Set the thread as detached
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_create(&thread_id, &attr, receiveAndRespond, &(acceptedSocket->clientSocketFd));
    }
    return (NULL);   
}

void *receiveAndRespond(void *clientSocketFd_arg) {
  
    //receiving and responding
    int clientSocketFd = *(int *)clientSocketFd_arg;
    receiveIncommingRequestAndRespond (clientSocketFd);
    return (NULL);
}