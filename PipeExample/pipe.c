#define _DEFAULT_SOURCE
#define _GNU_SOURCE 

#ifndef MYMACROS_H_
#define MYMACROS_H_

#define PRINT_INFO(MSG) \
        ( { printf("[PID: %d] %s\n", getpid(), MSG ); } )

#define FAIL_IF(EXP, MSG_OK, MSG_FAIL) \
        ( { if((EXP) == -1) { printf("[PID: %d] %s. Reason: %s\n", getpid(), MSG_FAIL, strerror(errno)); exit(EXIT_FAILURE); }\
            else{ printf("[PID: %d] %s\n", getpid(), MSG_OK);} } )

#endif


#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    //creating the 3 pid_t for supervisor, worker1, worker2
    pid_t w1, w2;

    //creating the pipes
    int w1pipe[2], w2pipe[2];
    /*
        w1pipe[0] -> reading from outside into w1
        w1pipe[1] -> writing to the pipe

        same for w2pipe
    */
    FAIL_IF( pipe(w1pipe), "Created worker1 pipe", "Error at creating the worker1 pipe");
    FAIL_IF( pipe(w2pipe), "Created worker2 pipe", "Error at creating worker2 pipe");

    //cerating the workers
    FAIL_IF(w1 = fork(), "Created the first worker", "Error at creating the first worker");
    if(w1 == 0) //we are in the first worker
    {
        //closing the unnecessary pipes
        close(w2pipe[0]);
        close(w2pipe[1]);

        int sum;        
        int numberRead;
        while( read(w1pipe[0], &numberRead, sizeof(numberRead)) != 0 )
        {
            printf("W1: %d\n", numberRead);
            sum += numberRead;
        }

        close(w1pipe[0]);
        FAIL_IF( write(w1pipe[1], &sum, sizeof(sum)), "Sent the partial sum to the supervisor. I am worker1", "Error at sending the partial sum from worker1");
        close(w1pipe[1]);

        return 0; //end the worker
    }
    else //we are in the supervisor, we ll spawn the second worker
    {
        FAIL_IF( w2 = fork(), "Created the second worker", "Error at creating the second worker");
        if(w2 == 0) //we are in the second worker
        {
            close(w1pipe[0]);
            close(w1pipe[1]);

            int sum;
            int numberRead;
            while( read(w2pipe[0], &numberRead, sizeof(numberRead)) != 0)
            {
                printf("W2 : %d\n", numberRead);
                sum += numberRead;
            }

            close(w2pipe[0]);
            FAIL_IF( write(w2pipe[1], &sum, sizeof(sum)), "Sent the partial sum to the supervisor. I am worker2", "Error at sending the partial sum from worker2");
            close(w2pipe[1]);

            return 0;
        }
        else //we are in the supervisor
        {
            int number;
            int flag = 1;
            while(1)
            {
                scanf("%d", &number);
                printf("Read from keyboard: %d\n", number);
                if(number == 0) break;

                write( (flag == 1? w1pipe[1] : w2pipe[1]), &number, sizeof(number) );
                flag = 3 - flag;
            }

            close(w1pipe[1]);
            close(w2pipe[1]);

            int partialSum1, partialSum2;
            FAIL_IF( read(w1pipe[0], &partialSum1, sizeof(partialSum1)), "Read the partial sum1", "Error reading the partial sum1");
            FAIL_IF( read(w2pipe[0], &partialSum2, sizeof(partialSum2)), "Read the partial sum2", "Error reading the partial sum2");

            close(w1pipe[0]);
            close(w2pipe[0]);

            printf("the sum is %d", partialSum2 + partialSum1);

            return 0;
        }
    }

    return 0;
}