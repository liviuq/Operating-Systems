#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>   // necesar pentru variabila errno si constantele simbolice asociate codurilor numerice de eroare
#include <limits.h>  // necesar pentru PATH_MAX

#define BLOCK_SIZE 3

int base64mine(char *nume_sursa, char *nume_dest);

int main(int argc, char* argv[])
{
	char fileInput[PATH_MAX], fileOutput[PATH_MAX];

	if(argc < 3)
	{
		fprintf(stderr, "Argumente insuficiente. Usage: thefilterbase64 <FILE_INPUT> <FILE_OUTPUT>\n");
		exit(1);
	}
	else
	{
		strcpy(fileInput,argv[1]);
		strcpy(fileOutput,argv[2]);
	}


	if(base64mine(fileInput, fileOutput) != 0)
	{
		printf("Not good\n");
		exit(1);
	}
	return 0;
}
//get value of a character
//scriu val lor pe 6 biti
//odata ce avem 3 caractere
//impart vectorul in 4 parti(daca avem 24 biti)
//fac translatarea in decimal a celor 4 perechi de 6 biti

int base64mine(char *nume_sursa, char *nume_dest)
{
    int input_fd, output_fd;
    ssize_t bytes_in, bytes_out;
    char buffer[BLOCK_SIZE];

    if( -1 == (input_fd = open(nume_sursa, O_RDONLY)) )
    {
        fprintf(stderr,"Eroare la deschiderea fisierului sursa %s\n",nume_sursa);
        perror("Cauza este");  return 20;
    }

    if( -1 == (output_fd = open(nume_dest, O_WRONLY|O_CREAT|O_TRUNC, 00200)) )
    {
        fprintf(stderr,"Eroare la deschiderea fisierului destinatie %s\n",nume_dest);
        perror("Cauza este");  return 21;
    }

    while (1)
    {
        if( -1 == (bytes_in = read(input_fd, buffer, BLOCK_SIZE)) )
        {
            fprintf(stderr,"Eroare la citirea din fisierul %s\n",nume_sursa);
            perror("Cauza este");  return 22;
        }
        if(bytes_in == 0) break; // am ajuns la EOF in fisierul de intrare
		
        //if( -1 == (bytes_out = write(output_fd, buffer, bytes_in)) )
        //{
        //   fprintf(stderr,"Eroare la scrierea in fisierul %s\n",nume_dest);
        //    perror("Cauza este");  return 23;
        //}		

        char bits[BLOCK_SIZE * 8];
        //char output[8];

        for(int i =  0; i <BLOCK_SIZE; i++)
        {
        	int character = (int)buffer[i];
        	for(int j = 7; j >= 0; j--)
        	{
        		bits[j + i*8] = character%2;
        		character /= 2;
        	}
        }

        bytes_out = write(output_fd, bits, 8);

        //if (bytes_out != bytes_in)
        //{
        //    fprintf(stderr, "Fatal write error: scriere incompleta!");  return 24;
        //}
    }

    close(input_fd);
	close(output_fd);

    printf("Am copiat cu succes fisierul sursa %s in destinatia %s\n", nume_sursa, nume_dest);
    return 0;
}