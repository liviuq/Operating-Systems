#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define FAIL_IF_MSG( EXP , MSG) ( {if( EXP == -1) {printf(MSG);exit(EXIT_FAILURE);}} )

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("Usage: %s <source_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sourceFileDescriptor;
    FAIL_IF_MSG( ( sourceFileDescriptor = open(argv[1], O_RDWR | O_CREAT, 0644) ),"Error at opening the file!\n");

    struct stat statBuffer;
    FAIL_IF_MSG(fstat(sourceFileDescriptor, &statBuffer),"Error at stat-ing the file!\n");
    FAIL_IF_MSG(access(argv[1], F_OK | R_OK | W_OK),"Error at verifying the permissions!\n");

    long pageSize;
    FAIL_IF_MSG( (pageSize = sysconf(_SC_PAGE_SIZE) ),"Error at getting the page size!\n");

    //If we had an offset, we could allign the offset like this:
    //pa_offset = offset & ~(pageSize - 1)

    char* myMap = NULL;
    myMap = mmap(NULL,
                statBuffer.st_size,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                sourceFileDescriptor,
                0);
    if(myMap == MAP_FAILED)
    {
        printf("Error at mapping: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("Map succecssfully created!\n");
    printf("Closing the fileDescriptor!\n");
    FAIL_IF_MSG(close(sourceFileDescriptor),"Error at closing the file descriptor!\n");

    printf("Let s do some processing on the mmap!\n");

    srand( time(NULL) );
    int reverseArray[statBuffer.st_size];
    for(int i = 0; i < statBuffer.st_size; i++)
    {
        int random = rand();
        reverseArray[i] = random;
        printf("%d ", reverseArray[i]);
        myMap[i] =(char) ( (random%127 + myMap[i])%94 + 32);
    }

    FAIL_IF_MSG(msync(myMap, statBuffer.st_size, MS_SYNC), "Error at syncing!\n");
    FAIL_IF_MSG(munmap(myMap, statBuffer.st_size), "Error at unmapping the map!\n");
    return 0;
}