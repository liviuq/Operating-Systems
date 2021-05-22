#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>

int main(int argc, char** argv, char** env)
{
    printf("[PID %d] Running %s\n", getpid(), argv[0]);

    printf("[PID %d] Executing the new script\n", getpid());

    int kid = fork();

    if(kid == 0) // kid process, replace it with exec
    {
        execl("hellofucker", "AYOOOOOOOO", "second", "third",(char*)NULL);
        printf("[PID %d] After the execl call\n", getpid()); //never ran
    }
    else
    {
        printf("[PID %d] After the execl of the program\n", getpid());
    }
    
    return 0;
}