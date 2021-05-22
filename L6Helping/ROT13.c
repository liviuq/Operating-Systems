#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#define FAIL_IF_MSG( EXP , MSG) \
 ( {if( EXP == -1) {printf(MSG);exit(EXIT_FAILURE);}} )

int main(int argc, char** argv)
{
    if(argc < 3)
    {
        printf("Usage: %s <input> <output>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fileDescriptorSource, fileDescriptorDestination;
    FAIL_IF_MSG((fileDescriptorSource = open(argv[1], O_RDONLY)), "Source file could not be opened\n");
    FAIL_IF_MSG((fileDescriptorDestination = open(argv[2], O_RDWR | O_CREAT)), "Destination file could not be opened\n");

    //checking if we have access to the files
    struct stat sourceInfo, destinationInfo;
    FAIL_IF_MSG(fstat(fileDescriptorSource, &sourceInfo) ,"Error at source stat-ing\n");
    FAIL_IF_MSG(fstat(fileDescriptorDestination, &destinationInfo) ,"Error at source stat-ing\n");

    FAIL_IF_MSG(access(argv[1], R_OK), "You don t have read acces to the input\n");
    FAIL_IF_MSG(access(argv[2], W_OK), "You don t have write acces to the output\n");

    //mapping the files in memory
    char* source = NULL, *destination = NULL;
    source = mmap(NULL, sourceInfo.st_size, PROT_READ, MAP_SHARED,fileDescriptorSource, 0);
    if(source == MAP_FAILED)
    {
        printf("Source could not be mapped successfully\n");
    }
    FAIL_IF_MSG( ftruncate(fileDescriptorDestination, sourceInfo.st_size), "Truncation failed\n");
    destination = mmap(NULL, sourceInfo.st_size, PROT_WRITE, MAP_SHARED,fileDescriptorDestination, 0);
    if(destination == MAP_FAILED)
    {
        printf("Source could not be mapped successfully\n");
    }    

    //closing the files opened with "open()"
    close(fileDescriptorSource);
    close(fileDescriptorDestination);

    // struct flock myLock;
    // myLock.l_type = F_WRLCK;
    // myLock.l_start = SEEK_CUR;
    // myLock.l_start = 0;
    // myLock.l_len = 1;

    //ROT13 alg
    for(int i = 0; i < sourceInfo.st_size; i++)
    {
        if(source[i] >= 'A' && source[i] <= 'Z')
        {
            if((source[i] + 13) <= 'Z')
            {
                //FAIL_IF_MSG( fcntl() , "Error at locking\n");
                destination[i] = source[i] + 13;
            }
            else
            {
                destination[i] = source[i] - 13;
            }
        }
        else
        {
            if (source[i] >= 'a' && source[i] <= 'z')
            {
                if ((source[i] + 13) <= 'z')
                {
                    destination[i] = source[i] + 13;
                }
                else
                {
                    destination[i] = source[i] - 13;
                }
            }
        }
    }

    //syncing
    FAIL_IF_MSG(msync(destination, sourceInfo.st_size, MS_SYNC) , "Error at syncing\n");

    //deallocating the memory files
    FAIL_IF_MSG(munmap(source, sourceInfo.st_size) , "Error at unmapping the source\n");
    FAIL_IF_MSG(munmap(destination, destinationInfo.st_size) , "Error at unmapping the destination\n");
    source = NULL;
    destination = NULL;

    return 0;
}