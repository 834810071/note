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

struct args
{
    long num1;
    long num2;
};

struct result
{
    long res;
};

void str_cli01(int connfd)
{
    char recv[1024], buf[1024];
    int n;
    result res;
    args args1;
    memset(recv, 0, sizeof recv);
    memset(buf, 0, sizeof buf);
    while (fgets(buf, sizeof buf, stdin) != NULL)
    {
        if (sscanf(buf, "%ld%ld", &args1.num1, &args1.num2) != 2)
        {
            ERR_EXIT("sscanf");
        }
        write(connfd, &args1, sizeof args1);
        if ((n = read(connfd, &res, sizeof res)) != 0)
            printf("msg: %ld\n", res.res);
        else
        {
            exit(0);
        }
        memset(recv, 0, sizeof recv);
        memset(buf, 0, sizeof buf);
    }
}

void str_cli02( int sockfd)
{
    char recv[1024], buf[1024];
    memset(recv, 0, sizeof recv);
    memset(buf, 0, sizeof buf);
    int maxfd1, stdineof = 0;
    fd_set fds;
    ssize_t n;

    FD_ZERO(&fds);
    for (; ;)
    {
        if (stdineof == 0)
        {
            FD_SET(fileno(stdin), &fds);
        }
        FD_SET(sockfd, &fds);
        maxfd1 = max(fileno(stdin), sockfd) + 1;
        select(maxfd1, &fds, NULL, NULL, NULL);
        if (FD_ISSET(sockfd, &fds))
        {
            if ((n = read(sockfd, recv, sizeof recv)) == 0)
            {
                if (stdineof == 1)
                {
                    return;;
                } else
                {
                    ERR_EXIT("read");
                }
            }
            write(fileno(stdout), recv, n);
        }

        if (FD_ISSET(fileno(stdin), &fds))
        {
            if ((n = read(fileno(stdin), buf, sizeof buf)) == 0)
            {
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(stdin), &fds);
                continue;
            }
            write(sockfd, buf, n);
        }
        memset(recv, 0, sizeof recv);
        memset(buf, 0, sizeof buf);
    }
}

int main()
{
//    int n;
//    char buf[1024];
//    if ((n = read(fileno(file), buf, sizeof buf)) != 0)
//    {
//        cout << buf << endl;
//        exit(0);
//    }
    int sockfd[FD_SETSIZE];
    struct sockaddr_in sockAddr;

    for (int i = 0; i < 1; ++i)
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
    for (int i = 0; i < 1; ++i)
        str_cli02(sockfd[i]);
    //close(sockfd);
    return 0;
}