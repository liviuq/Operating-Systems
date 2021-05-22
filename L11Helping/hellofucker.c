#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv, char** env)
{
    printf("[PID: %d] I am the evil exec!\n", getpid());
    printf("[PID: %d] The list of argvs is the following:\n", getpid());

    for(u_int32_t i = 0; i < argc; i++)
    {
        printf("[PID %d] The %d'th arg is: %s\n", getpid(), i, argv[i]);
    }
    printf("[PID: %d] Printing the list ov env variables:\n", getpid());

    for(u_int32_t i = 0; env[i] != NULL; i++)
    {
        printf("[PID: %d] The %d'th env var is: %s\n", getpid(), i, env[i]);
    }

    printf("[PID: %d] Ending of the evil exec!\n", getpid());
    return 0;
}