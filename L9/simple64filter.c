#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void encodeblock( unsigned char *in, unsigned char *out, int len );

//encoding table
static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int main(int argc, char* argv[])
{
    int fdSource, fdDest;

    if((fdSource = open(argv[1], O_RDONLY)) == -1)
    {
        printf("1");
        exit(1);
    }

    if ((fdDest = open(argv[2], O_WRONLY | O_TRUNC)) == -1)
    {
        printf("2");
        exit(2);
    }

    ssize_t bytesIn;
    unsigned char* buffer = malloc(3);
    unsigned char* base64Buffer = malloc(4);

    while((bytesIn = read(fdSource, buffer, 3)))
    {
        encodeblock(buffer, base64Buffer, bytesIn);
        if(( write(fdDest, base64Buffer, 4)) == -1)
        {
            printf("2");
            exit(3);
        }
    }

    free(buffer);
    free(base64Buffer);
    close(fdSource);
    close(fdDest);
    
    return 0;
}

static void encodeblock( unsigned char *in, unsigned char *out, int len )
{
    out[0] = (unsigned char) cb64[ (int)(in[0] >> 2) ];
    out[1] = (unsigned char) cb64[ (int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)) ];
    out[2] = (unsigned char) (len > 1 ? cb64[ (int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)) ] : '=');
    out[3] = (unsigned char) (len > 2 ? cb64[ (int)(in[2] & 0x3f) ] : '=');
}