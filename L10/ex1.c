
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

#define FAIL_IF_MSG( EXP , MSG) ( {if( EXP == -1) {printf(MSG);exit(EXIT_FAILURE);}} )
#define FAIL_IF_MSG_LOG( EXP , MSG, EXP2) ( {if( EXP == -1) {printf(MSG, EXP2);exit(EXIT_FAILURE);}} )

int base2Length(int number);

int main(int argc, char** argv)
{
    pid_t firstChild, secondChild;

    int lockFileDescriptor;
    struct flock myLock;

    FAIL_IF_MSG_LOG((lockFileDescriptor = open("locky.txt", O_RDWR | O_TRUNC)), "[MASTER: %d] Failed to open the binary file!\n", getpid() );
    
    //lseek(lockFileDescriptor, -4, SEEK_CUR); AFTER WRITE, SEEK IT 4 BYTES BEFORE!
    int nrBytesIn = 0, nrBytesOut = 0;
    int buffer;
    ftruncate(lockFileDescriptor, 4);
    while( (nrBytesIn = read("in.txt", &buffer, 4)) > 0 )
    {
        nrBytesOut = write(lockFileDescriptor, &buffer, 4);
        
        printf("read: %d , write: %d", nrBytesIn, nrBytesOut);
        // if (nrBytesOut != nrBytesIn)
        // {
        //     printf("[MASTER: %d] Writing error!\n", getpid());
        //     exit(EXIT_FAILURE);
        // }
        lseek(lockFileDescriptor, -4, SEEK_CUR);
    }
    printf("CTRLD WOIRDS");
    myLock.l_type = F_WRLCK;
    myLock.l_whence = SEEK_CUR;
    myLock.l_start = 0;
    myLock.l_len = 4;

    FAIL_IF_MSG_LOG(close(lockFileDescriptor), "[MASTER: %d] Failed to close the file!\n", getpid());
    return 0;
}

int base2Length(int number)
{
    double result = log2(number);
    return( result == floor(result)? (int)result : (int)result + 1 );
}