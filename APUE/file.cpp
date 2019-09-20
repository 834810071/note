//
// Created by jxq on 19-9-20.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <my_err.h>

char buf1[11] = "abcdefghij";
char buf2[11] = "ABCDEFGHIJ";

int main()
{
    int fd;
    if ((fd = creat("file1", O_WRONLY)) < 0)
    {
        err_sys("creat error");
    }

    if (write(fd, buf1, 10) != 10)
    {
        err_sys("write error");
    }

    if (lseek(fd, 40, SEEK_SET) == -1)
    {
        err_sys("lseek error");
    }

    if (write(fd, buf2, 10) != 10)
    {
        err_sys("write error");
    }

    exit(fd);
    return 0;
}