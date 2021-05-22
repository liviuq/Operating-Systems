#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int writeme;
	if(argc < 3)
	{
		printf("Usage: ./set1ex1 <source> <dest>\n");
		exit(EXIT_FAILURE);
	}

	if( access(argv[2], F_OK) == 0)
	{
		printf("This file already exists.\nDo you want to overwrite it? Y/N\n");
		char* answer = malloc(1);
		if(read(STDIN_FILENO, answer, 1))
		{
			printf("The answer is %c\n", *answer);
		}
		else
		{
			printf("Wrong answer. Exit.\n");
			exit(EXIT_FAILURE);
		}

		if(*answer == 'Y')
		{
			printf("Overwriting..\n");
			writeme = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, 0600);
		}
		else 
		{
			printf("Can't create. Abort\n");
			exit(EXIT_FAILURE);
		}

		free(answer);
	}
	else
	{
		writeme = open(argv[2], O_CREAT | O_RDWR, 0600);
	}
	int readme = open(argv[1], O_RDWR);

	char* buffer = malloc(1);

	//ssize_t for bytesRead and bytesWritten
	 int bytesRead = 0, bytesWritten = 0;
	while((bytesRead = read(readme, buffer, 1)))
	{
		if(*buffer == 65 || 
			*buffer == 69 || 
			*buffer == 73 || 
			*buffer == 79 || 
			*buffer == 85 || 
			*buffer == 97 || 
			*buffer == 101 || 
			*buffer == 105 || 
			*buffer == 111 || 
			*buffer == 117)
			continue;
		else
		{
			bytesWritten = write(writeme, buffer, bytesRead);
			if(bytesRead != bytesWritten)
			{
				printf("Writing error.\n");
				exit(EXIT_FAILURE);
			}

		}

	}

	close(readme);
	close(writeme);
	return 0;
}