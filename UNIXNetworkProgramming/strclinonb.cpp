//
// Created by jxq on 19-9-15.
//

#include <sys/socket.h>
#include <sys/types.h>          /* See NOTES */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <zconf.h>
#include <signal.h>
#include <wait.h>
#include <sys/time.h>
#include <fcntl.h>
#include <arpa/inet.h>

using namespace std;

#define ERR_EXIT(m) \
        do \
        { \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while (0);

char* gf_time(void)
{
    struct timeval tv;
    static char str[30];
    char *ptr;

    if (gettimeofday(&tv, NULL) < 0)
    {
        ERR_EXIT("gettimeofday error");
    }

    ptr = ctime(&tv.tv_sec);
    strcpy(str, &ptr[1]);
    snprintf(str + 8, sizeof(str)-8, ".%06ld", tv.tv_usec);
    return str;
}

FILE *fr;


#define MAXLINE 1024
#define STDIN_FILENO1 fileno(fp)
#define STDOUT_FILENO1 fileno(fp)

void str_cli(FILE *fp, int sockfd)
{
    fd_set rset, wset;
    int maxfd;

    int flags = ::fcntl(sockfd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int ret = ::fcntl(sockfd, F_SETFL, flags);

    flags = ::fcntl(STDIN_FILENO1, F_GETFD, 0);
    flags |= O_NONBLOCK;
    ret = ::fcntl(STDIN_FILENO1, F_SETFL, flags);

    flags = ::fcntl(STDOUT_FILENO1, F_GETFD, 0);
    flags |= O_NONBLOCK;
    ret = ::fcntl(STDOUT_FILENO1, F_SETFL, flags);

    maxfd = max(max(STDOUT_FILENO1, STDIN_FILENO1), sockfd) + 1;
    int stdineof = 0;
    char *tooptr, *toiptr, *froptr, *friptr;
    char to[MAXLINE], fr[MAXLINE];
    memset(to, 0, sizeof to);
    memset(fr, 0, sizeof fr);
    toiptr = tooptr = to;
    friptr = froptr = fr;
    int n, nwritten;
    for ( ; ; )
    {
        FD_ZERO(&rset);
        FD_ZERO(&wset);

        if (stdineof == 0 && toiptr < &to[MAXLINE])
        {
            FD_SET(STDIN_FILENO1, &rset);
        }

        if (tooptr != toiptr)
        {
            FD_SET(sockfd, &wset);
        }

        if (friptr < &fr[MAXLINE])
        {
            FD_SET(sockfd, &rset);
        }

        if (froptr != friptr)
        {
            FD_SET(STDOUT_FILENO1, &wset);
        }

        select(maxfd, &rset, &wset, NULL, NULL);
        if (FD_ISSET(STDIN_FILENO1, &rset))
        {
            if ((n = read(STDIN_FILENO1, toiptr, &to[MAXLINE] - toiptr)) < 0)
            {
                if (errno != EWOULDBLOCK)
                {
                    ERR_EXIT("read error on stdin");
                }
            }
            else if (n == 0)
            {
                fprintf(stderr, "%s: EOF on stdin\n", gf_time());
                stdineof = 1;
                if (tooptr == toiptr)
                {
                    shutdown(sockfd, SHUT_WR);
                }
            }
            else
            {
                fprintf(stderr, "%s: read %d bytes from stdin\n", gf_time(), n);
                toiptr += n;
                FD_SET(sockfd, &wset);
            }
        }

        if (FD_ISSET(sockfd, &rset))
        {
            if ((n = read(sockfd, friptr, &fr[MAXLINE]-friptr)) < 0)
            {
                if (errno != EWOULDBLOCK)
                {
                    ERR_EXIT("read error on socket");
                }
            }
            else if (n == 0)
            {
                fprintf(stderr, "%s: EOF on socket\n", gf_time());
                if (stdineof)
                {
                    return;
                }
                else
                {
                    ERR_EXIT("str_cli: server terminated prematurely");
                }
            }
            else
            {
                fprintf(stderr, "%s: read %d bytes from socket\n", gf_time(), n);
                friptr += n;
                FD_SET(STDOUT_FILENO1, &wset);
            }
        }

        if (FD_ISSET(STDOUT_FILENO1, &wset) && ((n = friptr - froptr) > 0))
        {
            if ((nwritten = write(STDOUT_FILENO1, froptr, n)) < 0)
            {
                if (errno != EWOULDBLOCK)
                {
                    ERR_EXIT("write error on stdout");
                }
            }
            else
            {
                fprintf(stderr, "%s: wrote %d bytes to stdout\n", gf_time(), nwritten);
                froptr += nwritten;
                if (froptr == friptr)
                {
                    froptr = friptr = fr;
                }
            }
        }

        if (FD_ISSET(sockfd, &wset) && ((n = toiptr - tooptr) > 0))
        {
            if ((nwritten = write(sockfd, tooptr, n)) < 0)
            {
                if (errno != EWOULDBLOCK)
                {
                    ERR_EXIT("write error on socket");
                }
            }
            else
            {
                fprintf(stderr, "%s: wrote %d bytes to socket\n", gf_time(), nwritten);
                tooptr += nwritten;
                if (tooptr == toiptr)
                {
                    tooptr = toiptr = to;
                    if (stdineof)
                    {
                        shutdown(sockfd, SHUT_WR);
                    }
                }
            }
        }
    }
}
int main()
{
    fr = fopen("/home/jxq/test/2000.lines","r");
    if(fr==NULL) {
        printf("File not open!/n");
        return 0;
    }
    int sockfd[FD_SETSIZE];
    struct sockaddr_in sockAddr;

    for (int i = 0; i < 1; ++i)
    {
        if ((sockfd[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            ERR_EXIT("socket");
        }

        socklen_t socklen = sizeof sockAddr;
        bzero(&sockAddr, socklen);
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(9981);
        inet_pton(AF_INET, "127.0.0.1",  &sockAddr.sin_addr);

        if (connect(sockfd[i], (struct sockaddr*)&sockAddr, socklen) < 0)
        {
            ERR_EXIT("connect");
        }
    }

    for (int i = 0; i < 1; ++i)
        str_cli(fr, sockfd[i]);
    //close(sockfd);
    return 0;
}