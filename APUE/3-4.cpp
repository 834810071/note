//
// Created by jxq on 19-9-20.
//

#include <sys/types.h>
#include <fcntl.h>
#include <cstdlib>
#include <my_err.h>

int main(int argc, char** argv)
{
    int accmode, val;
    if (argc != 2)
    {
        err_sys("usage: a.out <descriptor#>");
    }
    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0)
    {
        err_sys("fcntl");
    }

    accmode = val & O_ACCMODE;

    if (accmode == O_RDWR) printf("read write");
    else if (accmode == O_WRONLY) printf("write only");
    else if (accmode == O_RDONLY) printf("read only");
    else err_dump("unknown access mode");

    if (val & O_APPEND) printf(", append");
    if (val & O_NONBLOCK) printf(", nonblocking");
# if !defined(_POSIX_SOURCE) && defined(O_SYNC)
    if (val & O_SYNC) printf(", synchronous writes");
# endif
    putchar('\n');

    exit(0);
}