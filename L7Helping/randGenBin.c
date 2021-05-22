#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define FAIL_IF_MSG( EXP , MSG) ( {if( EXP == -1) {printf(MSG);exit(EXIT_FAILURE);}} )

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        printf("Usage: %s <source_file> <nr_of_rand_numbers>\n", argv[0]);
        printf("You inputted %d arguments", argc - 1 );
        exit(EXIT_FAILURE);
    }

    int sourceFileDescriptor;
    FAIL_IF_MSG((sourceFileDescriptor = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0644)) ,"Error at opening the file!\n");

    int nrOfnumbers = atoi(argv[2]);
    truncate(argv[1], nrOfnumbers * sizeof(nrOfnumbers));

    srand(time(NULL));    
    for(int i = 0; i < nrOfnumbers; i++)
    {
        int random = rand() % 1000;
        FAIL_IF_MSG( write(sourceFileDescriptor, &random, sizeof(random)),"Error at writing the number!\n");
    }

    FAIL_IF_MSG( close(sourceFileDescriptor), "Failed to close the source file!\n");
    return 0;
}