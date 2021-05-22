#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
    pthread_mutex_t lock;
    if( pthread_mutex_init(&lock, NULL) != 0 )
    {
        printf("mutex init failed\n");
        exit(1);
    }
    pthread_mutex_lock(&lock);
    pthread_mutex_unlock(&lock);
    return 0;
}