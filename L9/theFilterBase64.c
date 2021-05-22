#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void encodeblock( unsigned char *in, unsigned char *out, int len );

#define handle_error(msg) \
    { perror(msg); exit(EXIT_FAILURE); }

//table of encoding
static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int main(int argc, char *argv[])
{
    char *mapSource, *mapDest;
    int fdSource, fdDest;

    struct stat sbSource;
    struct stat sbDest;

    ssize_t codw;

    long PageSize = sysconf(_SC_PAGE_SIZE);
    if (PageSize == -1)
        handle_error("Error at sysconf");
    fprintf(stderr, "Notification: the page size on your hardware system is: %ld bytes.\n", PageSize);


    /* Validăm argumentele din linia de comandă. */
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Commandline:  %s source destination\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //Source file
    fdSource = open(argv[1], O_RDWR);
    if (fdSource == -1)
        handle_error("Error at open");

    if (fstat(fdSource, &sbSource) == -1)   /* To obtain file size */
        handle_error("Error at fstat");

    //Dest file
    if( access(argv[2], F_OK) == 0)
    {
        char option;
        printf("Doriti sa se suprascrie fisierul? (y/n): ");
        option = getchar();
        if(option == 'y')
        {
            fdDest = open(argv[2], O_RDWR | O_TRUNC);
        }
        else
        {
            printf("No overwrite\n");
            exit(EXIT_FAILURE);
        }

    }
    else
    {
        fdDest = open(argv[2], O_RDWR | O_CREAT | O_TRUNC);
    }
    
    if (fdDest == -1)
        handle_error("Error at open");

    if (fstat(fdDest, &sbDest) == -1)   /* To obtain file size */
        handle_error("Error at fstat");

    //Source mapping
    mapSource = mmap(NULL,
                     sbSource.st_size,
                     PROT_WRITE,
                     MAP_SHARED, 
                     fdSource,
                     0
    );
    if (mapSource == MAP_FAILED)
        handle_error("Error at mmap1");

    //dest mapping
    ftruncate(fdDest, sbSource.st_size/3 * 4 + 4);
    mapDest = mmap(  NULL,
                     sbSource.st_size/3 * 4 + 4,
                     PROT_WRITE | PROT_READ,
                     MAP_SHARED,
                     fdDest,
                     0
    );
    if (mapDest == MAP_FAILED)
        handle_error("Error at mmap2");


    /* După crearea mapării, descriptorul de fișier poate fi închis imediat, fără a se invalida maparea ! */
    if (-1 == close(fdSource) )
        handle_error("Error at close");
    if (-1 == close(fdDest) )
        handle_error("Error at close");


    //Conversia in BASE64
    unsigned char *buffer = malloc(3);
    unsigned char *base64Buffer = malloc(4);

    for(int i = 0, j = 0; i < sbSource.st_size; i+=3, j+=4)
    {
        // buffer[0] = mapSource[i];
        // buffer[1] = mapSource[i+1];
        // buffer[2] = mapSource[i]+2;

        memcpy(buffer, mapSource+i, 3 );
        encodeblock(buffer, base64Buffer, strlen(buffer));
        memcpy(mapDest+j, base64Buffer, 4);

        // mapDest[j] = base64Buffer[0];
        // mapDest[j+1] = base64Buffer[1];
        // mapDest[j+2] = base64Buffer[2];
        // mapDest[j+3] = base64Buffer[3];
    }

    free(buffer);
    free(base64Buffer);

    if (-1 == msync(mapDest, sbSource.st_size, MS_SYNC) )
        handle_error("Error at msync");

    if (-1 == munmap(mapSource, sbSource.st_size) )
        handle_error("Error at munmap");
    if (-1 == munmap(mapDest, sbSource.st_size/3 * 4 + 4) )
        handle_error("Error at munmap");

    exit(EXIT_SUCCESS);
}

static void encodeblock( unsigned char *in, unsigned char *out, int len )
{
    out[0] = (unsigned char) cb64[ (int)(in[0] >> 2) ];
    out[1] = (unsigned char) cb64[ (int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)) ];
    out[2] = (unsigned char) (len > 1 ? cb64[ (int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)) ] : '=');
    out[3] = (unsigned char) (len > 2 ? cb64[ (int)(in[2] & 0x3f) ] : '=');
}