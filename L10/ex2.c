#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

#define FAIL_IF_MSG( EXP , MSG) ( {if( EXP == NULL) {printf(MSG);exit(EXIT_FAILURE);}} )

void dialog_tata();
void dialog_fiu();

void scrie0();
void scrie1();
void scrie2();

char zero = '0';
char unu = '1';

int main()
{
	pid_t pid_fiu;

    int fileDescriptorflag;
    if( (fileDescriptorflag = open("flag.bin", O_RDWR)) == -1)
    {
        printf("Eroare la deschiderea fisierului cu flag!\n");
        exit(EXIT_FAILURE);
    }
    ftruncate(fileDescriptorflag, 1);
    if( write(fileDescriptorflag, &zero, 1) == -1)
    {
        printf("Eroare la scrierea in fisierul cu flag!\n");
        exit(EXIT_FAILURE);
    }

	if(-1 == (pid_fiu=fork()) )
	{
		perror("Eroare la fork");
        return 1;
	}

	/* Ramificarea execuției în cele două procese, tată și fiu. */
	if(pid_fiu == 0)
	{   /* Zona de cod executată doar de către fiu. */
		dialog_fiu();
	}
	else
	{   /* Zona de cod executată doar de către părinte. */
		dialog_tata();
	}

	printf("Sfarsitul executiei procesului %s.\n\n", pid_fiu == 0 ? "fiu" : "parinte" );
    close(fileDescriptorflag);
	return 0;
}

void dialog_tata()
{
    char* buffer = malloc(256);

    FILE* fileTata;
    FAIL_IF_MSG((fileTata = fopen("replici-parinte.txt", "r")) , "Failed to open the father file!\n");

    int fileDescriptorBinary;
    if( (fileDescriptorBinary = open("flag.bin", O_RDWR)) == -1)
    {
        printf("Eroare la deschiderea fisierului cu flag!\n");
        exit(EXIT_FAILURE);
    }

    char flagValue;
    while((buffer = fgets( buffer, 255, fileTata)) != NULL)
    {
        printf("%s", buffer);

        if( write(fileDescriptorBinary, &unu, 1) == -1)
        {
            printf("Eroare la scrierea in fisierul cu flag!\n");
            exit(EXIT_FAILURE);
        }
        lseek(fileDescriptorBinary, 0, SEEK_SET);

        do
        {
            read(fileDescriptorBinary, &flagValue, 1);
            lseek(fileDescriptorBinary, 0, SEEK_SET);
            sleep(2);
        } while( flagValue != '0');
    }   
	
    if(fclose(fileTata) != 0)
    {
        printf("Error at closing the parent file!\n");
        exit(EXIT_FAILURE);
    }
    close(fileDescriptorBinary);
    free(buffer);
}

void dialog_fiu()
{
    char* buffer = malloc(256);
    FILE* fileFiu;
    FAIL_IF_MSG((fileFiu = fopen("replici-fiu.txt", "r")) , "Failed to open the son file!\n");

    int fileDescriptorBinary;
    if( (fileDescriptorBinary = open("flag.bin", O_RDWR)) == -1)
    {
        printf("Eroare la deschiderea fisierului cu flag!\n");
        exit(EXIT_FAILURE);
    }

    char flagValue;
    while((buffer = fgets( buffer, 255, fileFiu)) != NULL)
    {
        do
        { 
            read(fileDescriptorBinary, &flagValue, 1);
            lseek(fileDescriptorBinary, 0, SEEK_SET);
            sleep(2);
        } while( flagValue != '1' );

        printf("%s", buffer);

        if( write(fileDescriptorBinary, &zero, 1) == -1)
        {
            printf("Eroare la scrierea in fisierul cu flag!\n");
            exit(EXIT_FAILURE);
        }
        lseek(fileDescriptorBinary, 0, SEEK_SET);
    } 

    if(fclose(fileFiu) != 0)
    {
        printf("Error at closing the kid!\n");
        exit(EXIT_FAILURE);
    }

    close(fileDescriptorBinary);
    free(buffer);
}
