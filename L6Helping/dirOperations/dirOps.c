#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    DIR* directory;
    struct dirent *data;

    if(NULL == (directory = opendir(argv[1])))
    {
        printf("oops\n");
        return 1;
    }
    data = readdir(directory);

    while(NULL != (data = readdir(directory)))
    {
        if(!(strcmp(data->d_name, ".") && strcmp(data->d_name, "..")))
            continue;
        else
            printf("%s\n", data->d_name);
    }

    closedir(directory);

    return 0;
}