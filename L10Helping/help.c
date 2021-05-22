#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    char* buffer = malloc(256);

    FILE* source = NULL;
    source = fopen("source.txt", "r");

    while( fgets(buffer, 255, source) != NULL)
    {
        printf("%s", buffer);
    }

    free(buffer);
    fclose(source);
    return 0;   
}