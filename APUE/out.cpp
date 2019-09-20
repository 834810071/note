//
// Created by jxq on 19-9-20.
//

#include <stdio.h>
#include <zconf.h>
#include <my_err.h>

#define BUFFSIZE 8192

int main()
{
    int n;
    char buf[BUFFSIZE];
    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
    {
        if (write(STDOUT_FILENO, buf, n) != n)
        {
            err_sys("write error");
        }
    }
    if (n < 0)
    {
        err_sys("read error");
    }
    exit(0);
}