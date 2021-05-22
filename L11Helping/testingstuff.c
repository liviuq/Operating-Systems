#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <limits.h>
#include <dirent.h>

#include "myMacros.h"

int main()
{
    int file = open("file.txt", O_RDWR | O_CREAT | O_TRUNC);

    int a = 4, b = 20, c = 100;
    write(file, &a, 4);
    write(file, &b, 4);
    write(file, &c, 4);

    lseek(file, SEEK_SET, 0);

    int d,e,f;
    read(file, &d, 4);
    read(file, &e, 4);
    read(file, &f, 4);

    printf("%d %d %d\n", d, e ,f);
    return 0;
}