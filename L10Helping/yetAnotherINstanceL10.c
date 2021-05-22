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
#include <math.h>

#include "myMacros.h"

int base2Length(int number);

int main(int argc, char** argv)
{
    PRINT_INFO("Starting the program. I am the father");
    PRINT_INFO("Creating the 2 workers that we need");

    pid_t worker1;
    pid_t worker2;

    int* map1;
    map1 = mmap(NULL, 2*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1,  0);
    if(map1 != MAP_FAILED)
        printf("[PID: %d] Successfully created the first anon mmap", getpid());
    else
    {
        printf("[PID: %d] Error at the first anon mmap. Error: %s\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }
    PRINT_INFO("Created the first mmap");

    int* map2;
    map2 = mmap(NULL, 2*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); 
    if(map2 != MAP_FAILED)
        printf("[PID: %d] Successfully created the second anon mmap", getpid());
    else
    {
        printf("[PID: %d] Error at the second anon mmap. Error: %s\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }
    PRINT_INFO("Created the second mmap");

    FAIL_IF( (worker1 = fork() ), "Successfully forked the first worker", "Error at forking the first worker");
    if(worker1 != 0) //Parent
    {
        FAIL_IF( (worker2 = fork() ), "Successfully forked the second worker", "Error at forking the second worker");
        if(worker2 != 0) //Main parent stuff
        {
            int odd = 1;
            int numberToBeSent;
            char num[5];
            ssize_t bytesRead;
            //while( ((bytesRead = read(STDIN_FILENO, &numberToBeSent, sizeof(numberToBeSent))) != 0 ) || bytesRead != -1)
            while(  (bytesRead = read(STDIN_FILENO, &num, 4)))
            {
                num[4] = 0;
                numberToBeSent = atoi(num);
                printf("[PID: %d] Read %ld bytes\n", getpid(), bytesRead);
                if(odd == 1)
                {
                    //send to worker1
                    printf("[PID: %d] Sending %d\n", getpid(), numberToBeSent);
                    map1[0] = numberToBeSent;
                    odd = 0;
                }
                else
                {
                    //send to worker2
                    printf("[PID: %d] Sending %d\n", getpid(), numberToBeSent);
                    map2[0] = numberToBeSent;
                    odd = 1;
                }
            }

            //Signal the workers to write in the maps the numbers
            kill(worker1, 9);
            kill(worker2, 9);

            if(map1[1] > map2[1])
                printf("[PID: %d] Maximum value is %d\n", getpid(), map1[1]);
            else
                printf("[PID: %d] Maximum value is %d\n", getpid(), map2[1]);
            PRINT_INFO("Done");
        }

        else //Worker2
        {
            int sum = 1;
            while (1)
            {
                do
                {
                    sleep(1);
                    PRINT_INFO("Waiting for a number");
                } while (map2[0] == 0);
                PRINT_INFO("Got a number!");
                printf("[PID: %d] Number is %d\n", getpid(),map2[0]);
                sum += base2Length(map2[0]);
                map2[1] = sum;
                map2[0] = 0;
            }
        }
    }
    else //worker 1
    {
        int sum = 1;
        while (1)
        {
            do
            {
                sleep(1);
                PRINT_INFO("Waiting for a number");
            } while (map1[0] == 0);

            PRINT_INFO("Got a number!");
            printf("[PID: %d] Number is %d\n", getpid(), map1[0]);
            sum += base2Length(map1[0]);
            map1[1] = sum;
            map1[0] = 0;
        }
    }

    FAIL_IF( munmap(map1, 2*sizeof(int)), "Successfully unmapped the first anon mmap", "Error at unmapping the first anon mmap");
    FAIL_IF( munmap(map2, 2*sizeof(int)), "Successfully unmapped the second anon mmap", "Error at unmapping the second anon mmap");
    return 0;
}

int base2Length(int number)
{
    double result = log2(number);
    return( result == floor(result)? (int)result : (int)result + 1 );
}