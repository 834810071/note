//
// Created by jxq on 20-2-12.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <errno.h>

using namespace std;

#define INVALID_FD -1

int main()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        cout << "create listen socket error." << endl;
        return -1;
    }

    int oldSocketFlag = fcntl(listenfd, F_GETFL, 0);
    int newSocketFlag = oldSocketFlag | O_NONBLOCK;
    if (fcntl(listenfd, F_SETFL, newSocketFlag) == -1)
    {
        close(listenfd);
        cout << "set listenfd to nonblock error." << endl;
        return -1;
    }

    // 复用地址和端口号
    int on = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, (char*)&on, sizeof(on));


    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(3000);
    if (bind(listenfd, (struct sockaddr*)&bindaddr, sizeof bindaddr) == -1)
    {
        cout << "bind listen socket errror" << endl;
        close(listenfd);
        return -1;
    }

    if (listen(listenfd, SOMAXCONN) == -1)
    {
        cout << "listen error" << endl;
        close(listenfd);
        return -1;
    }

    // int poll(struct pollfd* fds, nfds_t nfds, int timeout);
    vector<pollfd> fds;
    pollfd listenfd_info;
    listenfd_info.fd = listenfd;
    listenfd_info.events = POLLIN;  // 数据可读
    listenfd_info.revents = 0;
    fds.push_back(listenfd_info);

    bool exist_invalid_fd;
    int n;
    while (true)
    {
        exist_invalid_fd = false;
        n = poll(&fds[0], fds.size(), 1000);
        if (n < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            break;
        }
        else if (n == 0)
        {
            continue;
        }
        for (size_t i = 0; i < fds.size(); ++i)
        {
            if (fds[i].revents && POLLIN)
            {
                if (fds[i].fd == listenfd)
                {
                    struct sockaddr_in clientaddr;
                    socklen_t clientaddrlen = sizeof clientaddr;
                    int clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
                    if (clientfd != -1)
                    {
                        int oldSocketFlag = fcntl(clientfd, F_GETFL, 0);
                        int newSocketFlag = oldSocketFlag | O_NONBLOCK;
                        if (fcntl(clientfd, F_SETFL, newSocketFlag) == -1)
                        {
                            close(clientfd);
                            cout << "set clientfd to nonblock error" << endl;
                        }
                        else
                        {
                            struct pollfd client_fd_info;
                            client_fd_info.fd = clientfd;
                            client_fd_info.events = POLLIN;
                            client_fd_info.revents = 0;
                            fds.push_back(client_fd_info);
                            cout << "new client accepted, clientfd: " << clientfd << endl;
                        }
                    }
                }
                else
                {
                    char buf[64] = {0};
                    int m = recv(fds[i].fd, buf, 64, 0);
                    if (m <= 0)
                    {
                        if (errno != EINTR && errno != EWOULDBLOCK)
                        {
                            for (vector<pollfd>::iterator iter = fds.begin(); iter != fds.end(); ++iter)
                            {
                                if (iter->fd == fds[i].fd)
                                {
                                    cout << "client diasconnected, clientfd = " << fds[i].fd << endl;
                                    close(fds[i].fd);
                                    iter->fd = INVALID_FD;
                                    exist_invalid_fd = true;
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        cout << "recv from client: " << buf << ", clientfd: " << fds[i].fd << endl;
                    }
                }
            }
            else if (fds[i] .revents && POLLERR)
            {

            }
        }
        if (exist_invalid_fd)
        {
            for (vector<pollfd>::iterator iter = fds.begin(); iter != fds.end(); )
            {
                if (iter->fd == INVALID_FD)
                {
                    iter = fds.erase(iter);
                }
                else
                {
                    ++iter;
                }
            }
        }
    }

    for (vector<pollfd>::iterator iter = fds.begin(); iter != fds.end(); ++iter)
    {
        close(iter->fd);
    }

    return 0;
}