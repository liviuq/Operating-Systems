#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "myMacros.h"

int main(int argc, char** argv)
{
    int source = open(argv[1], O_RDWR | O_CREAT, 0644);

    int one = 1;
    write(source, &one, 4);

    close(source);
    return 0;
}