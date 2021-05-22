#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>    // necesar pentru functia getpwuid
#include <grp.h>    // necesar pentru functia getgrgid
#include <limits.h> // necesar pentru PATH_MAX

void parcurgere_director(char *cale);

int main(int argc, char* argv[])
{

	struct stat statStruct;

	if(1 == argc)
	{
		printf("Usage: %s <dir_to_process>\n", argv[0]);
		exit(1);
	}

	if(0 != stat(argv[1], &statStruct))
	{
		printf("Eroare la stat\n");
		exit(2);
	}

	if(! S_ISDIR(statStruct.st_mode))
	{
		printf("%s nu este director!\n", argv[0]);
		exit(3);
	}

	printf("analizare director!\n");
	parcurgere_director(argv[1]);
	printf("Parcurgere realizata.\n");

	return 0;
}

int afisare_fileinfo(char* cale);
void parcurgere_director(char *cale);

void parcurgere_director(char *cale)
{
	DIR* directory;
	struct dirent *de;

	char nume[PATH_MAX];
	int isFolder;


	isFolder = afisare_fileinfo(cale);
	if(isFolder != 1)
	{
		return;
	}
	else
	{
		if(NULL == (directory = opendir(cale)))
		{
			printf("Eraore la deschiderea directorului!\n");
			return;
		}

		while(NULL != (de = readdir(directory)))
		{
			if(strcmp(de->d_name, ".") && strcmp(de->d_name, "..") )
			{
				sprintf(nume, "%s/%s", cale, de->d_name);
				parcurgere_director(nume);
			}
		}

		closedir(directory);
	}
}

int afisare_fileinfo(char* cale)
{
	struct stat st;
	char perm[10] = "---------";
	int result = 0;

	if(0 != stat(cale, &st))
	{
		printf("Eroare la stat");
		return 2;
	}

	printf("Tipul directorului: ");
	switch(st.st_mode & S_IFMT)
	{
		case S_IFBLK:  printf("block device\n");            break;
        case S_IFCHR:  printf("character device\n");        break;
        case S_IFDIR:  printf("directory\n");               break;
        case S_IFIFO:  printf("FIFO/pipe\n");               break;
        case S_IFLNK:  printf("symlink\n");                 break;
        case S_IFREG:  printf("regular file\n");            break;
        case S_IFSOCK: printf("socket\n");                  break;
        default:       printf("unknown?\n");                break;
	}

	printf("\tPermisiunile acestuia, in notatie octala: %o\n", st.st_mode & 0777 );

    if( S_IRUSR & st.st_mode )  perm[0]='r';
    if( S_IWUSR & st.st_mode )  perm[1]='w';
    if( S_IXUSR & st.st_mode )  perm[2]='x';
    if( S_IRGRP & st.st_mode )  perm[3]='r';
    if( S_IWGRP & st.st_mode )  perm[4]='w';
    if( S_IXGRP & st.st_mode )  perm[5]='x';
    if( S_IROTH & st.st_mode )  perm[6]='r';
    if( S_IWOTH & st.st_mode )  perm[7]='w';
    if( S_IXOTH & st.st_mode )  perm[8]='x';

    printf("\tPermisiunile acestuia, in notatie simbolica: %s\n", perm );
    return result;
}

