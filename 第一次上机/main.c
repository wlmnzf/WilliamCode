#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<signal.h>
#include<semaphore.h> 

#define KEY 1234
#define SIZE 4096
sem_t  sem_id;

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
        else  
        {  
           //printf("");
            strcpy(shm_addr,"");
            printf("please writing\n");
            sem_wait(&sem_id);
            scanf("%s",shm_addr);
            sem_post(&sem_id);
        }  
 
       int flag=0;
       while(!flag)
       {
          sem_wait(&sem_id);
          if(strcmp(shm_addr,"over")==0)
           {
             printf("run over\n");
             flag=1;
           }
          sem_post(&sem_id);
          if(flag==1)
               sem_destroy(&sem_id);   
       }
    

      if((shmdt(shm_addr)) < 0)  
        {  
            perror("shmdt");  
            exit(1);  
        }  
         return 0;
}

int reader()
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
        else  
        {  
            int flag=0;
            while(!flag)
            {   
                    if(strlen(shm_addr)>0)
                     {
                         flag=1;
                         sem_wait(&sem_id);
			             printf("reader=> %s\n",shm_addr);
			             strcpy(shm_addr,"over");
                         sem_post(&sem_id);

                     } 
            }

        }  



  if((shmdt(shm_addr)) < 0)  
        {  
            perror("shmdt");  
            exit(1);  
        }  

         return 0;
}

int main()
{
    sem_init(&sem_id,1,1);
    pid_t fpid;
    fpid = fork();  
    if (fpid < 0)  
        printf("error in fork!");  
    else if (fpid == 0)  
       //reader();
        writer();
    else  
       // writer(); 
      reader();
    return 0;  
}
