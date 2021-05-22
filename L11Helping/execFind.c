#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc,  char** argv)
{
    printf("[PID %d] Se va rula comanda find!\n", getpid());

    char* command[] = 
    {
        "find",
        "/home/andrew",
        "-name",
        "*.c",
        "-printf",
        "\"%p : %k KB\n\"",
        (char*)NULL
    };

    pid_t pid1;
    if( (pid1 = fork()) == -1)
    {
        printf("[PID %d] Cannot spawn a process!\n", getpid());
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0)
    {
        //cu execvp
        // if (execvp("find", command) == -1)
        // {
        //     printf("[PID %d] Find couldn t be executed!\n", getpid());
        //     exit(EXIT_FAILURE);
        // }    

        //cu execv
        if( execv("/usr/bin/find", command) == -1)
        {
            printf("[PID %d] Find couldn t be executed!\n", getpid());
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        wait(&pid1);
        printf("[PID %d] Comanda find a fost executata! Return code ul este: %d\n", getpid(), pid1 >> 8);
    }
        
    return 0;
}