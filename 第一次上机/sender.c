#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<signal.h>

struct  data
{
       int flag;	/* 1 writen  0 read*/
       char data[100000];
};

int main()
{

    pid_t pid;  
    int shmid;  
    char *shm_addr;  
    char flag[] = "WROTE";  
    char *buff;  
    int key=1444;

    if ((shmid = shmget((key_t)(key), sizeof(struct data), 0666|IPC_CREAT)) < 0)  
    {  
        perror("shmget");  
        exit(1);  
    }  
    else  
    {  
        printf("Create shared-memory: %d\n", shmid);  
    }  
  
    //显示共享内存的情况  
   // system("ipcs -m");  



  if ((shm_addr = shmat(shmid, 0, 0)) == (void*)-1)  
        {  
            perror("Child: shmat");  
            exit(1);  
        }  
        else  
        {  
            struct  data *shared=( struct  data *)shm_addr ;
            while(  shared->flag==1)
            	   {
                          sleep(1);
                          printf("wait");
            	   }
            shared->flag=1;
            printf("please writing");
            scanf("%s",shared->data);

             shared->flag=0;
            printf("Child:Attach shared-memory: %p\n", shm_addr);  
        }  
     //   system("ipcs -m");  



  if((shmdt(shm_addr)) < 0)  
        {  
            perror("shmdt");  
            exit(1);  
        }  
        else  
        {  
            printf("Parent: Deattach shared-memory\n");  
        }  
  


         return 0;
}
