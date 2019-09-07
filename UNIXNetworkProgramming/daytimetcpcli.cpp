//
// Created by jxq on 19-9-7.
//

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <memory.h>
#include <iostream>

using namespace std;

#define ERR_EXIT(m) \
        do \
        { \
            perror(m);  \
            exit(EXIT_FAILURE);\
        } while (0);\

int main(int argc, char** argv)
{
    //cout << "hello" << endl;
    int sockfd;
    struct sockaddr_in sockAddr;
    bzero(&sockAddr, sizeof(sockAddr));

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ERR_EXIT("socket");
    }

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(9981);
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
    {
        ERR_EXIT("connect");
    }

    int n;
    char buf[1024];
    while ((n = recv(sockfd, buf, sizeof(buf), 0)) > 0)
    {
        buf[n] = 0;
        fputs(buf, stdout);
    }
    if (n < 0)
    {
        ERR_EXIT("recv");
    }
    return 0;
}