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
    acceptedSocket->ClientAddress = NULL;
    socklen_t ClientAddressLen = sizeof(*acceptedSocket->ClientAddress);
    acceptedSocket->ClientSocketFd = accept(serverSocketFd, (struct sockaddr *)acceptedSocket->ClientAddress, &ClientAddressLen);

    if (acceptedSocket->ClientSocketFd < 0) {
        acceptedSocket->error = acceptedSocket->ClientSocketFd;
        acceptedSocket->fullyAccepted = false;
        perror("accept error");
        free(acceptedSocket);
        return (NULL);
    }
    printf("accept success, Client Fd: %d\n", acceptedSocket->ClientSocketFd);
    acceptedSocket->error = 0;
    acceptedSocket->fullyAccepted = true;
    return  (acceptedSocket);
}