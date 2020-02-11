//
// Created by jxq on 20-2-11.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <string.h>

using namespace std;

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 3000

int main(int argc, char** argv)
{
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        cout << "create client socket error." << endl;
        return -1;
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serveraddr.sin_port = htons(SERVER_PORT);

    if (connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
        cout << "connect socket error. " << endl;
        close(clientfd);
        return -1;
    }

    fd_set readset;
//    FD_ZERO(&readset);
//
//    FD_SET(clientfd, &readset);
//    timeval tm;
//    tm.tv_sec = 5;
//    tm.tv_usec = 0;

    int ret;
    int count = 0;
    fd_set backup_readset;
    memcpy(&backup_readset, &readset, sizeof(fd_set));
    while (true)
    {
        FD_ZERO(&readset);
        FD_SET(clientfd, &readset);

        if (memcmp(&readset, &backup_readset, sizeof(fd_set)) == 0)
        {
            //cout << "equal" << endl;
        }
        else
        {
            //cout << "not equal" << endl;
        }

        ret = select(clientfd + 1, &readset, NULL, NULL, NULL);
        //cout << "tm.tv_sec" << tm.tv_sec << ", tm.tv_usec" << tm.tv_usec << endl;
        if (ret == -1)
        {
            if (errno != EINTR)
            {
                break;
            }
        }
        else if (ret == 0)
        {
            //cout << "no event in specific time interval, count: " << count << endl;
            //++count;
        }
        else
        {
            if (FD_ISSET(clientfd, &readset))
            {
                char recvbuf[64];
                memset(recvbuf, 0, sizeof(recvbuf));
                int n = recv(clientfd, recvbuf, 64, 0);
                if (n < 0)
                {
                    if (errno != EINTR)
                    {
                        break;
                    }
                }
                else if (n == 0)
                {
                    break;
                }
                else
                {
                    cout << "recv data: " << recvbuf << endl;
                }
            }
        }
    }
    close(clientfd);
    return 0;
}