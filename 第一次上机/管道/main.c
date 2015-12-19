#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define FIFO_NAME "/tmp/myfifo2"
struct w_buf
{
    char message[50];
    struct w_buf *next;
};

int main()
{
    int fd;
    //char w_buf[50];
    struct w_buf *head=(struct w_buf *)malloc(sizeof(struct w_buf));
    strcpy(head->message,"");
    struct w_buf *index=head;
    int w_num;
    char r_buf[50];
    int  r_num;
    pid_t pid;
    // 若fifo已存在，则直接使用，否则创建它
    if((mkfifo(FIFO_NAME,0777)<0)&&(errno!=EEXIST))
    {
        printf("cannot create fifo...\n");
        exit(1);
    }


                                                                                                        
         pid=fork();
         if(pid==0)
	{
                  //以阻塞型只写方式打开fifo
	    fd=open(FIFO_NAME,O_WRONLY,0);
	    if(fd==-1)
		if(errno==ENXIO)
		{
		    printf("cannot open fifo for read...\n");
		    exit(1);
		}
        
		  while(1)
		   {
		       printf("please input something:");
		       scanf("%s",index->message);
                       index->next=NULL;
		       if(strcmp(index->message,"exit")==0) break;
                       struct w_buf *node=(struct w_buf*)malloc(sizeof(struct w_buf));
                       index->next=node;
                       index=index->next;
		    }
                    index=head;
		    int len=0; //好像有问题，改成50试试 
                    while(index!=NULL&&(len=strlen(index->message)))
                    {
                      w_num=write(fd,index->message,len);
		      printf("real write num is %d\n",w_num);
		      index=index->next;
                    }
	}
                                                                                                                            
         else if(pid>0)
         {
                                   fd=open(FIFO_NAME,O_RDONLY,0);
				    if(fd==-1)
				    {
					printf("open  for read error\n");
					exit(1);
				    }
                                       
                                    while(1)
                                     {
					memset(r_buf,0,sizeof(r_buf));
					r_num=read(fd,r_buf,50);
					if(r_num==-1)
					    if(errno==EAGAIN)
						printf("no data avlaible\n");
					printf(" %d bytes read:%s\n",r_num,r_buf);
					if(strcmp(r_buf,"exit")==0) break;
					sleep(1);    
                                    }              

                /* pid=fork();
                 if(pid==0)
			{
                              //以阻塞型只读方式打开fifo
				    fd=open(FIFO_NAME,O_RDONLY,0);
				    if(fd==-1)
				    {
					printf("open  for read error\n");
					exit(1);
				    }

				    // 通过键盘输入字符串，再将其写入fifo，直到输入"exit"为止
				    while(1)
				    {

					memset(r_buf,0,sizeof(r_buf));
					r_num=read(fd,r_buf,50);
					if(r_num==-1)
					    if(errno==EAGAIN)
						printf("no data avlaible\n");
					printf(" %d bytes read:%s\n",r_num,r_buf);
					if(strcmp(r_buf,"exit")==0) break;
					sleep(1);
				    }
				    unlink(FIFO_NAME);//删除fifo
			}
                 else if(pid>0)
                        printf("I'm father process,my pid is %d.\n",getpid());
                 else
                        printf("fork() error.\n");*/
         }
         else printf("fork() error.\n");
}
