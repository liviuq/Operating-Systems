#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

//#include "myMacros.h" //am doar macrourile astea doua in "myMacros" si le am copiat aici anyway

#define PRINT_INFO(MSG) \
        ( { printf("[PID: %d] %s\n", getpid(), MSG ); } )

#define FAIL_IF(EXP, MSG_OK, MSG_FAIL) \
        ( { if((EXP) == -1) { printf("[PID: %d] %s. Reason: %s\n", getpid(), MSG_FAIL, strerror(errno)); exit(EXIT_FAILURE); }\
            else{ printf("[PID: %d] %s\n", getpid(), MSG_OK);} } )

int main(void)
{
    int pipe3to4[2];
    pid_t pid1, pid2, pid3;
    FAIL_IF( pipe(pipe3to4), "Created pipe 3-4", "Error at creating pipe 1-2");

    //ramnificarea:
    FAIL_IF( pid1 = fork(), "Created first process", "Error at creating the first process");

    if(pid1 == 0)
    {
        int pipe2to3[2];
        FAIL_IF( pipe(pipe2to3), "Created pipe 2-3", "Error at creating pipe 2-3");

        FAIL_IF(pid2 = fork(), "Created second process", "Error at creating the second process");

        //ramnificare2
        if(pid2 == 0)
        {
            int pipe1to2[2];
            FAIL_IF( pipe(pipe1to2), "Created pipe 1-2", "Error at creating pipe 1-2");

            FAIL_IF(pid3 = fork(), "Created third process", "Error at creating the third process");

            //ramnificare3
            if(pid3 == 0)
            {
                //FAIL_IF( dup2(pipe1to2[1], 1), "Redirected stdout to pipe1to2[1]", "Error redirecting stdout to pipe1to2[1]");
                dup2(pipe1to2[1], 1);
                //executing grep
                //FAIL_IF(execlp("grep", "grep", "sudo:", "/etc/group", (char*)NULL), "Executed grep", "Error at executing grep");
                execlp("grep", "grep", "sudo:", "/etc/group", (char*)NULL);

            }
            else
            {
                FAIL_IF( close(pipe1to2[1]), "Closed pipe1to2[1]", "Error at closing pipe1to2[1]");

                //FAIL_IF( dup2(pipe1to2[0], 0), "Redirected stdin to pipe1to2[0]", "Error at redirecting stdin to pipe1to2[0]");
                //FAIL_IF( dup2(pipe2to3[1], 1), "Redirected stdout to pipe2to3[1]", "Error at redirecting stdout to pipe2to3[1]");
                dup2(pipe1to2[0], 0);
                dup2(pipe2to3[1], 1);
                //exec cut
                //FAIL_IF( execlp("cut", "cut", "-f4", "-d:", (char*)NULL), "Executed cut", "Error at executing cut");
                execlp("cut", "cut", "-f4", "-d:", (char*)NULL);
            }
        }
        else
        {
            FAIL_IF( close(pipe2to3[1]), "Closed pipe2to3[1]", "Error at closing pipe2to3[1]");

            //FAIL_IF( dup2(pipe2to3[0], 0), "Redirected stdin to pipe2to3[0]", "Error at redirecting stdin to pipe2to3[0]");
            //FAIL_IF( dup2(pipe3to4[1], 1), "Redirected stdout to pipe3to4[1]", "Error at redirecting stdout to pipe3to4[1]");
            dup2(pipe2to3[0], 0);
            dup2(pipe3to4[1], 1);
            //exec tr
            //FAIL_IF( execlp("tr", "tr", "\",\"", "\"\n\"", (char*)NULL), "Executed tr", "Error at executing tr");
            execlp("tr", "tr", "\",\"", "\"\n\"", (char*)NULL);
        }
    }
    else
    {
        FAIL_IF( close(pipe3to4[1]), "Closed pipe3to4[1]", "Error at closing pipe3to4[1]");

        //FAIL_IF( dup2(pipe3to4[0], 0), "Redirected stdin to pipe2to3[0]", "Error at redirecting stdin to pipe2to3[0]");
        dup2(pipe3to4[0], 0);
        //FAIL_IF( execlp("wc", "wc", "-l", (char*)NULL), "Executed wc", "Error at executing wc");
        execlp("wc", "wc", "-l", (char*)NULL);
    }

    return 0;
}