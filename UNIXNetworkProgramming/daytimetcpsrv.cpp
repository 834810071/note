//
// Created by jxq on 19-9-7.
//

#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <error.h>
#include <time.h>
#include <unistd.h>
#include <cstring>

#define ERR_EXIT(m) \
        do \
        {\
            perror(m);\
            exit(EXIT_FAILURE);\
        }while (0); \

int main()
{
    int sockfd, connfd;
    char buf[1024];
    struct sockaddr_in sockAddr;
    int n;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ERR_EXIT("socket");
    }

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddr.sin_port = htons(9981);

    if (bind(sockfd, (struct sockaddr*)&sockAddr, sizeof sockAddr) < 0)
    {
        ERR_EXIT("bind");
    }

    if (listen(sockfd, SOMAXCONN) < 0)
    {
        ERR_EXIT("listen");
    }

    struct sockaddr_in peeraddr;
    socklen_t  peerlen = sizeof peeraddr;
    for ( ; ; )
    {
        if ((connfd = accept(sockfd,(struct sockaddr*)&peeraddr, &peerlen)) < 0)
        {
            ERR_EXIT("accept");
        }

        time_t t = time(NULL);
        snprintf(buf, sizeof buf, "%.24s\r\n", ctime(&t));
        if (write(connfd, buf,  strlen(buf)) < 0)
        {
            ERR_EXIT("writev");
        }
        close(connfd);
    }
    return 0;
}