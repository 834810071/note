//
// Created by jxq on 19-9-12.
//

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <zconf.h>

using namespace std;

#define ERR_EXIT(m) \
        do \
        { \
            perror(m);\
            exit(EXIT_FAILURE);\
        } while (0);    \


int main()
{
    int listenfd, client[FD_SETSIZE], connfd, n;
    char buf[1024];
    int maxfd, maxi;
    fd_set rset, allset;
    FD_ZERO(&rset);
    FD_ZERO(&allset);
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ERR_EXIT("socket");
    }
    struct sockaddr_in sockAddr;
    socklen_t socklen = sizeof sockAddr;
    bzero(&sockAddr, socklen);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddr.sin_port = htons(9981);
    int on = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) < 0)
    {
        ERR_EXIT("setsockopt");
    }

    if (bind(listenfd, (struct sockaddr *)&sockAddr, socklen) < 0)
    {
        ERR_EXIT("bind");
    }

    if (listen(listenfd, SOMAXCONN) < 0)
    {
        ERR_EXIT("listen");
    }
    maxfd = listenfd;
    maxi = -1;
    memset(client, -1, sizeof client);
    struct sockaddr_in peerAddr;
    socklen_t peerlen = sizeof peerAddr;
    int nready;
    memset(buf, 0, sizeof(buf));
    FD_SET(listenfd, &allset);
    for ( ; ; )
    {
        rset = allset;
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &rset))
        {
            if ((connfd = accept(listenfd, (struct sockaddr *)&peerAddr, &peerlen)) < 0)
            {
                ERR_EXIT("connfd");
            }
            int i;
            for (i = 0; i < FD_SETSIZE; ++i)
            {
                if (client[i] < 0)
                {
                    client[i] = connfd;
                    break;
                }
            }
            maxi = max(maxi, i);
            maxfd = max(maxfd, connfd);
            FD_SET(connfd, &allset);
            if (--nready <= 0)
            {
                continue;
            }
        }

        for (int i = 0; i <= maxi; ++i)
        {
            if (client[i] > 0)
            {
                if (FD_ISSET(client[i], &rset))
                {
                    if ((n = read(client[i], buf, sizeof buf)) <= 0)
                    {
                        close(client[i]);
                        client[i] = -1;
                        FD_CLR(client[i], &allset);
                    }
                    else
                    {
                        printf("msg: %s", buf);
                        write(client[i], buf, n);
                    }
                }
                memset(buf, 0, sizeof(buf));
                if (--nready <= 0)
                {
                    break;
                }
            }
        }
    }

    return 0;
}