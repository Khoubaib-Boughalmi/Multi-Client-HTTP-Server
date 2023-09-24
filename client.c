#include "GCS.h"

int main(void) {
    int sockfd;
    sockfd = createIPV4Socket();
    if (sockfd < 0) {
        printf("socket error\n");
        return (-1);
    } 
    struct sockaddr_in *addr = createIPV4Address();
    int res = connectSocket(sockfd, addr);
    if (res < 0) {
        printf("connect error\n");
        return (-1);
    }
    printf("connect success\n");
    char *http = "GET / HTTP/1.1\r\nHost: www.google.com\r\n\r\n";
    send(sockfd, http, strlen(http), 0);
    char buf[5024] = "";
    int bytesRecv = recv(sockfd, buf, sizeof(buf), 0);
    if(bytesRecv < 0) {
        printf("recv error\n");
        return (-1);
    }
    printf("%s", buf);
    free(addr);
    return (0);
}
