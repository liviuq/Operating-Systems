#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    pid_t currentProcess = getpid();
    pid_t parentProcess = getppid();

    pid_t realOwner = getuid();
    pid_t realGroup = getgid();

    pid_t effectiveOwner = geteuid();
    pid_t effectiveGroup = getegid();

    printf("currentProcess : %d, parentProcess : %d\nrealOwner : %d, realGroup : %d\neffectiveOwner : %d, effectiveGroup : %d\n", 
            currentProcess, parentProcess,
            realOwner, realGroup,
            effectiveOwner, effectiveGroup);

    return EXIT_SUCCESS;
}