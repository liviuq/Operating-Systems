
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
    //checking the usage
    if(argc != 3)
    {
        printf("Usage: %s yearDir monthdir\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //composing the directory
    char workingDir[PATH_MAX];
    sprintf(workingDir, "%s/%s", argv[1], argv[2]);

    //checking if we have permission to access the dir
    FAIL_IF( access(workingDir, F_OK | R_OK), "Directory exists. We have read perms", "Error at directory");

    //Changing the cwd to workingDir
    FAIL_IF( chdir(workingDir), "Changed cwd to workingDir", "Error at changinf the working dir");

    //open the current directory
    DIR* directory;
    directory = opendir(".");
    if(directory == NULL)
    {
        PRINT_INFO("Error at DIR*");
        exit(EXIT_FAILURE);
    }

    //preparing the profit, purchase, sold
    int purchase = 0, profit = 0, sold = 0;

    //reading the files in directory
    struct dirent *directoryStruct;
    while( (directoryStruct = readdir(directory)) != NULL)
    {
        //opening the 
        int currentFile;
        currentFile = open(directoryStruct->d_name, O_RDONLY);
        if(currentFile == -1)
        {
            printf("Error at opennig the day\n");
            exit(EXIT_FAILURE);
        }

        char purchase_ch[4];
        char profit_ch[4];
        char sold_ch[4];

        read(currentFile, purchase_ch, 4);
        purchase = atoi(purchase_ch);

        lseek(currentFile, SEEK_CUR, 1); //jump over the \n

        read(currentFile, profit_ch, 4);
        profit = atoi(profit_ch);

        lseek(currentFile, SEEK_CUR, 1); //jump over the \n

        read(currentFile, sold_ch, 4);
        sold = atoi(sold_ch);

        //finished readingg the values, now we have to close the file
        close(currentFile);

        //changing the cwd to the year dir to store the numbers
        FAIL_IF(chdir(".."), "Changed back the dir", "Error at changing the dir");

        //open result.bin
        int result;
        FAIL_IF( result = open("result.bin", O_RDWR), "updating the result file", "Error updating the resultt file");

        //prepare the locks
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 14;

        struct flock unlock;
        unlock.l_type = F_UNLCK;
        unlock.l_whence = SEEK_CUR;
        unlock.l_start = -14;
        unlock.l_len = 14;

        //locking the file
        FAIL_IF( fcntl(result, F_SETLK, &lock), "Locked the file", "Error locking the file");

        //update the file
        int temp1, temp2, temp3;
        read(result, &temp1, 4);
        read(result, &temp2, 4);
        read(result, &temp3, 4);
        purchase += temp1;
        profit += temp2;
        sold += temp3;

        //reposition to write the new values
        lseek(result, SEEK_CUR, -14);

        //write the new values
        write(result, &purchase, 4);
        write(result, &profit, 4);
        write(result, &sold, 4);

        //unlock the file
        FAIL_IF( fcntl(result, F_SETLK, &unlock), "UNLocked the file", "Error unlocking the file");

        //close the filedescriptor
        close(result);
    }
    return 0;
}