//
// Created by jxq on 19-9-10.
//

#include <sys/socket.h>
#include <sys/types.h>          /* See NOTES */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <zconf.h>
#include <string>
#include <string.h>
#include <arpa/inet.h>

using namespace std;

#define ERR_EXIT(m) \
        do \
        { \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while (0);

void str_cli(int connfd)
{
    char recv[1024], buf[1024];
    int n;
    memset(recv, 0, sizeof recv);
    memset(buf, 0, sizeof buf);
    while (fgets(buf, sizeof buf, stdin) != NULL)
    {

        write(connfd, buf, sizeof buf);
        if ((n = read(connfd, recv, sizeof recv)) != 0)
            printf("msg: %s", recv);
        else
        {
            exit(0);
        }
        memset(recv, 0, sizeof recv);
        memset(buf, 0, sizeof buf);
    }
}

int main()
{
    int sockfd[5];
    struct sockaddr_in sockAddr;

    for (int i = 0; i < 5; ++i)
    {
        if ((sockfd[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            ERR_EXIT("socket");
        }

        socklen_t socklen = sizeof sockAddr;
        bzero(&sockAddr, socklen);
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(9981);
        inet_pton(AF_INET, "127.0.0.1",  &sockAddr.sin_addr);

        if (connect(sockfd[i], (struct sockaddr*)&sockAddr, socklen) < 0)
        {
            ERR_EXIT("connect");
        }
    }

    str_cli(sockfd[0]);
    //close(sockfd);
    return 0;
}