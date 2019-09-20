//
// Created by jxq on 19-9-19.
//

#include <sys/types.h>
#include <dirent.h>
#include <cstdlib>
#include <apue.h>
#include <my_err.h>

int main(int argc, char** argv)
{
    DIR *dp;
    struct dirent *dirp;

    if (argc != 2)
    {
        err_quit("a single argument is required");
    }

    if ((dp = opendir(argv[1])) == NULL)
    {
        err_sys("can't open %s", argv[1]);
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        printf("%s\n", dirp->d_name);
    }

    closedir(dp);
    exit(0);
}