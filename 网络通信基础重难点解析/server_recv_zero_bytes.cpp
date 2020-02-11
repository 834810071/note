//
// Created by jxq on 20-2-11.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        cout << "create listen socket error." << endl;
        return -1;
    }

    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(3000);

    if (bind(listenfd, (struct sockaddr*)&bindaddr, sizeof(bindaddr)) == -1)
    {
        cout << "bind listen socket error" << endl;
        return -1;
    }

    if (listen(listenfd, SOMAXCONN) == -1)
    {
        cout << "listen error." << endl;
        return -1;
    }

    int clientfd;
    struct sockaddr_in clientaddr;
    socklen_t clientaddrlen = sizeof(clientaddr);
    clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
    if (clientfd != -1)
    {
        while (true)
        {
            char recvbuf[32] = {0};
            int ret = recv(clientfd, recvbuf, 32, 0);
            if (ret > 0)
            {
                cout << "recv data from client, data : " << recvbuf << endl;
            }
            else if (ret == 0)
            {
                cout << "recv 0 byte data." << endl;
                continue;
            }
            else
            {
                cout << "recv data error." << endl;
                break;
            }
        };
    }
    close(clientfd);
    close(listenfd);

    return 0;
}