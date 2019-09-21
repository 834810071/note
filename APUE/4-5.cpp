//
// Created by jxq on 19-9-21.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <my_err.h>

int main()
{
    if (open("tempfile", O_RDWR) < 0)
    {
        err_sys("open error");
    }
    
    if (unlink("tempfile") < 0)
    {
        err_sys("unlink error");
    }
    
    printf("file unlink\n");
    sleep(15);
    printf("done\n");
    
    exit(0);
}