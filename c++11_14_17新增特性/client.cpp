//
// Created by jxq on 20-1-10.
//

#include <iostream>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <zconf.h>
#include <arpa/inet.h>

using namespace std;

int main()
{
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);

    if (clientfd == -1)
    {
        cout << "create client socket error." << endl;
        return -1;
    }

    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(20000);

    if (bind(clientfd, (struct sockaddr*)&bindaddr, sizeof bindaddr) == -1)
    {
        cout << "bind error." << endl;
        return -1;
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(3000);
    if (connect(clientfd, (struct sockaddr*)&serveraddr, sizeof serveraddr) == -1)
    {
        cout << "connect error." << endl;
        return -1;
    }

    int ret = send(clientfd, "helloworld", strlen("helloworld"), 0);
    if (ret != strlen("helloworld"))
    {
        cout << "send data error." << endl;
        return -1;
    }

    cout << "send data successfully, data: " << "helloworld" << endl;
    char recvbuf[32] = {0};
    ret = recv(clientfd, recvbuf, 32, 0);
    if (ret > 0)
    {
        cout << "recv data successfully, data: " << recvbuf << endl;
    }
    else
    {
        cout << "recv data error." << endl;
    }
   // close(clientfd);
   while (true)
   {
       sleep(3);
   }

    return 0;
}