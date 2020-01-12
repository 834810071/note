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
#include <vector>

using namespace std;

int main()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        cout << "create listen socket error." << endl;
        return -1;
    }

    struct sockaddr_in bindarr;
    bindarr.sin_family = AF_INET;
    bindarr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindarr.sin_port = htons(3000);

    if (bind(listenfd, (struct sockaddr*)&bindarr, sizeof bindarr) == -1)
    {
        cout << "bind listen socket error." << endl;
        return -1;
    }

    if (listen(listenfd, SOMAXCONN) == -1)
    {
        cout << "listen error." << endl;
        return -1;
    }

    while (true)
    {
        struct sockaddr_in clientaddr;
        socklen_t clientaddrlen = sizeof clientaddr;
        vector<int> clientfds;
        int clientfd = accept(listenfd, (struct sockaddr*)& clientaddr, &clientaddrlen);
        if (clientfd != -1)
        {
            char recvbuf[32] = {};
            int ret = recv(clientfd, recvbuf, 32, 0);
            if (ret > 0)
            {
                cout << "recv data from client, data: " << recvbuf << endl;
                ret = send(clientfd, recvbuf, strlen(recvbuf), 0);
                if (ret != strlen(recvbuf))
                {
                    cout << "send data error." << endl;
                }
                else
                {
                    cout << "send to client successful, data : " << recvbuf << endl;
                }
            }
            else
            {
                cout << "recv data error." << endl;
            }
            //close(clientfd);
            clientfds.push_back(clientfd);
        };
    }
    close(listenfd);


    return 0;
}