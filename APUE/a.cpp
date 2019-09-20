//
// Created by jxq on 19-9-20.
//

#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>
#include <cstring>
#include <zconf.h>
#include <my_err.h>
#include <signal.h>

//#define MAXLINE 1024

void sig_int(int signo)
{
    printf("interrupt\n%% ");
}

int main(int argc, char** argv)
{
    char buf[MAXLINE];
    pid_t pid;
    int status;

    if (signal(SIGINT, sig_int) == SIG_ERR)
    {
        err_sys("signal err");
    }

    printf("%% ");
    while (fgets(buf, MAXLINE, stdin) != NULL)
    {
        buf[strlen(buf) - 1]  = 0;
        if ((pid = fork()) < 0)
        {
            err_sys("fork error");
        }

        else if (pid == 0)
        {
            execlp(buf, buf, (char*)0);
            err_ret("could't execute: %s", buf);
            exit(127);
        }

        if ((pid = waitpid(pid, &status, 0)) < 0)
        {
            err_sys("waitpid err");
        }
        printf("%% ");
    }

    return 0;
}