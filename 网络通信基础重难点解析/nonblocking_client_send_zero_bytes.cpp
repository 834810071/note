//
// Created by jxq on 20-2-11.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <zconf.h>

using namespace std;

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 3000
#define SEND_DATA ""

int main()
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
    if (connect(clientfd, (struct sockaddr*)&serveraddr, sizeof serveraddr) == -1)
    {
        cout << "connect socket error" << endl;
        close(clientfd);
        return -1;
    }
    
    int oldSocketFlag = fcntl(clientfd, F_GETFL, 0);
    int newSocketFlag = oldSocketFlag | O_NONBLOCK;
    if (fcntl(clientfd, F_SETFL, newSocketFlag)== -1)
    {
        close(clientfd);
        cout << "set socket to nonblock error." << endl;
        return -1;
    }
    
    int count = 0;
    while (true)
    {
        int ret = send(clientfd, SEND_DATA, 0, 0);
        if (ret == -1)
        {
            if (errno == EWOULDBLOCK)
            {
                cout << "send data error as TCP Window size is too small" << endl;
                continue;
            }
            else if (errno == EINTR)
            {
                cout << "sending data interrupted by signal." << endl;
                continue;
            }
            else 
            {
                cout << "Send data error." <<endl;
                break;
            }
        }
        else if (ret == 0)
        {
            cout << "send 0 byte data" << endl;
        }
        else 
        {
            count++;
            cout << "send data successfully, count = " << count << endl;
        }
        sleep(3);
    }
    close(clientfd);
    
    return 0;
}