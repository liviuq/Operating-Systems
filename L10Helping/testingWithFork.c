#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include "myMacros.h" //FAIL_IF, PRINT_INFO

int main(int argc, char** argv)
{
    PRINT_INFO("Heyo. Starting the fork() stuff");
    PRINT_INFO("starting the fork()");

    pid_t kid;
    FAIL_IF((kid = fork()), "Successfully created a kiddo", "Error at fork()" );

    if(kid == 0)
    {
        PRINT_INFO("We are in the kiddo process.");
        sleep(1);
        PRINT_INFO("What shall we do here?");

        for(int i = 0; i < 3; i++)
        {
            printf("%d\n", i);
            sleep(1);
        }
        PRINT_INFO("SIKE IM LOOPIN!");
        while(1);
        PRINT_INFO("I'm done.");
        return 6;
    }
    else
    {   int cod_return;
        FAIL_IF( wait(&cod_return), "Successfully waiting for the kiddo.","Error at wait()" );
        if(WIFEXITED(cod_return))
            printf("[PID: %d] Kid ended with code: %d\n", getpid(), WEXITSTATUS(cod_return));

        if(WIFSIGNALED(cod_return))
        {
            printf("[PID: %d] Kid was signaled! Signal: %d\n", getpid(), WTERMSIG(cod_return));
            if(WCOREDUMP(cod_return))
            {
                PRINT_INFO("KId got core dumped.");
                PRINT_INFO("Handling the other stuff now!");
            }

            if(WIFSTOPPED(cod_return))
            {
                printf("[PID: %d] Kid was signaled: %d", getpid(), WSTOPSIG(cod_return));
            }
                
        }

        PRINT_INFO("We're in the parent process!");
        for(int i = 3; i < 7; i++)
        {
            printf("%d\n", i);
            sleep(1);
        }
        PRINT_INFO("I'm done too kiddo.");
    }

    PRINT_INFO("Common part. Happy now?");

    return 0;
}