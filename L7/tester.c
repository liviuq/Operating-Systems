#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <math.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>    // necesar pentru functia getpwuid
#include <grp.h>    // necesar pentru functia getgrgid
#include <limits.h> // necesar pentru PATH_MAX

void adauga(char sourceIp[15], char destIp[15], int elapsedTime, char execProc[50], char argProc[50], char sha256sum[64]);
void modifica(int id, bool reviewed);
void cauta_id(int id);
void cauta_ip_sursa(char sourceIp[15]);
void cauta_ip_dest(char destIp[15]);
void cauta_reviewed(bool reviewed);

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <database> <modifiers>", argv[0]);
        return 1;
    }

    int logFile;
    if (-1 == (logFile = open(argv[1], O_RDWR | O_CREAT, 0600)))
    {
        printf("Eroare la open\n");
        return 2;
    }

    int modifyFile;
    if (-1 == (modifyFile = open(argv[2], O_RDONLY, 0600)))

    {
        printf("Eroare la open\n");
        return 2;
    }


    int incident = 0;
    if (-1 == read(modifyFile, &incident, sizeof(int)))
    {
        printf("Eraore la citire din fisier\n");
        return 3;
    }
    if (-1 == lseek(logFile, 0, SEEK_END))
    {
        printf("Eroare la seek\n");
        return 3;
    }
    if (-1 == write(logFile, &incident, sizeof(int)))
    {
        printf("Eraore la scriere\n");
        return 3;
    }

    if (-1 == close(logFile))
    {
        printf("Eroare la inchiderea bazei de date!\n");
        return 4;
    }
    if (-1 == close(modifyFile))
    {
        printf("Eroare la inchiderea modificatorului\n");
        return 5;
    }
    return 0;
}