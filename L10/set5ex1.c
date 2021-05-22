#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>

#define FAIL_IF_MSG( EXP , MSG) ( {if( EXP == -1) {printf(MSG);exit(EXIT_FAILURE);}} )

int main(void)
{
    pid_t processL1;
    pid_t processL2;

    printf("Luke(s), I am your father nr. (1,1), my PID is %d, my parent PID is %d\n",
           getpid(), getppid());

    for (int i = 1; i < 3; i++)
    {
        FAIL_IF_MSG((processL1 = fork()), "Error at forking.\n");
        if (processL1 == 0) //the child subproblem
        {
            printf("i m the kid nr. (1, %d), my process id is %d  and my parent is %d\n", i, getpid(),getppid());
            for (int j = 1; j < 4; j++)
            {
                FAIL_IF_MSG((processL2 = fork()), "Error at forking.\n");
                if (processL2 == 0) //the last layer children
                {
                    printf("I'm the kid (%d, %d), my PID is %d, my parent PID is %d\n",
                           i, j, getpid(), getppid());
                    return i;
                }
            }
            return i;
        }
    }

    for(int i = 0; i < 8; i++)
        wait(NULL);
    
    return EXIT_SUCCESS;
}