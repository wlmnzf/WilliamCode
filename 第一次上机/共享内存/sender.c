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
char SYNC_SENDER_NAME[]="syncsender";
char SYNC_RECEIVER_NAME[]="syncreceiver";
sem_t *mutex;
sem_t *syncsender;
sem_t *syncreceiver;

int writer()
{
    int shmid;  
    char *shm_addr;  
    char *buff;  

    if ((shmid = shmget((key_t)(KEY), sizeof(SIZE), 0666|IPC_CREAT)) < 0)  
    {  
        perror("shmget");  
        exit(1);  
    }  

    if ((shm_addr = shmat(shmid, 0, 0)) == (void*)-1)  
        {  
            perror("Child: shmat");  
            exit(1);  
        }  
 
     int flag=0;
     int index=1;
     strcpy(shm_addr,"");
       while(!flag)
       {
       	    sem_wait(syncsender);
            sem_wait(mutex);
            printf("please writing,(input 'exit' to quit)\n");
            scanf("%s",shm_addr);
            if(strcmp(shm_addr,"exit")==0)
            {
            	flag=1;
            	printf("\nsend end\n");
            }
            else
            {
            	printf("sender say:%s\n",shm_addr);
            }

            sem_post(mutex);
            sem_post(syncreceiver);


	          sem_wait(syncsender);
	          while(strcmp(shm_addr,"over")!=0);
	          sem_wait(mutex);
	             printf("\n");
	             strcpy(shm_addr,"");
	          sem_post(mutex);
	          sem_post(syncsender);
	          
       }
    

    //if(flag==1)
      //{
           sem_close(mutex);
           sem_close(syncsender);
           sem_close(syncreceiver);
    	   sem_unlink(MUTEX_NAME);
    	   sem_unlink(SYNC_SENDER_NAME);
    	   sem_unlink(SYNC_RECEIVER_NAME);
    //   }

      if((shmdt(shm_addr)) < 0)  
        {  
            perror("shmdt");  
            exit(1);  
        }  
         return 0;
}


int main()
{
    // sem_unlink(SEM_NAME);
	mutex= sem_open(MUTEX_NAME,O_CREAT,0666,1);
	syncsender = sem_open(SYNC_SENDER_NAME,O_CREAT,0666,1);
    syncreceiver = sem_open(SYNC_RECEIVER_NAME,O_CREAT,0666,0);

	if(syncsender == SEM_FAILED||mutex==SEM_FAILED||syncreceiver==SEM_FAILED)
    {
      perror("unable to create semaphore");
      sem_unlink(MUTEX_NAME);
      sem_unlink(SYNC_SENDER_NAME);
      sem_unlink(SYNC_RECEIVER_NAME);
      exit(-1);
    }
    // sem_init(sem_id,1,1);
    writer();
}
