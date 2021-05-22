#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char** argv)
{
    char* directory = malloc(255);
    if( argc != 2)
    {
        printf("[PID %d] Input your directory\n", getpid());
        if( scanf("%s", directory) == -1 )
        {
            perror("Error at reading ");
            exit(EXIT_FAILURE);
        }
        printf("[PID %d] The directory is %s\n", getpid(), directory);
    }
    else
    {
        strcpy(directory, argv[1]);
    }

    pid_t my_ls = fork();
    if(my_ls == 0) //child process
    {
        execlp("ls", "ls", "-l", directory, (char*) NULL);
        printf("[PID %d] Error at execlp for the ls\n", getpid());
        exit(EXIT_FAILURE);
    }
    //parent

    int my_ls_return;
    wait(&my_ls_return);
    if (my_ls_return != 0)
    {
        printf("[PID %d] ls failed.\n", getpid());
        exit(EXIT_FAILURE);
    }

    
    char option;
    printf("[PID %d] Do you want to remove this directory? y/n ", getpid());
    fflush(stdout);
    read(STDIN_FILENO, &option, 1);

    if (option == 'y')
    {
        int my_rm = fork();
        if (my_rm == 0) //child rm
        {
            printf("[PID %d] Removing the directory %s\n", getpid(), directory);
            char *exec_param[] = {"/usr/bin/rm", "-r", "-f", directory, (char *)NULL};

            execv(exec_param[0], exec_param);
        }

        //parent rm
        int my_rm_return;
        wait(&my_rm_return);
        if (my_rm_return != 0)
        {
            printf("[PID %d] rm failed.\n", getpid());
            exit(EXIT_FAILURE);
        }
        printf("[PID %d] Finished removing the directory %s\n", getpid(), directory);
    }

    free(directory);
    return 0;
}