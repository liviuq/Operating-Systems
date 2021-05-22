#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("Usage: openReadWrite <source> <dest>\n");
		exit(EXIT_FAILURE);
	}

	int readme = open(argv[1], O_CREAT | O_RDWR, 0600);
	int writeme = open(argv[2], O_CREAT | O_RDWR, 0600);

	if(readme != -1 && writeme != -1)
		printf("Open successfull\n");
	else
	{
		printf("open failed\n");
		exit(EXIT_FAILURE);
	}

	char* buffer = malloc(5);

	ssize_t bytesRead;
	while((bytesRead = read(readme, buffer, 3)))
	{	
		printf("Read %ld bytes\n", bytesRead);
		ssize_t bytesWritten;
		if((bytesWritten= write(writeme, buffer, bytesRead)))
			printf("Written %ld bytes\n", bytesWritten);
		else
			printf("wow\n");
		write(writeme, "\n", 2);
	}
	
	free(buffer);
	if(!close(readme))
		printf("Closing successfull\n");
	else
		printf("Closing failed\n");
	return 0;
}