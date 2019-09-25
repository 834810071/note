//
// Created by jxq on 19-9-23.
//


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <my_err.h>
#include <iostream>

int glob = 6;
char buf[] = "a write to stdout\n";

int main()
{
    int var;
    var = 88;
    if (write(STDIN_FILENO, buf, sizeof(buf)-1) != sizeof buf -1)
    {
        err_sys("write error");
    }

    printf("before fork\n");
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        err_sys("fork");
    }
    else if (pid == 0)
    {
        var++;
        glob++;
    }
    else
    {
        sleep(2);
    }

    printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);

    return 0;
}