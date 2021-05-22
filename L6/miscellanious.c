#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define ASCIINUM 255

int main(int argc, char* argv[])
{
    char numeFisier[PATH_MAX];

    if(argc == 1)
    {
        printf("Intoduceti numele fisierului!\n");
        scanf("%s", numeFisier);
    }
    else
    {
        strcpy(numeFisier, argv[1]);
    }

    int vectorAp[ASCIINUM];
    for(int i = 0; i < ASCIINUM; i++) vectorAp[i] = 0;

    int fd;
    if((fd = open(numeFisier, O_RDONLY)) == -1)
    {
        printf("Eroare la deschidere!\n");
        return 1;
    }

    unsigned char ch;
    int cod_return;
    while((cod_return = read(fd, &ch, 1)) != 0)
        vectorAp[ch]++;

    if(close(fd) == -1)
    {
        printf("eroare la inchidere!\n");
        return 2;
    }

    for(int i = 0; i < ASCIINUM; i++)
        if(vectorAp[i] != 0)
            printf("Char %c cu codul ascii %d a aparut de %d ori\n", i, i, vectorAp[i]);
            
    return 0;
}