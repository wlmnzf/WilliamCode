#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <semaphore.h> 
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>


#define KEY 1234
#define SIZE 4096

char MUTEX_NAME[]= "mutex";
char SYNC_NAME[]= "syncsender";
sem_t *sem_id;
int main()
{
     sem_unlink(MUTEX_NAME);
     sem_unlink(SYNC_NAME);
     sem_unlink("syncreceiver");
}


