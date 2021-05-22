#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

int main(int argc, char* argv[])
{
    int fd;
    if(argv[1] == NULL)
    {
        printf("Date de intrare eronate!\n");
        exit(1);
    }
    else
    {
        if( (fd = open("sourceFile.txt", O_RDWR)) == -1)
        {
            printf("Nu pot deschide fisierul sursa: %s\n", strerror(errno));
            exit(2);
        }
    }

    unsigned char ch;
    int nrRead=0;

    struct flock myLock;
    myLock.l_type = F_WRLCK;
    myLock.l_whence = SEEK_CUR;
    myLock.l_start = -1;
    myLock.l_len = 1;

    while( (nrRead = read(fd, &ch, 1)) != 0)
    {
        if(nrRead == -1)
        {
            printf("Eraore la apelul read\n");
            exit(3);
        }

        if(ch == 'x')
        {
            off_t poz;
            if(-1 == (poz = lseek(fd, -1L, 1)))
            {
                printf("Eroare la apelul lseek\n"); 
                exit(4);
            }

            printf("\n[ProcesID:%d] Incep incercarile de punere a lacatului prin apel blocant\n", getpid());
            if(-1 == fcntl(fd, F_SETLK, &myLock) )
            {
                if(errno == EINTR)
                    printf("\n[ProcessID:%d] Eroare la apelul fcntl, user exit.",getpid());
                else
                    printf("\n[ProcessID:%d] Eroare la blocaj", getpid());
            }
            else
                printf("\n[ProcessID:%d] Blocaj reusit!", getpid());

            sleep(3);

            if(-1 == read(fd, &ch, 1))
            {
                printf("eroare la citire\n");
                exit(6);
            }

            if (ch == 'x')
            {
                if (-1 == (poz = lseek(fd, -1L, 1)))
                {
                    printf("Eroare la lseek\n");
                    exit(7);
                }
                if (-1 == write(fd, argv[1], 1))
                {
                    printf("Eroare la apelul write");
                    exit(8);
                }
                printf("\n[ProcessID:%d] S-a inlocuit primul x la pozitia %d",getpid(), poz);
                return 0;
            }
            else
            {
                printf("\n[ProcessID:%d] Caracterul pe care l am gasit a fost deja inlocuit\n",getpid());
                myLock.l_type = F_UNLCK;
                if(-1 == fcntl(fd, F_SETLK, &myLock))
                {
                    printf("eroare la deblocaj\n");
                    exit(9);
                }
                myLock.l_type = F_WRLCK; //sa l am pregatit sa pun locku
            }
        }
    }

    close(fd);
    printf("\n[ProcesID:%d] Terminat. Nu exista 'x' in fisierul sursa !\n", getpid());
    return 0;
}