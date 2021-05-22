#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define PRINT_INFO(MSG) \
        ( { printf("[PID: %d] %s\n", getpid(), MSG ); } )

#define PRINT_KID_INFO(MSG) \
        ( { printf("[PID: %d] %s. My father is %d\n", getpid(), MSG, getppid()); } )

#define FAIL_IF(EXP, MSG_OK, MSG_FAIL) \
        ( { if((EXP) == -1) { printf("[PID: %d] %s. Reason: %s\n", getpid(), MSG_FAIL, strerror(errno)); exit(EXIT_FAILURE); }\
            else{ printf("[PID: %d] %s\n", getpid(), MSG_OK);} } )

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("Usage: %s <number_of_children>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int nrOfChildren = atoi(argv[1]); 
    printf("[PID: %d] Spawning %d children.\n", getpid(), nrOfChildren);

    pid_t tenChilds[nrOfChildren];
    for(int i = 0; i < nrOfChildren; i++)
    {
        FAIL_IF( (tenChilds[i] = fork()), "Spawned a child", "Error at fork()");
        if(tenChilds[i] == 0)
        {
            PRINT_KID_INFO("I'm a child");
            return i;
        }
        else //im in the parent
        {
            int returnCode;
            FAIL_IF( (tenChilds[i] = wait(&returnCode)), "Successfully waiting", "Error at wait()");

            if(WIFEXITED(returnCode))
                printf("[PID: %d] Kid %d terminated\n", getpid(), WEXITSTATUS(returnCode));
        }
    }

    return 0;
}