#include "GCS.h"

int connectSocket(int sockfd, struct sockaddr_in *addr) {
    int res = connect(sockfd, (struct sockaddr *)addr, sizeof(*addr));
    return (res);
}