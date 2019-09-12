//
// Created by jxq on 19-9-12.
//

#include <iostream>
#include <sys/socket.h>
#include <string>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char** argv)
{
    char *ptr, **pptr;
    char str[1024];
    struct hostent *hptr;
    while (--argc > 0)
    {
        ptr = *++argv;
        if ((hptr = gethostbyname(ptr)) == NULL)
        {
            printf("gethostbyname error for host: %s : %s", ptr, hstrerror(h_errno));
            continue;
        }

        printf("offical hostname : %s\n", hptr->h_name);
        for (pptr = hptr->h_aliases; *pptr != NULL; ++pptr)
        {
            printf("\talias: %s\n", *pptr);
        }

        switch(hptr->h_addrtype)
        {
            case AF_INET:
                pptr = hptr->h_addr_list;
                for ( ; *pptr != NULL; ++pptr)
                {
                    printf("\taddress: %s\n", inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
                }
                break;
            default:
                break;
        }
    }


    return 0;
}