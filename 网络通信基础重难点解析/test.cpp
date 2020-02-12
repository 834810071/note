//
// Created by jxq on 20-2-12.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

using namespace std;

bool connect_to_server(const char* server, short port)
{
    int hSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (hSocket == -1)
    {
        return false;
    }

    struct sockaddr_in addrSrv = {0};
    struct hostent* pHosttent = NULL;

    if (addrSrv.sin_addr.s_addr = inet_addr(server) == INADDR_NONE)
    {
        struct addrinfo hints = {0};
        hints.ai_flags = AI_CANONNAME;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        struct addrinfo* res;
        int n = getaddrinfo(server, NULL, &hints, &res);
        if(n == 0)
        {
            //调用成功，使用 res
            addrSrv.sin_addr.s_addr = ((sockaddr_in*)(res->ai_addr))->sin_addr.s_addr;

            //释放 res 资源
            freeaddrinfo(res);
        }
//        pHosttent = gethostbyname(server);
//        if (pHosttent == NULL)
//        {
//            return false;
//        }


        //addrSrv.sin_addr.s_addr = *((unsigned long*)pHosttent->h_addr_list[0]);

        //addrSrv.sin_addr.s_addr = *((unsigned long*)pHosttent->h_addr_list[0]);
    }

    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(port);
    int ret = connect(hSocket, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
    if (ret == -1)
        return false;
    return true;
};

int main()
{
    if (connect_to_server("baidu.com", 80))
        printf("connect successfully.\n");
    else
        printf("connect error.\n");

    return 0;
}