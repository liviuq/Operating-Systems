#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "myMacros.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <source_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sourceFD;
    FAIL_IF( (sourceFD = open(argv[1], O_RDWR | O_CREAT, 0644 )) , "File opened successfully", "Error at open()" );

    struct stat myStat;
    FAIL_IF( (stat(argv[1], &myStat)), "Successfully statted the source file", "Error at stat()");
    FAIL_IF( access(argv[1], F_OK | R_OK | W_OK), "All permissions are met", "Error at access()");

    char* myMap = NULL;
    //FAIL_IF( (myMap = mmap(NULL, myStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, sourceFD, 0)), "Successfully mapped the file", "Error at mmap()");
    myMap = mmap(NULL, myStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, sourceFD, 0);
    if(myMap == MAP_FAILED)
        printf("[PID: %d] Error at mmap()\n", getpid());
    else
        printf("[PID: %d] Successfully mapped the source file!\n", getpid());

    FAIL_IF(close(sourceFD), "Successfully closed the source file", "Error at close()");


    PRINT_INFO("Creating the first child to check the sharing of the mmap");
    pid_t child;
    child = fork();

    if(child == 0)
    {
        PRINT_INFO("We are in the child process");
        PRINT_INFO("Reading the memory");

        for(int i = 0; i <myStat.st_size; i++)
        {
            printf("%c ", myMap[i]);
        }
        myMap[0] = 'X';
        return 0;
    }
    else
    {
        PRINT_INFO("Waiting for the kid");
        int retVal;
        FAIL_IF( (child = wait(&retVal)), "Waiting for the kiddo", "Error at wait()");
        PRINT_INFO("Kid finished");
        for(int i = 0; i <myStat.st_size; i++)
        {
            printf("%c ", myMap[i]);
        }

    }

    FAIL_IF(munmap(myMap, myStat.st_size), "Successfully unmapped the mapping", "Error at munmap()");
    return 0;
}