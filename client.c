#include "GCS.h"

int main(void) {
    int sockfd;
    sockfd = createIPV4Socket();
    if (sockfd < 0) {
        perror("socket error");
        return (-1);
    } 
    struct sockaddr_in *addr = createIPV4Address("127.0.0.1", 8080);
    int res = connectSocket(sockfd, addr);
    if (res < 0) {
        perror("connect error");
        return (-1);
    }
    printf("connect success\n");
    char *http = "GET / HTTP/1.1\r\nHost: www.boobs.com\r\n\r\n";
    send(sockfd, http, strlen(http), 0);
    char buf[5024] = "";
    int bytesRecv = recv(sockfd, buf, sizeof(buf), 0);
    if(bytesRecv < 0) {
        perror("recv error");
        return (-1);
    }
    printf("%s", buf);
    free(addr);
    return (0);
}
