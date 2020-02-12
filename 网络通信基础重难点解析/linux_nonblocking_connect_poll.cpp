//
// Created by jxq on 20-2-12.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

using namespace std;

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 3000
#define SEND_DATA "helloworld"

int main(int argc, char** argv)
{
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        cout << "creat client socket error." << endl;
        return -1;
    }

    int oldSocketFlag = fcntl(clientfd, F_GETFL, 0);
    int newSocketFlag = oldSocketFlag | O_NONBLOCK;
    if (fcntl(clientfd, F_SETFL, newSocketFlag) == -1)
    {
        close(clientfd);
        cout << "set socket to nonblock error." << endl;
        return -1;
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serveraddr.sin_port = htons(SERVER_PORT);

    for (; ;)
    {
        int ret = connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
        if (ret == 0)
        {
            cout << "connect to server successfully." << endl;
            close(clientfd);
            return 0;
        }
        else if (ret == -1)
        {
            if (errno == EINTR)
            {
                cout << "connecting interrupted by signal, try again." << endl;
                continue;
            }
            else if (errno == EWOULDBLOCK)
            {
                break;
            }
            else
            {
                close(clientfd);
                break;
            }
        }
    }

    pollfd event;
    event.fd = clientfd;
    event.events = POLLIN;
    int timeout = 3000;
    if (poll(&event, 1, timeout) != 1)
    {
        close(clientfd);
        cout << "[poll] connect to server error." << endl;
        return -1;
    }
//
//    if (!(event.revents & POLLOUT))
//    {
//        close(clientfd);
//        cout << "[POLLOUT] connect to server error." << endl;
//    }

    int err;
    socklen_t len = static_cast<socklen_t>(sizeof err);
    if (::getsockopt(clientfd, SOL_SOCKET, SO_ERROR, &err, &len) < 0)
    {
        return -1;
    }

    if (err == 0)
    {
        cout << "connect to server successfully" << endl;
    } else{
        cout << "connect to server error." << endl;
    }
    close(clientfd);

    return 0;
}
