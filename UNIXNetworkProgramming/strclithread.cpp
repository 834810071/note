//
// Created by jxq on 19-9-15.
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
#include <pthread.h>

using namespace std;

#define ERR_EXIT(m) \
        do \
        { \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while (0);
void *copyto(void* arg);

static int sockfd;

void str_cli(int connfd)
{
    char recv[1024];
    memset(recv, 0, sizeof recv);
    pthread_t tid;
    sockfd = connfd;

    pthread_create(&tid, NULL, copyto, NULL);
    while (read(connfd, recv, sizeof recv) > 0)
    {
        fputs(recv, stdout);
        memset(recv, 0, sizeof recv);
    }
}

void *copyto(void* arg)
{
    char buf[1024];
    int n;
    memset(buf, 0, sizeof buf);
    while (fgets(buf, sizeof buf, stdin) != NULL)
    {
        write(sockfd, buf, sizeof buf);
        memset(buf, 0, sizeof buf);
    }
    shutdown(sockfd, SHUT_WR);
    return (NULL);
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
        str_cli(sockfd[i]);
    //close(sockfd);
    return 0;
}