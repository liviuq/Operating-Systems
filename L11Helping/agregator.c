
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
#include <limits.h>
#include <dirent.h>

#include "myMacros.h"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("Usage: %s year\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    //check if directory existst and you have perms to read
    FAIL_IF( access(argv[1], F_OK | R_OK), "Directory exists. We have read perms", "Error at directory");

    //change the directory to the dir year
    FAIL_IF( chdir(argv[1]), "Changed the dir to the respective year", "Error at changing the directory");
    
    //creating the result.bin file in the folder year
    // the structure of result.bin is:
    /*

        profit      purchase    sold
        4 bytes     4 bytes     4 bytes
    */
    int result = open("result.bin", O_RDWR | O_CREAT, 0600);

    //Changing the directory back to where it was
    FAIL_IF( chdir(".."), "Changed to main folder", "Error changing to main folder");

    //spawning the workers
    pid_t worker;
    for(int i = 0; i < 12; i++)
    {
        worker = fork();
        if(worker == 0) //in kid
        {
            PRINT_INFO("I am a worker");
            //pass month(i) as a character
            char month;
            snprintf(&month, "%d", i);

            //execute the worker
            execl("worker", "worker", argv[1], month, (char*)NULL);

            //pray that we don t end up here
            PRINT_INFO("Error at executing. rip.");
            return i;
        }
    }
    
    //wait for the workers to finish their jobs
    //we don t care the return as the profit, purchase and sold can exceed the 1byte limit
    for(int i = 0; i < 12; i++)
        wait(NULL);

    //change to directory year to read the result.bin
    FAIL_IF( chdir(argv[1]), "Changed the dir to the respective year", "Error at changing the directory");

    //computing the resources
    int profit = 0, purchase = 0, sold = 0;
    FAIL_IF(read(result, &profit, 4), "Read profit", "Error reading profit");
    FAIL_IF(read(result, &purchase, 4), "Read purchase", "Error reading purchase");
    FAIL_IF(read(result, &sold, 4), "Read sold", "Error reading sold");

    printf("Profit:%d\nPurchase: %d\nSold: %d\n", profit, purchase, sold);

    return 0;
}

