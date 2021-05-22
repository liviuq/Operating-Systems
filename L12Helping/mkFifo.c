#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#include "myMacros.h"

int main(int argc, char** argv)
{
    int p[2];
    FAIL_IF(pipe(p), "Pipe has been opened successfully", "Error at openig the pipe");

    close(p[0]);
    close(p[1]);
    return 0;
}