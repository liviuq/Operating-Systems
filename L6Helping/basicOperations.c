#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char* argv[])
{
	//CREATES A FILE
	int value = creat("pizza.txt", S_IRUSR | S_IWUSR);
	if(value != -1)
		printf("Creat succesfullll\n");
	else printf("oops\n");

	//CHECK IF STAT RETURNED OK
	struct stat statusfile;
	if(!stat("ciaoless.txt", &statusfile))
		printf("Stat check OK\n");
	else printf("Stat check FAILEd\n");

	printf("Size is %d\n", statusfile.st_mode);


	//CHECK FOR FILE TYPE
	mode_t valsimple = (statusfile.st_mode & S_IFMT);
	printf("valsimple = %d\n", valsimple);
	switch (statusfile.st_mode & S_IFMT) {
    case S_IFBLK:  printf("block device\n");            break;
    case S_IFCHR:  printf("character device\n");        break;
    case S_IFDIR:  printf("directory\n");               break;
    case S_IFIFO:  printf("FIFO/pipe\n");               break;
    case S_IFLNK:  printf("symlink\n");                 break;
    case S_IFREG:  printf("regular file\n");            break;
    case S_IFSOCK: printf("socket\n");                  break;
    default:       printf("unknown?\n");                break;
    }

    //CHECK FOR FILE PERMISSION
	mode_t val=(statusfile.st_mode & ~S_IFMT);
	printf("val of val = %d\n", val);
    (val & S_IRUSR) ? printf("r") : printf("-");
    (val & S_IWUSR) ? printf("w") : printf("-");    
    (val & S_IXUSR) ? printf("x") : printf("-");
    (val & S_IRGRP) ? printf("r") : printf("-");
    (val & S_IWGRP) ? printf("w") : printf("-");
    (val & S_IXGRP) ? printf("x") : printf("-");
    (val & S_IROTH) ? printf("r") : printf("-");
    (val & S_IWOTH) ? printf("w") : printf("-");
    (val & S_IXOTH) ? printf("x") : printf("-");
    
    int accessval;
    if(!(accessval = access("ciaoless.txt", F_OK | R_OK | W_OK | X_OK)))
    	printf("I have the perms\n");
    else printf("sorry, no perms\n");

	printf("\n");
	close(value);
	return 0;
}