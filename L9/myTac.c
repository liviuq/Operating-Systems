#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>


#define FAIL_IF_MSG( EXP , MSG) ( {if( EXP == -1) {printf(MSG);exit(EXIT_FAILURE);}} )

void myTac(const char* source, char delim);

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("Usage: %s [input1,2,..] <flags>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("The number of parameters is %d\n", argc);
        if (strcmp(argv[argc - 2], "-s") == 0)
        {
            for(int i = 1; i < argc - 2; i++)
                myTac(argv[i], *argv[argc - 1]);
        }
        else
        {
            for (int i = 1; i < argc; i++)
                myTac(argv[i], NULL);
        }
    }
    return 0;
}

void myTac(const char* source, char delim)
{
    char* sourceMap = NULL;
    int fdSource;
    struct stat sourceStatus;

    FAIL_IF_MSG((fdSource = open(source, O_RDONLY)), "Error at opening the source\n");
    FAIL_IF_MSG( (fstat(fdSource, &sourceStatus)), "Error at getting informations in fstat\n");

    if(access(source, F_OK) != 0)
    {
        printf("Error: not enough permissions\n");
        exit(EXIT_FAILURE);
    }

    sourceMap = mmap(NULL, sourceStatus.st_size, PROT_READ, MAP_SHARED, fdSource, 0);
    if(sourceMap == MAP_FAILED)
        printf("Error at mapping the file\n");


    int lineLength = 0;
    char* buffer = malloc(512);

    if (delim == NULL)
    {
        for (int i = sourceStatus.st_size - 1; i >= 0; i--)
        {

            if (sourceMap[i] != '\n' && i)
                buffer[lineLength++] = sourceMap[i];
            else
            {
                for (int j = lineLength - 1; j >= 0; j--)
                    printf("%c", buffer[j]);
                printf("\n");
                lineLength = 0;
            }
        }
    }
    else
    {
        for (int i = sourceStatus.st_size - 1; i >= 0; i--)
        {

            if ((sourceMap[i] != delim ) && i)
                buffer[lineLength++] = sourceMap[i];
            else
            {
                buffer[lineLength] = '\0';
                for (int j = lineLength - 1; j >= 0; j--)
                    printf("%c", buffer[j]);
                printf("\n");
                lineLength = 0;
            }
        }
    }

    free(buffer);
}