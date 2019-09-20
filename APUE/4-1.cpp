//
// Created by jxq on 19-9-20.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <string.h>
#include <my_err.h>

int main(int argc, char** argv)
{
    int i;
    std::string ptr;
    struct stat buf;
    for (i = 1; i < argc; ++i)
    {
        printf("%s: ", argv[i]);
        if (lstat(argv[i], &buf) < 0)
        {
            err_ret("lstat");
            continue;
        }

        if (S_ISLNK(buf.st_mode)) ptr = "symbolic link";
        else ptr = "** unknow mode **";
        printf("%s\n", ptr.c_str());
    }
    exit(0);
}