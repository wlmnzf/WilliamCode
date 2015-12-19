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
#define MYKEY 2333
#define SIZE 4096



union semun {
        int val;                    
        struct semid_ds *buf;             
        unsigned short int *array;      
        struct seminfo *__buf;          
};



int sid;


   struct sembuf sem_opt_wait1[1] = {0, -1, SEM_UNDO};

   struct sembuf sem_opt_wakeup1[1] = {0, 1, SEM_UNDO};


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
            strcpy(shm_addr,"");
            printf("please writing\n");
            //sem_wait(&sem_id);
            semop(sid, sem_opt_wait1, 1);
            scanf("%s",shm_addr);
            semop(sid, sem_opt_wakeup1, 1);
        }  
 
       int flag=0;
       while(!flag)
       {
              semop(sid, sem_opt_wait1, 1);
            
          if(strcmp(shm_addr,"over")==0)
           {
             printf("run over\n");
             flag=1;
           }

            semop(sid, sem_opt_wakeup1, 1);
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

            semop(sid, sem_opt_wait1, 1);
			 printf("reader=> %s\n",shm_addr);
			 strcpy(shm_addr,"over");
                        //scanf("%s",shm_addr);
                         semop(sid, sem_opt_wakeup1, 1);
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
   if((sid=semget(MYKEY,1,IPC_CREAT|0666))==-1)
	{
          printf("信号量创建失败\n");
	  return(-1);
	}

    union semun sem_args;

    unsigned short array[1]={1};

    sem_args.array = array;

   int ret = semctl(sid, 1, SETALL, sem_args);

    if (-1 == ret)

    {

       perror("semctl");

       exit(EXIT_FAILURE);

    }

    pid_t fpid;
    fpid = fork();  
    if (fpid < 0)  
        printf("error in fork!");  
    else if (fpid == 0)  
       reader();
    else  
       writer(); 
    return 0;  
}
