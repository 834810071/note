//
// Created by jxq on 20-2-6.
//

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <vector>

using namespace std;

#define INVALID_FD -1

int main(int argc, char** argv)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        cout << "create listen socket error" << endl;
        return -1;
    }

    // 初始化服务器地址
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(3000);
    if (bind(listenfd, (struct sockaddr*)&bindaddr, sizeof(bindaddr)) == -1)
    {
        cout << "bind listen socket error" << endl;
        close(listenfd);
        return -1;
    }

    if (listen(listenfd, SOMAXCONN) == -1)
    {
        cout << "listen error." << endl;
        close(listenfd);
        return -1;
    }

    vector<int> clientfds;
    int maxfd = listenfd;
    while (true)
    {
        fd_set readset;
        FD_ZERO(&readset);

        // 将侦听socket加入待检测的可读事件中去
        FD_SET(listenfd, &readset);

        // 将客户端fd加入待检测的可读事件中去
        int clientfdslength = clientfds.size();
        for (int i = 0; i < clientfdslength; ++i) {
            if (clientfds[i] != INVALID_FD)
                FD_SET(clientfds[i], &readset);
        }

        timeval tm;
        tm.tv_sec = 1;
        tm.tv_usec = 0;
        int ret = select(maxfd + 1, &readset, NULL, NULL, &tm);
        if (ret == -1)
        {
            if (errno != EINTR)
            {
                break;
            }
        }
        else if (ret == 0)
        {
            continue;
        }
        else
        {
          if (FD_ISSET(listenfd, &readset)) {
              struct sockaddr_in clientaddr;
              socklen_t clientaddrlen = sizeof(clientaddr);
              int clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
              if (clientfd == -1) {
                  break;
              }
              cout << "accept a client connection, fd: " << clientfd << endl;
              clientfds.push_back(clientfd);
              if (clientfd > maxfd) {
                  maxfd = clientfd;
              }
          } else {
              char recvbuf[64];
              int clientfdslength = clientfds.size();
              for (int i = 0; i < clientfdslength; ++i)
              {
                  if (clientfds[i] != -1 && FD_ISSET(clientfds[i], &readset)) {
                      memset(recvbuf, 0, sizeof(recvbuf));
                      int lenth = recv(clientfds[i], recvbuf, 64, 0);
                      if (lenth <= 0 && errno != EINTR) {
                          cout << "recv data error, clientfd: " << clientfds[i] << endl;
                          close(clientfds[i]);
                          clientfds[i] = INVALID_FD;
                          continue;
                      } else {
                          cout << "clientfd: " << clientfds[i] << ", recv data: " << recvbuf << endl;
                      }
                  }
              }
          }
        }

    }

    int clientfdslength = clientfds.size();
    for (int i = 0; i < clientfdslength; ++i)
    {
        if (clientfds[i] != INVALID_FD)
        {
            close(clientfds[i]);
        }
    }
    close(listenfd);
    return 0;
}
