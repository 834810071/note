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
#include <signal.h>
#include <wait.h>

using namespace std;

#define ERR_EXIT(m) \
        do \
        { \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while (0);

void str_echo00(int connfd)
{
    char recv[1024];
    int n;
    memset(recv, 0, sizeof recv);
again:
    while ((n = read(connfd, recv, sizeof(recv))) != 0)
    {
        printf("msg: %s", recv);
        write(connfd, recv, n);
        memset(recv, 0, sizeof recv);
    }
    if (n < 0 && errno == EINTR)
    {
        goto again;
    }
    else
    {
        exit(0);
    }
}

void str_echo01(int connfd)
{
    char recv[1024];
    int n;
    memset(recv, 0, sizeof recv);
    int a, b;
    again:
    while ((n = read(connfd, recv, sizeof(recv))) != 0)
    {
        printf("msg: %s", recv);
        if (sscanf(recv, "%d%d", &a, &b) != 2)
        {
            ERR_EXIT("sscanf");
        }
        snprintf(recv, sizeof recv, "%d\n", a+b);

        write(connfd, recv, sizeof recv);
        memset(recv, 0, sizeof recv);
    }

    if (n < 0 && errno == EINTR)
    {
        goto again;
    }
    else
    {
        exit(0);
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

void str_echo02(int connfd)
{
    char recv[1024];
    int n;
    memset(recv, 0, sizeof recv);
    int a, b;
    args args1;
    again:
    while ((n = read(connfd, &args1, sizeof(args1))) != 0)
    {
        printf("msg: %ld, %ld\n", args1.num1, args1.num2);
        result res;
        res.res = args1.num1 + args1.num2;
        write(connfd, &res, sizeof res);
    }

    if (n < 0 && errno == EINTR)
    {
        goto again;
    }
    else
    {
        exit(0);
    }
}

void sig_chld(int singo)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return;
}

int main()
{
    int listenfd, connfd;
    struct sockaddr_in sockAddr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ERR_EXIT("socket");
    }

    socklen_t socklen = sizeof sockAddr;
    bzero(&sockAddr, socklen);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddr.sin_port = htons(9981);
    int on = 1;
    if ((setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int))) < 0)
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

    struct sockaddr_in peerAddr;
    socklen_t peerlen;
    pid_t pid;
    signal(SIGCHLD, sig_chld);
    while (true)
    {
        if ((connfd = accept(listenfd, (struct sockaddr*)&peerAddr, &peerlen)) < 0)
        {
            ERR_EXIT("accept");
        }

        if ((pid = fork()) == 0)
        {
            close(listenfd);
            str_echo00(connfd);
            exit(0);
        }
        close(connfd);
    }


    return 0;
}