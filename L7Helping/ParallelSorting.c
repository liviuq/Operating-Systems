#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define FAIL_IF_MSG( EXP , MSG) ( {if( EXP == -1) {printf(MSG);exit(EXIT_FAILURE);}} )

void bubbleSort(int fd);

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("Usage: %s <source_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sourceFileDescriptor;
    FAIL_IF_MSG( (sourceFileDescriptor = open(argv[1], O_RDWR | O_CREAT, 0644) ) , "Source file couldn't be opened.\n");

    bubbleSort(sourceFileDescriptor);

    FAIL_IF_MSG(close(sourceFileDescriptor), "Error at closing the file descriptor!\n");
    return 0;
}

void bubbleSort(int fd)
{
    int firstNum, secondNum;
    int read1,    read2;
    int modif = 1;

    struct flock lock, unlock;

    lock.l_type     = F_WRLCK;
    lock.l_len      = 2 * sizeof(firstNum);
    lock.l_whence   = SEEK_CUR;
    lock.l_start    = 0;

    unlock.l_type   = F_UNLCK;
    unlock.l_len    = 2 * sizeof(firstNum);
    unlock.l_whence = SEEK_CUR;
    unlock.l_start  = -2 * sizeof(firstNum);

    while (modif)
    {
        modif = 0; //it will be set if we make a swap

        while (1)
        {
            FAIL_IF_MSG( (read1 = read( fd, &firstNum, sizeof(firstNum))),"Error at reading the number!\n");
            if(read1 == 0)
                break; //EOF
            
            FAIL_IF_MSG( (read2 = read( fd, &secondNum, sizeof(secondNum))),"Error at reading the number!\n");
            if(read2 == 0)
                break;

            if(firstNum > secondNum)
            {
                modif = 1;
                FAIL_IF_MSG(lseek(fd, -2*sizeof(firstNum), SEEK_CUR) ,"Error at seeking 2*sizeof(num) behind!\n");

                printf("[PID: %d] trying to set the lock!\n", getpid());
                int errnumber = fcntl(fd, F_SETLK, &lock);
                if(errnumber == -1)
                {
                    printf("[PID: %d] Failed to set the lock! Error: %s\n", getpid(), strerror(errno));
                    //exit(EXIT_FAILURE);
                }
                printf("[PID: %d] Succes on setting the lock!\n", getpid());

                FAIL_IF_MSG(write(fd, &secondNum, sizeof(secondNum)) ,"Error at writing the first num!\n");
                FAIL_IF_MSG(write(fd, &firstNum, sizeof(firstNum)),"Error at writing the second number!\n");

                //FAIL_IF_MSG( fcntl(fd, F_SETLK, &unlock),"Error at unlocking the C.S!\n");
                //i had some issues with this macro, i couldn t print the error message.

                errnumber = fcntl(fd, F_SETLK, &unlock);
                if(errnumber == -1)
                {
                    printf("[PID: %d] Failed to set the unlock lock! Error: %s\n", getpid(), strerror(errno));
                    //exit(EXIT_FAILURE);
                }

                printf("[PID: %d] Succes on setting the UNlock!\n", getpid());
            }

            FAIL_IF_MSG(lseek(fd, -sizeof(firstNum), SEEK_CUR), "Error at seeking behind for the next loop!\n");
        }
        
        FAIL_IF_MSG(lseek(fd, 0L, SEEK_SET),"Error at repositioning at the beginning of the file!\n");   
    }
}