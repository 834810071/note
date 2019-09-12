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
#include <poll.h>
#include <limits.h>

using namespace std;

#define ERR_EXIT(m) \
        do \
        { \
            perror(m);\
            exit(EXIT_FAILURE);\
        } while (0);    \


int main()
{
    int listenfd, connfd;
    struct pollfd polls[1024];
    int maxi;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ERR_EXIT("socket");
    }
    polls[0].fd = listenfd;
    polls[0].events = POLLIN | POLLRDNORM;
    for (int i = 1; i < 1024; ++i)
    {
        polls[i].fd = -1;
        polls[i].events = -1;
    }

    struct sockaddr_in sockAddr;
    socklen_t socklen = sizeof sockAddr;
    bzero(&sockAddr, socklen);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(9981);
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int on = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) < 0)
    {
        ERR_EXIT("setsockopt");
    }

    if (bind(listenfd, (struct sockaddr*)&sockAddr, socklen) < 0)
    {
        ERR_EXIT("bind");
    }

    if (listen(listenfd, SOMAXCONN) < 0)
    {
        ERR_EXIT("listen");
    }
    maxi = 0;
    int nready;
    struct sockaddr_in clientAddr;
    socklen_t clientlen = sizeof clientAddr;
    char buf[1024];
    memset(buf, 0, sizeof buf);
    int n;

    for ( ; ; )
    {
        nready = ::poll(polls, maxi+1, 100000);
        if (polls[0].revents & POLLRDNORM)
        {
            if ((connfd = accept(listenfd, (struct sockaddr *)&clientAddr, &clientlen)) < 0)
            {
                ERR_EXIT("connfd");
            }
            int i;
            for (i = 1; i < 1024; ++i)
            {
                if (polls[i].fd < 0)
                {
                    polls[i].fd = connfd;
                    polls[i].events = POLLRDNORM;
                    break;
                }
            }
            maxi = max(maxi, i);
            if (--nready <= 0)
            {
                continue;
            }
        }

        for (int i = 1; i <= maxi; ++i)
        {
            if (polls[i].fd < 0)
            {
                continue;
            }
            if (polls[i].revents & (POLLRDNORM | POLLERR))
            {
                if ((n = read(polls[i].fd, buf, sizeof buf)) < 0)
                {
                    if (errno == ECONNRESET)
                    {
                        close(polls[i].fd);
                        polls[i].fd = -1;
                    }
                    else
                    {
                        ERR_EXIT("Read");
                    }
                }
                else if (n == 0)
                {
                    close(polls[i].fd);
                    polls[i].fd = -1;
                }
                else
                {
                    write(polls[i].fd, buf, n);
                    memset(buf, 0, sizeof buf);
                }
                if (--nready <= 0)
                {
                    break;
                }
            }

        }

    }
    return 0;
}