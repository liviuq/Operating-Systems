#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#define FAIL_IF_MSG( EXP , MSG) ( {if( EXP == -1) {printf(MSG);exit(EXIT_FAILURE);}} )

int main(int argc, char** argv)
{
    pid_t childID;
    FAIL_IF_MSG(childID = fork(), " Fork has failed");
    if(childID == 0)
    {
        printf("[PID: %d] I'm the child!\n", getpid());    
        //execl("/home/andrew/Documents/SO/L10/hello", NULL);
        printf("[PID: %d] After the exec!\n", getpid());    
    }
    else
    {
        //waitpid(childID, , 0);
        //wait(NULL);
        printf("[PID: %d] I'm the parent!\n", getpid());        
    }

    printf("Sectiune comuna!\n");
    return 0;
}