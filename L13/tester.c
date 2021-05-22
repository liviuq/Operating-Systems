#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <time.h>
#include <limits.h>
#include <dirent.h>
#include <signal.h>

#include "myMacros.h"

int main(int argc, char** argv)
{
    //seeding the pseudo rand gen
    srandom(time(NULL));

    //creating the file descriptor for the source file
    int source;

    //input safety
    char sourceFile[PATH_MAX];
    if(argc != 2)
    {
        printf("Usage: %s sourceFile\n", argv[0]);
        exit(EXIT_FAILURE);

    }
    else
    {
        strcpy(sourceFile, argv[1]);
        //creating the file
        FAIL_IF(source = open(sourceFile, O_RDWR | O_CREAT | O_TRUNC, 0600), "Successfully opened/created the file", "Error at creating/opening the file");

        //checking the file s permissions
        FAIL_IF(access(sourceFile, F_OK | R_OK | W_OK), "Successfully checked the permissions", "Error at accesing the file");
    }

    //creating the 2 workers
    pid_t workers[2];
    FAIL_IF(workers[0] = fork(), "Created the first worker", "Error at creating the first worker");
    if (workers[0] != 0) //we are still in the aprent here
    {
        //create the second process
        FAIL_IF(workers[1] = fork(), "Created the second worker", "Error at creating the second worker");
        if (workers[1] != 0) //parent
        {
            //writing to the file
            int nrLines = rand() % 30;
            for (int i = 0; i < nrLines; i++)
            {
                char buffer[10];
                for (int j = 0; j < 9; j++)
                    buffer[j] = rand() % 93 + 33; //from ! to ~(every printable character)
                buffer[9] = '\n';

                if (i == nrLines - 1)
                {
                    buffer[9] = '\0';
                }
                write(source, buffer, 10); //CHECK HERE IF ERRORS
            }

            //close the file
            FAIL_IF(close(source), "Successfully closed the source file", "Error at closing the source file");

            //signal workers that you finished
            kill(workers[0], 12);
            kill(workers[1], 10);
            
            //wait for the workers to finish
            wait(&workers[0]);
            wait(&workers[1]);

            //returning
            return 0;
        }
        else //workers[1]
        {
            //pause on SIGUSR1
            sigset_t worker1Signal;
            sigfillset(&worker1Signal);
            sigdelset(&worker1Signal, SIGUSR1);
            sigprocmask(SIG_BLOCK, &worker1Signal, NULL);
            sigsuspend(&worker1Signal);
            
            printf("doing the sorting\n");
            //memory mapping the file
            struct stat myStat;
            FAIL_IF(stat(sourceFile, &myStat), "Successfully stated the file", "Error at statting");
            char *sourceMap;
            sourceMap = mmap(NULL,
                             myStat.st_size,
                             PROT_READ | PROT_WRITE,
                             MAP_SHARED,
                             source,
                             0);
            if (sourceMap == MAP_FAILED)
            {
                PRINT_INFO("Error at mapping the file");
                exit(EXIT_FAILURE);
            }

            //sort the file in an increasing order
            char firstSentence[PATH_MAX], secondSentence[PATH_MAX];
            (void)firstSentence;
            (void)secondSentence;
            for(int i = 0; i < myStat.st_size; i++)
            {
               //to be implemented 
            }

            //wait for first worker to print the number of lines
            sigfillset(&worker1Signal);
            sigdelset(&worker1Signal, SIGUSR2);
            sigsuspend(&worker1Signal);

            //printt the sorted file
            printf("[PID %d] The sorted file is:\n", getpid());
            return 0;
        }
    }
    else //workers[0]
    {
        //pause on SIGUSR2
        sigset_t worker0Signal;
        sigfillset(&worker0Signal);
        sigdelset(&worker0Signal, SIGUSR2);
        sigprocmask(SIG_BLOCK, &worker0Signal, NULL);
        sigsuspend(&worker0Signal);

        printf("doing the number of lines\n");

        //count the number of \n in the file
        int counter = 0;
        char character;
        while (read(source, &character, sizeof(character)) != EOF)
        {
            if (character == '\n')
                counter++;
        }
        //print the numvber of lines
        printf("[PID: %d] The number of lines is: %d\n", getpid(), counter);

        //signal the second worker that you finished
        //printing your number
        kill(workers[1], SIGUSR2);

        //terminating the worker
        return 0;
    }
    return 0;
}